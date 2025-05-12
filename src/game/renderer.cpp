#include <game/renderer.h>
#include <stdexcept>

#include <SDL2/SDL.h>

namespace Raynder {

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

    SDL_SetRenderDrawColor(this->context, 255, 0, 0, 255);
    SDL_RenderClear(this->context);
    SDL_RenderPresent(this->context);
    SDL_Delay(1000);
}

}