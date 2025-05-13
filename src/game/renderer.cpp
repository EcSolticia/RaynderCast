#include <game/renderer.h>
#include <game/player.h>
#include <grid.h>
#include <stdexcept>

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

void Renderer::set_map_ptr(Grid* map_ptr) {
    this->map_ptr = map_ptr;
}

void Renderer::set_player_ptr(Player* player_ptr) {
    this->player_ptr = player_ptr;
}

void Renderer::draw_debug_topdown_grid(const uint8_t col_count, const uint8_t row_count, const uint8_t side_length) const {

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

    const uint8_t row_count = this->map_ptr->get_row_count();
    const uint8_t col_count = this->map_ptr->get_column_count();


    this->clear_display();
    
    this->draw_debug_topdown_grid(col_count, row_count, 32);

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