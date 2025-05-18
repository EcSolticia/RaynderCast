#pragma once

#include <map.h>
#include <renderer.h>
#include <player.h>
#include <types.h>

#include <string>
#include <stdint.h>

#include <memory>

namespace Raynder {

class Game {
    std::unique_ptr<Renderer> renderer_ptr;
    Map map;
    Player player;

    GameConfig config;

    bool running = false;

    uint64_t last_tick;
    uint64_t current_tick;
    float delta;

    void compute_delta();
public:
    void gameloop();

    void configure_renderer(RendererConfig config) const;
    void configure_player(PlayerConfig config);

    void set_renderer_distance_func(RendererDistanceFunc func) const;

    void create_map(
        const uint8_t col_count, 
        const uint8_t row_count,
        const uint8_t side_length,
        const std::string& map_grid_data
    );
    void create_player(
        const float initial_x, 
        const float initial_y, 
        const float initial_rotation
    );

    Game(
        const uint16_t window_width, 
        const uint16_t window_height, 
        const std::string window_title,
        const GameConfig& config
    );
    ~Game();
};

}