#include <game.h>

#include <SDL2/SDL.h>

namespace Raynder {

void Game::gameloop(const uint32_t delay) {
    running = true;
    while (running) {

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
        
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                this->player.handle_keypress();
            }
        
        }

        this->renderer_ptr.get()->render_loop();

        SDL_Delay(delay);

    }
}

void Game::create_player(
    const float initial_x, 
    const float initial_y, 
    const float initial_rotation,
    const float collision_radius
) {
    if (initial_x < 0 || initial_y < 0) {
        throw std::runtime_error("Position values must be non-negative");
    }

    this->player = Player(initial_x, initial_y, initial_rotation, collision_radius);
    this->renderer_ptr.get()->set_player_ptr(&this->player);
}

void Game::create_map(
    const uint8_t col_count, 
    const uint8_t row_count,
    const uint8_t side_length, 
    const std::string& map_grid_data
) {       
    if (!row_count || !col_count) {
        throw std::runtime_error("Grid dimensions cannot be zero.");
    }
    
    this->map = Map(col_count, row_count, side_length, map_grid_data);
    this->renderer_ptr.get()->set_map_ptr(&this->map);
}

Game::Game(const uint16_t window_width, const uint16_t window_height) {
    if (!window_width || !window_height) {
        throw std::runtime_error("Window resolution cannot be 0x0.");
    }

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        throw std::runtime_error(SDL_GetError());
    }
    
    this->renderer_ptr = std::make_unique<Renderer>(window_width, window_height);
};

Game::~Game() {
    SDL_Quit();
}

}
