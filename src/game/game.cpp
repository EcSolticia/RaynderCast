#include <game/game.h>

namespace Raynder {

void Game::create_renderer(const uint16_t window_width, const uint16_t window_height) {
    if (!window_width || !window_height) {
        throw std::runtime_error("Window resolution cannot be 0x0.");
    }
    this->renderer = Renderer(window_width, window_height);
}

void Game::create_player(const float initial_x, const float initial_y, const float initial_rotation) {
    this->player = Player(initial_x, initial_y, initial_rotation);
}

void Game::create_map(const uint8_t col_count, const uint8_t row_count, const std::string& map_grid_data) {       
    if (!row_count || !col_count) {
        throw std::runtime_error("Grid dimensions cannot be zero.");
    }
    
    this->map = Grid(col_count, row_count, map_grid_data);
}

}
