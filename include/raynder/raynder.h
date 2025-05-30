#pragma once

#include <raynder/export_raynder.h>

#include <raynder/types.h>

#include <string>
#include <stdint.h>

#include <memory>

namespace Raynder {

class Renderer;
class Map;
class Player;

class RAYNDERCAST_EXPORT Game {
    std::unique_ptr<Renderer> renderer_ptr;
    std::unique_ptr<Map> map_ptr;
    std::unique_ptr<Player> player_ptr;

    uint64_t last_tick;
    uint64_t current_tick;
    float delta;

    void compute_delta();
public:
    void gameloop();

    void configure_renderer(const RendererConfig& config) const;
    void configure_player(const PlayerConfig& config) const;

    void set_renderer_distance_func(const RendererDistanceFunc& func) const;

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
        const uint16_t eucliview_height,
        const uint16_t eucliview_width,
        const std::string window_title,
        const GameConfig& config
    );
    ~Game();
};

}