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
    const uint16_t window_height;
    const uint16_t window_width;

    const uint16_t eucliview_height;
    const uint16_t eucliview_width;

    SDL_Window* window;
    SDL_Renderer* context;

    Map* map_ptr = nullptr; // readonly
    Player* player_ptr = nullptr; // readonly

    enum FillType {
        NOT_FILLED = 0,
        FILLED = 1
    };

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
      
    enum Viewport {
        MAIN,
        EUCLI
    };

    void draw_quadri_3d(
        const uint16_t x1,
        const uint16_t line_height1,
        const uint16_t x2,
        const uint16_t line_height2,
        const uint16_t midpoint,
        const bool hit_vertical
    );

    void draw_quadri_3d_from_angles(
        const float angle1, 
        const float distance1,
        const float angle2,
        const float distance2,
        const bool vertical,
        const uint16_t width,
        const uint16_t height,
        const uint16_t origin_x,
        const uint16_t origin_y
    );

    void draw_eucliview_ceiling();

    void draw_3d_floor(
        enum Viewport viewport
    );

    void draw_3d_wall(
        enum Viewport viewport,
        const uint16_t width,
        const uint16_t height,
        const uint16_t origin_x,
        const uint16_t origin_y,
        const float theta_increment
    );

    void draw_viewport(enum Viewport viewport);

    void hud_draw_eucliview();

public:
    void set_drawing_color(const uint8_t r, const uint8_t g, const uint8_t b) const;
    void set_drawing_color(const Color color) const;

    RendererConfig config;

    RendererDistanceFunc distance_func = [](float x, float y) -> float {
        return sqrt(pow(x, 2) + pow(y, 2));
    };

    const float get_renderer_distance(
        const float x,
        const float y,
        enum Viewport viewport
    ) const;

    void set_map_ptr(Map* map_ptr);
    void set_player_ptr(Player* player_ptr);

    void clear_display() const;

    void render_loop();

    void update_display() const;

    Renderer() : window_width{0}, window_height{0}, eucliview_height{0}, eucliview_width{0} {};
    Renderer(
        const uint16_t window_width,
        const uint16_t window_height,
        const uint16_t eucliview_height,
        const uint16_t eucliview_width,
        const std::string window_title,
        const bool enable_vsync
    );
    ~Renderer();
};

}