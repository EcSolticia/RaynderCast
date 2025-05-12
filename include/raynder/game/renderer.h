#pragma once
#include <string>

struct SDL_Window;
struct SDL_Renderer;

namespace Raynder {

class Renderer {
    uint16_t window_width;
    uint16_t window_height;

    SDL_Window* window;
    SDL_Renderer* context;

public:

    Renderer() {};
    Renderer(const uint16_t window_width, const uint16_t window_height);
};

}