#include <game/game.h>

#include <SDL2/SDL.h>

namespace Raynder {

void Game::gameloop(const uint32_t delay) {
    running = true;
    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
        
            if (event.type == SDL_QUIT) {
                running = false;
            } else {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        
        }

        this->renderer.render_loop();

        SDL_Delay(delay);

    }
}

void Game::create_player(const float initial_x, const float initial_y, const float initial_rotation) {
    if (initial_x < 0 || initial_y < 0) {
        throw std::runtime_error("Position values must be non-negative");
    }

    this->player = Player(initial_x, initial_y, initial_rotation);
    this->renderer.set_player_ptr(&this->player);
}

void Game::create_map(const uint8_t col_count, const uint8_t row_count, const std::string& map_grid_data) {       
    if (!row_count || !col_count) {
        throw std::runtime_error("Grid dimensions cannot be zero.");
    }
    
    this->map = Grid(col_count, row_count, map_grid_data);
    this->renderer.set_map_ptr(&this->map);
}

Game::Game(const uint16_t window_width, const uint16_t window_height) {
    if (!window_width || !window_height) {
        throw std::runtime_error("Window resolution cannot be 0x0.");
    }

    this->renderer = Renderer(window_width, window_height);

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        throw std::runtime_error(SDL_GetError());
    }
};

Game::~Game() {
    SDL_Quit();
}

}
