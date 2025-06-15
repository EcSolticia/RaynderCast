#include <renderer/renderer.h>
#include <player/player.h>
#include <map.h>
#include <raycaster.h>

#include <raynder/types.h>
#include <raynder/math_helpers.h>

#include <stdexcept>

#include <string>
#include <vector>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for Windows
#endif
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

void Renderer::draw_quad(
    const float x1, const float y1,
    const float x2, const float y2,
    const float x3, const float y3,
    const float x4, const float y4,
    const Color color
) {
    const SDL_Color draw_color{color.r, color.g, color.b, 255};
    
    this->quad_buffer[0] = SDL_Vertex{SDL_FPoint{x1, y1}, draw_color, SDL_FPoint{0}};
    this->quad_buffer[1] = SDL_Vertex{SDL_FPoint{x2, y2}, draw_color, SDL_FPoint{0}};
    this->quad_buffer[2] = SDL_Vertex{SDL_FPoint{x3, y3}, draw_color, SDL_FPoint{0}};
    this->quad_buffer[3] = SDL_Vertex{SDL_FPoint{x4, y4}, draw_color, SDL_FPoint{0}};
    
    if (SDL_RenderGeometry(this->context, nullptr, this->quad_buffer.data(), 4, this->quad_indices.data(), 6)) {
        throw std::runtime_error("SDL_RenderGeometry is not supported.");
    }
}

void Renderer::clear_display() const {
    if (SDL_RenderClear(this->context)) {
        throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::update_display() const {
    SDL_RenderPresent(this->context);
}

void Renderer::render_loop() {
    if (!this->map_ptr || !this->player_ptr) {
        throw std::runtime_error("Rendering cannot proceed without valid Map and Player objects.");
    }

    this->set_drawing_color(this->config.ceiling_color);
    this->clear_display();

    

    this->update_display();
}

Renderer::Renderer(
    
    const uint16_t window_width, 
    const uint16_t window_height,
    std::string window_title,
    const Map* const map_ptr,
    const Player* const player_ptr,
    const RendererConfig config

) : window_width{window_width}, 
    window_height{window_height}, 
    map_ptr{map_ptr},
    player_ptr{player_ptr},
    config{config}
{

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
        this->window, 
        -1, 
        SDL_RENDERER_ACCELERATED | (int(config.vsync_enabled) * SDL_RENDERER_PRESENTVSYNC)
    );
    if (this->context == NULL) {
        throw std::runtime_error(SDL_GetError());
    }

    this->quad_buffer.resize(4);
}

Renderer::~Renderer() {
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->context);
}

}