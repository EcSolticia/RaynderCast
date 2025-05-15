#pragma once
#include <stdint.h>

struct SDL_Window;
struct SDL_Renderer;

namespace Raynder {

class Player;
class Map;

struct CartesianPair {
    float x;
    float y;
};

struct HitData {
    CartesianPair coords;
    bool vertical;
};

class Renderer {
    uint16_t window_width;
    uint16_t window_height;

    SDL_Window* window;
    SDL_Renderer* context;

    Map* map_ptr = nullptr;
    Player* player_ptr = nullptr;

    enum FillType {
        NOT_FILLED = 0,
        FILLED = 1
    };

    void set_drawing_color(const uint8_t r, const uint8_t g, const uint8_t b) const;
    void draw_rectangle(const uint16_t origin_x, 
                        const uint16_t origin_y, 
                        const uint8_t width, 
                        const uint8_t height,
                        FillType filled) const;
    void draw_line(
        const uint16_t x1, 
        const uint16_t y1, 
        const int16_t x2, 
        const int16_t y2) const;

    HitData cast_ray(const float relative_angle_to_player) const;

    void draw_quadri_3d(
        const uint16_t x1,
        const uint16_t line_height1,
        const uint16_t x2,
        const uint16_t line_height2,
        const uint16_t height_on_window
    ) const;

    void draw_3d(
        const uint16_t origin_on_window_x, 
        const uint16_t origin_on_window_y,
        const uint16_t width_on_window,
        const uint16_t height_on_window,
        const float field_of_view
    ) const;

public:
    void set_map_ptr(Map* map_ptr);
    void set_player_ptr(Player* player_ptr);

    void clear_display() const;

    void draw_debug_topdown_player() const;
    void draw_debug_topdown_grid() const;

    void render_loop() const;

    void update_display() const;

    Renderer() {};
    Renderer(const uint16_t window_width, const uint16_t window_height);
    ~Renderer();
};

}