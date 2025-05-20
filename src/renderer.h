#pragma once
#include <stdint.h>
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
    SDL_Window* window;
    SDL_Renderer* context;

    Map* map_ptr = nullptr;
    Player* player_ptr = nullptr;

    enum FillType {
        NOT_FILLED = 0,
        FILLED = 1
    };

    mutable std::vector<SDL_Vertex> quad_buffer;
    std::vector<int> quad_indices = { 0, 1, 2, 0, 2, 3 };

    
    void draw_rectangle(const uint16_t origin_x, 
                        const uint16_t origin_y, 
                        const uint8_t width, 
                        const uint8_t height,
                        FillType filled) const;

    void draw_point(const uint16_t x, const uint16_t y) const;

    void draw_quad(
        const float x1, const float y1,
        const float x2, const float y2,
        const float x3, const float y3,
        const float x4, const float y4,
        const Color color
    ) const;
        
    void draw_quadri_3d(
        const uint16_t x1,
        const uint16_t line_height1,
        const uint16_t x2,
        const uint16_t line_height2,
        const uint16_t height_on_window,
        const bool hit_vertical
    ) const;

    void draw_3d_floor(
        const uint16_t origin_on_window_x, 
        const uint16_t origin_on_window_y,
        const uint16_t width_on_window,
        const uint16_t height_on_window
    ) const;

    void draw_3d() const;

public:
    void set_drawing_color(const uint8_t r, const uint8_t g, const uint8_t b) const;
    void set_drawing_color(const Color color) const;

    void draw_line(
        const uint16_t x1, 
        const uint16_t y1, 
        const int16_t x2, 
        const int16_t y2
    ) const;

    RendererConfig config;

    RendererDistanceFunc distance_func = [](float x, float y) -> float {
        return sqrt(pow(x, 2) + pow(y, 2));
    };

    void set_map_ptr(Map* map_ptr);
    void set_player_ptr(Player* player_ptr);

    void clear_display() const;

    void draw_debug_topdown_player() const;
    void draw_debug_topdown_grid() const;

    void render_loop() const;

    void update_display() const;

    Renderer() {};
    Renderer(
        const uint16_t window_width, 
        const uint16_t window_height, 
        const std::string window_title,
        const bool enable_vsync
    );
    ~Renderer();
};

}