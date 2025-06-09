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

    uint64_t last_tick{0};
    uint64_t current_tick{0};
    float delta{0};

    void compute_delta();

    void validate_map_config(const MapConfig& config) const;
    void validate_renderer_config(const RendererConfig& config) const;
    void validate_player_config(const PlayerConfig& config) const;

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