#include <renderer.h>
#include <player.h>
#include <map.h>

#include <stdexcept>

#include <cmath>
#include <algorithm>

#include <SDL2/SDL.h>

namespace Raynder {

void Renderer::set_drawing_color(const uint8_t r, const uint8_t g, const uint8_t b) const {
    if (SDL_SetRenderDrawColor(this->context, r, g, b, 255)) {
            throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::draw_rectangle(
    const uint16_t origin_x, 
    const uint16_t origin_y, 
    const uint8_t width, 
    const uint8_t height,
    FillType filled) const 
{
    SDL_Rect rect;
    rect.x = origin_x;
    rect.y = origin_y;
    rect.w = width;
    rect.h = height;

    if (!filled) {
        if (SDL_RenderDrawRect(this->context, &rect)) {
                throw std::runtime_error(SDL_GetError());
        }
    } else {
        if (SDL_RenderFillRect(this->context, &rect)) {
            throw std::runtime_error(SDL_GetError());
        }
    }
    
}

void Renderer::draw_line(
    const uint16_t x1, 
    const uint16_t y1, 
    const int16_t x2, 
    const int16_t y2
) const {
    if (SDL_RenderDrawLine(this->context, x1, y1, x2, y2)) {
        throw std::runtime_error(SDL_GetError());
    }
}

CartesianPair Renderer::cast_ray(const float relative_angle_to_player) const {
    const float angle = this->player_ptr->get_rotation() + relative_angle_to_player;
    
    CartesianPair pos;
    pos.x = this->player_ptr->get_pos_x();
    pos.y = this->player_ptr->get_pos_y();

    const uint8_t side_length = this->map_ptr->get_side_length();

    CartesianPair pos_in_tile;
    pos_in_tile.x = pos.x - ((int)pos.x/side_length) * side_length;
    pos_in_tile.y = pos.y - ((int)pos.y/side_length) * side_length;

    const float cos_angle = cos(angle);
    const float sin_angle = sin(angle);
    const float tan_angle = sin_angle/cos_angle;
    const float cot_angle = cos_angle/sin_angle;

    CartesianPair Dv;
    float Dv_unit_x = side_length;

    if (cos_angle < 0) {
        Dv.x = -pos_in_tile.x;
        Dv.y = Dv.x * tan_angle;

        Dv_unit_x *= -1.0;
    
    } else if (cos_angle > 0) {
        Dv.x = side_length - pos_in_tile.x;
        Dv.y = Dv.x * tan_angle;
    }

    const float wall_increment_v = Dv_unit_x * tan_angle;
    bool hit_wall_v = false;

    while (!hit_wall_v) {
        const uint8_t index_y = (int)((pos.y + Dv.y)/side_length);
        const uint8_t index_x = (int)((pos.x + Dv.x)/side_length) - int(Dv.x < 0);

        if (index_y >= this->map_ptr->get_row_count() || index_x >= this->map_ptr->get_column_count()) {
            hit_wall_v = true;
            break;
        }

        if (this->map_ptr->get_data(index_x, index_y)) {
            hit_wall_v = true;
        } else {
            Dv.x += Dv_unit_x;
            Dv.y += wall_increment_v;
        }
    }

    CartesianPair Dh;
    float Dh_unit_y = side_length;

    if (sin_angle < 0) {
        Dh.y = -pos_in_tile.y;
        Dh.x = Dh.y * cot_angle;

        Dh_unit_y *= -1.0;
    } else if (sin_angle > 0) {
        Dh.y = side_length - pos_in_tile.y;
        Dh.x = Dh.y * cot_angle;
    }

    const float wall_increment_h = Dh_unit_y * cot_angle;
    bool hit_wall_h = false;

    while (!hit_wall_h) {
        const uint8_t index_y = (int)((pos.y + Dh.y)/side_length) - int(Dv.y < 0);
        const uint8_t index_x = (int)((pos.x + Dh.x)/side_length);

        if (index_y >= this->map_ptr->get_row_count() || index_x >= this->map_ptr->get_column_count()) {
            hit_wall_h = true;
            break;
        }

        if (this->map_ptr->get_data(index_x, index_y)) {
            hit_wall_h = true;
        } else {
            Dh.y += Dh_unit_y;
            Dh.x += wall_increment_h;
        }
    }


    CartesianPair resultant_ray = (pow(Dv.x, 2) + pow(Dv.y, 2) <= pow(Dh.x, 2) + pow(Dh.y, 2)) ? Dv : Dh;

    this->draw_line(
        pos.x, 
        pos.y, 
        pos.x + resultant_ray.x, 
        pos.y + resultant_ray.y
    );

    return resultant_ray;
}


void Renderer::set_map_ptr(Map* map_ptr) {
    this->map_ptr = map_ptr;
}

void Renderer::set_player_ptr(Player* player_ptr) {
    this->player_ptr = player_ptr;
}

void Renderer::draw_debug_topdown_grid() const {

    const uint8_t row_count = this->map_ptr->get_row_count();
    const uint8_t col_count = this->map_ptr->get_column_count();
    const uint8_t side_length = this->map_ptr->get_side_length();

    this->set_drawing_color(255, 255, 255);
    
    for (size_t i = 0; i < col_count; ++i) {
        for (size_t j = 0; j < row_count; ++j) {
            const uint16_t rect_x = i * side_length;
            const uint16_t rect_y = j * side_length;
            
            if (this->map_ptr->get_data(i, j) == 0) {
                this->set_drawing_color(255, 0, 0);
                this->draw_rectangle(rect_x, rect_y, side_length, side_length, FillType::NOT_FILLED);
            } else {
                this->set_drawing_color(255, 255, 255);
                this->draw_rectangle(rect_x, rect_y, side_length, side_length, FillType::FILLED);
            }

        }

    }

    
}

void Renderer::draw_debug_topdown_player() const {
    this->set_drawing_color(0, 255, 0);

    const uint16_t pos_x = this->player_ptr->get_pos_x();
    const uint16_t pos_y = this->player_ptr->get_pos_y();
    const uint8_t side_length = 2 * this->player_ptr->get_collision_radius();
    
    this->draw_rectangle(
        pos_x - side_length/2.0, 
        pos_y - side_length/2.0, 
        side_length, 
        side_length, 
        FillType::FILLED
    );

    this->set_drawing_color(255, 255, 0);
    this->cast_ray(0);
}

void Renderer::clear_display() const {
    this->set_drawing_color(0, 0, 0);
    if (SDL_RenderClear(this->context)) {
        throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::update_display() const {
    SDL_RenderPresent(this->context);
}

void Renderer::render_loop() const {
    if (!this->map_ptr || !this->player_ptr) {
        throw std::runtime_error("Rendering cannot proceed without valid Map and Player objects.");
    }

    this->clear_display();
    
    this->draw_debug_topdown_grid();
    this->draw_debug_topdown_player();
    
    this->update_display();
}

Renderer::Renderer(const uint16_t window_width, const uint16_t window_height) {       
    this->window_width = window_width;
    this->window_height = window_height;

    this->window = SDL_CreateWindow(
        "RaynderCast", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        0
    );
    if (this->window == NULL) {
        throw std::runtime_error(SDL_GetError());
    }

    this->context = SDL_CreateRenderer(
        this->window, -1, 0
    );
    if (this->context == NULL) {
        throw std::runtime_error(SDL_GetError());
    }

}

Renderer::~Renderer() {
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->context);
}

}