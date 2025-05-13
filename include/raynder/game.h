#pragma once

#include <map.h>
#include <renderer.h>
#include <player.h>

#include <string>
#include <stdint.h>

#include <memory>

namespace Raynder {

class Game {
    std::unique_ptr<Renderer> renderer_ptr;
    Map map;
    Player player;

    bool running = false;

public:
    void gameloop(const uint32_t delay);

    void create_map(
        const uint8_t col_count, 
        const uint8_t row_count,
        const uint8_t side_length,
        const std::string& map_grid_data
    );
    void create_player(const float initial_x, const float initial_y, const float initial_rotation);

    Game(const uint16_t window_width, const uint16_t window_height);
    ~Game();
};

}