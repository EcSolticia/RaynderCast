#include <renderer.h>
#include <player.h>
#include <map.h>

#include <stdexcept>
#include <cmath>

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

    this->set_drawing_color(0, 0, 255);
    const float ray_length = 100;
    const float rotation = this->player_ptr->get_rotation();
    const int8_t ray_x = ray_length * std::cos(rotation);
    const int8_t ray_y = ray_length * std::sin(rotation);
   
    this->draw_line(
        pos_x, 
        pos_y,
        pos_x + ray_x, 
        pos_y + ray_y
    );
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