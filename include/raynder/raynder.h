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
    void configure_renderer(const RendererConfig& config) const;
    void configure_player(const PlayerConfig& config) const;

public:
    void gameloop();

    void set_renderer_distance_func(const RendererDistanceFunc& func) const;

    Game(
        const GameConfig& config,
        const RendererConfig& renderer_config,
        const PlayerConfig& player_config,
        const MapConfig& map_config
    );
    ~Game();
};

}