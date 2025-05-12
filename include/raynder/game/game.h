#pragma once

#include <grid.h>
#include <game/renderer.h>
#include <game/player.h>

#include <string>
#include <stdint.h>

namespace Raynder {

class Game {
    Renderer renderer;
    Grid map;
    Player player;

public:
    void create_renderer(const uint16_t window_width, const uint16_t window_height);
    void create_map(const uint8_t col_count, const uint8_t row_count, const std::string& map_grid_data);
    void create_player(const float initial_x, const float initial_y, const float initial_rotation);

    Game();
    ~Game();
};

}