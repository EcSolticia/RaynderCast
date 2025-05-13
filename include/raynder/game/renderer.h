#pragma once
#include <stdint.h>

struct SDL_Window;
struct SDL_Renderer;

namespace Raynder {

class Player;
class Grid;

class Renderer {
    uint16_t window_width;
    uint16_t window_height;

    SDL_Window* window;
    SDL_Renderer* context;

    Grid* map_ptr = nullptr;
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

public:
    void set_map_ptr(Grid* map_ptr);
    void set_player_ptr(Player* player_ptr);

    void clear_display() const;

    void draw_debug_topdown_grid(const uint8_t col_count, const uint8_t row_count, const uint8_t side_length) const;

    void render_loop() const;

    void update_display() const;

    Renderer() {};
    Renderer(const uint16_t window_width, const uint16_t window_height);
    ~Renderer();
};

}