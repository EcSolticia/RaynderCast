#include <game.h>

#include <SDL2/SDL.h>

#include <string>

#include <iostream>

namespace Raynder {

void Game::compute_delta() {
    this->current_tick = SDL_GetTicks();

    this->delta = (this->current_tick - this->last_tick) * 0.001;

    this->last_tick = this->current_tick;
}

void Game::gameloop() {
    running = true;
    while (running) {

        this->compute_delta();
        std::cout << delta << "\n";
        
        this->player.apply_velocity(this->delta);
        this->player.apply_angular_velocity(this->delta);

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
                    
            this->player.update_key_status();
            this->player.handle_keypress();

            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        
        }

        this->renderer_ptr.get()->render_loop();
    }
}

void Game::configure_renderer(const RendererConfig& config) const {
    this->renderer_ptr.get()->config = config;
}

void Game::configure_player(const PlayerConfig& config) {
    this->player.config = config;
}

void Game::set_renderer_distance_func(RendererDistanceFunc func) const {
    this->renderer_ptr.get()->distance_func = func;
}

void Game::create_player(
    const float initial_x, 
    const float initial_y, 
    const float initial_rotation
) {
    if (initial_x < 0 || initial_y < 0) {
        throw std::runtime_error("Position values must be non-negative");
    }

    this->player = Player(initial_x, initial_y, initial_rotation);
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

Game::Game(
    const uint16_t window_width, 
    const uint16_t window_height, 
    const std::string window_title,
    const GameConfig& config
) {
    if (!window_width || !window_height) {
        throw std::runtime_error("Window resolution cannot be 0x0.");
    }

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        throw std::runtime_error(SDL_GetError());
    }
    
    this->renderer_ptr = std::make_unique<Renderer>(
        window_width, 
        window_height, 
        window_title,
        config.vsync_enabled
    );
};

Game::~Game() {
    SDL_Quit();
}

}
