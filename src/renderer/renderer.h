#pragma once
#include <stdint.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for Windows
#endif
#include <cmath>

#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include <raynder/types.h>

struct SDL_Window;
struct SDL_Renderer;

namespace Raynder {

class Player;
class Map;

class Renderer {
    const uint16_t window_height;
    const uint16_t window_width;

    SDL_Window* window;
    SDL_Renderer* context;

    const Map* const map_ptr = nullptr;
    const Player* const player_ptr = nullptr;

    std::vector<SDL_Vertex> quad_buffer;
    std::vector<int> quad_indices = { 0, 1, 2, 0, 2, 3 };

    static bool same_or_adjacent_blocks(const IdxPair A, const IdxPair B) {
        const uint8_t dx = abs((int)A.x - (int)B.x);
        const uint8_t dy = abs((int)A.y - (int)B.y);

        return (dx <= 1 && dy <= 1);
    }

    static bool diagonal_blocks(const IdxPair A, const IdxPair B) {
        const uint8_t dx = abs((int)A.x - (int)B.x);
        const uint8_t dy = abs((int)A.y - (int)B.y);

        return (dx == 1 && dy == 1);
    }

    static const float angular_distance_between(CartesianPair v1, CartesianPair v2) {
        const float dotprod = v1.x * v2.x + v1.y * v2.y;
        const float v1_euc_dist = sqrt(pow(v1.x, 2) + pow(v1.y, 2));
        const float v2_euc_dist = sqrt(pow(v2.x, 2) + pow(v2.y, 2));
        return acos(dotprod/(v1_euc_dist * v2_euc_dist));
    }

    void draw_quad(
        const float x1, const float y1,
        const float x2, const float y2,
        const float x3, const float y3,
        const float x4, const float y4,
        const Color color
    );

public:
    void set_drawing_color(const uint8_t r, const uint8_t g, const uint8_t b) const;
    void set_drawing_color(const Color color) const;

    const RendererConfig config;

    RendererDistanceFunc distance_func = [](float x, float y) -> float {
        return sqrt(pow(x, 2) + pow(y, 2));
    };

    void clear_display() const;

    void render_loop();

    void update_display() const;

    Renderer(
        const uint16_t window_width,
        const uint16_t window_height,
        const std::string window_title,
        const Map* const map_ptr,
        const Player* const player_ptr,
        const RendererConfig config
    );
    ~Renderer();
};

}