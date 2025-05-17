#include <renderer.h>
#include <player.h>
#include <map.h>

#include <stdexcept>

#include <string>

#include <cmath>
#include <algorithm>

#include <SDL2/SDL.h>

namespace Raynder {

void Renderer::set_drawing_color(const uint8_t r, const uint8_t g, const uint8_t b) const {
    if (SDL_SetRenderDrawColor(this->context, r, g, b, 255)) {
            throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::set_drawing_color(const Color color) const {
    this->set_drawing_color(color.r, color.g, color.b);
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

void Renderer::draw_point(const uint16_t x, const uint16_t y) const {
    if (SDL_RenderDrawPoint(this->context, x, y)) {
        throw std::runtime_error(SDL_GetError());
    }
}

HitData Renderer::cast_ray(const float relative_angle_to_player) const {
    const float angle = this->player_ptr->get_rotation() + relative_angle_to_player;
    
    CartesianPair pos;
    pos.x = this->player_ptr->get_pos_x();
    pos.y = this->player_ptr->get_pos_y();

    const uint8_t side_length = this->map_ptr->get_side_length();

    // by constructor design, pos can't be negative and no checks are done in this regard.
    CartesianPair pos_in_tile;
    pos_in_tile.x = pos.x - ((int)pos.x/side_length) * side_length;
    pos_in_tile.y = pos.y - ((int)pos.y/side_length) * side_length;

    const float cos_angle = cos(angle);
    const float sin_angle = sin(angle);
    const float tan_angle = sin_angle/cos_angle;
    const float cot_angle = cos_angle/sin_angle;

    CartesianPair Dv;
    float Dv_x_increment = side_length;

    if (cos_angle < 0) {
        Dv.x = -pos_in_tile.x;
        Dv.y = Dv.x * tan_angle;

        Dv_x_increment *= -1.0;
    
    } else if (cos_angle > 0) {
        Dv.x = side_length - pos_in_tile.x;
        Dv.y = Dv.x * tan_angle;
    }

    const float Dv_y_increment = Dv_x_increment * tan_angle;
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
            Dv.x += Dv_x_increment;
            Dv.y += Dv_y_increment;
        }
    }

    CartesianPair Dh;
    float Dh_y_increment = side_length;

    if (sin_angle < 0) {
        Dh.y = -pos_in_tile.y;
        Dh.x = Dh.y * cot_angle;

        Dh_y_increment *= -1.0;
    } else if (sin_angle > 0) {
        Dh.y = side_length - pos_in_tile.y;
        Dh.x = Dh.y * cot_angle;
    }

    const float Dh_x_increment = Dh_y_increment * cot_angle;
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
            Dh.y += Dh_y_increment;
            Dh.x += Dh_x_increment;
        }
    }

    HitData hit_data;

    if (
        pow(Dv.x, 2) + pow(Dv.y, 2) <= pow(Dh.x, 2) + pow(Dh.y, 2)
    ) {
        hit_data.coords = Dv;
        hit_data.vertical = true;
    } else {
        hit_data.coords = Dh;
        hit_data.vertical = false;
    }

    this->draw_line(
        pos.x, 
        pos.y, 
        pos.x + hit_data.coords.x, 
        pos.y + hit_data.coords.y
    );

    return hit_data;
}

void Renderer::draw_quadri_3d(
    const uint16_t x1,
    const uint16_t line_height1,
    const uint16_t x2,
    const uint16_t line_height2,
    const uint16_t height_on_window
) const {
    
    const float dist = x2 - x1;
    const float line_height_dist = line_height2 - line_height1;

    const uint16_t midpoint = height_on_window/2;

    for (uint16_t x = x1; x < x2; ++x) {
        const uint16_t half_line_height = (line_height1 + (line_height_dist) * float(x - x1)/dist)/2;
        const int16_t upper_point = midpoint - half_line_height;
        this->draw_line(x, (upper_point >= 0 ? upper_point : 0), x, midpoint + half_line_height);
    }

}

void Renderer::draw_3d_floor(
    const uint16_t origin_on_window_x, 
    const uint16_t origin_on_window_y,
    const uint16_t width_on_window,
    const uint16_t height_on_window
) const {    
    this->set_drawing_color(this->config.floor_color);
    
    const uint16_t top_y = origin_on_window_y + height_on_window/2.0;
    const uint16_t bottom_y = origin_on_window_y + height_on_window;
    const uint16_t max_x = origin_on_window_x + width_on_window;

    for (uint16_t i = origin_on_window_x; i < max_x; ++i) {
        this->draw_line(i, top_y, i, bottom_y);
    }
}

void Renderer::draw_3d() const {

    const uint16_t origin_on_window_x = this->config.render_origin_on_window_x;
    const uint16_t origin_on_window_y = this->config.render_origin_on_window_y;
    const uint16_t width_on_window = this->config.render_width_on_window;
    const uint16_t height_on_window = this->config.render_height_on_window;

    this->draw_3d_floor(origin_on_window_x, origin_on_window_y, width_on_window, height_on_window);

    uint16_t last_line_height = 0;
    uint16_t last_window_x = 0;

    const float field_of_view = this->config.field_of_view;
    const float theta_increment = field_of_view/this->config.ray_count;
    const float line_height_scalar = this->config.line_height_scalar;

    for (float theta = -field_of_view/2.0; theta < field_of_view/2.0; theta += theta_increment) {
        
        HitData hit_data = this->cast_ray(theta);
        const float distance = sqrt(pow(hit_data.coords.x, 2) + pow(hit_data.coords.y, 2));

        if (hit_data.vertical) {
            this->set_drawing_color(this->config.vertical_wall_color);
        } else {
            this->set_drawing_color(this->config.horizontal_wall_color);
        }

        const float t = (theta + field_of_view/2)/field_of_view;
        const uint16_t window_x = width_on_window * t + origin_on_window_x;

        const uint16_t line_height = line_height_scalar * height_on_window/distance;

        for (uint16_t window_y = origin_on_window_y; window_y < origin_on_window_y + height_on_window; ++window_y) {
            if (!last_line_height) {
                break;
            }
            this->draw_quadri_3d(last_window_x, last_line_height, window_x, line_height, height_on_window);
        }

        last_line_height = line_height;
        last_window_x = window_x;
    }

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
    this->set_drawing_color(this->config.topdown_player_square_color);

    const uint16_t pos_x = this->player_ptr->get_pos_x();
    const uint16_t pos_y = this->player_ptr->get_pos_y();
    const uint8_t side_length = 2 * this->player_ptr->config.collision_radius;
    
    this->draw_rectangle(
        pos_x - side_length/2.0, 
        pos_y - side_length/2.0, 
        side_length, 
        side_length, 
        FillType::FILLED
    );
}

void Renderer::clear_display() const {
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

    this->set_drawing_color(this->config.ceiling_color);
    this->clear_display();

    this->draw_3d();
    this->draw_debug_topdown_grid();
    this->draw_debug_topdown_player();
    
    this->update_display();
}

Renderer::Renderer(const uint16_t window_width, const uint16_t window_height, std::string window_title) {       

    this->window = SDL_CreateWindow(
        window_title.c_str(),
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