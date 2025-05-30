#include <raynder/raynder.h>

#include <renderer/renderer.h>
#include <map.h>
#include <player.h>
#include <raycaster.h>

#include <SDL2/SDL.h>

#include <string>

#ifdef DEBUG_BUILD
#include <iostream>
#endif

namespace Raynder {

void Game::compute_delta() {
    this->current_tick = SDL_GetTicks();

    this->delta = (this->current_tick - this->last_tick) * 0.001;

    this->last_tick = this->current_tick;
}

void Game::gameloop() {
    bool running = true;
    while (running) {

        this->compute_delta();
        #ifdef DEBUG_BUILD
            std::cout << "[Game] delta: " << delta << "\n";
        #endif

        this->player_ptr->apply_velocity(this->delta);
        this->player_ptr->apply_angular_velocity(this->delta);

        this->player_ptr->update_key_status();
        this->player_ptr->input_to_dir();
        
        this->player_ptr->move_and_slide();

        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        
        }

        this->renderer_ptr->render_loop();

        this->player_ptr->hit_data = Raycaster::cast_ray(
            this->player_ptr.get(),
            this->map_ptr.get(),
            this->player_ptr->get_basis_d_relative_rotation()
        );

        this->renderer_ptr->update_display();
    }
}

void Game::configure_renderer(const RendererConfig& config) const {
    this->renderer_ptr->config = config;
}

void Game::configure_player(const PlayerConfig& config) const {
    this->player_ptr->config = config;
}

void Game::set_renderer_distance_func(const RendererDistanceFunc& func) const {
    this->renderer_ptr->distance_func = func;
}

void Game::create_player(
    const float initial_x, 
    const float initial_y, 
    const float initial_rotation
) {
    if (!this->map_ptr) {
        throw std::runtime_error("Cannot create Player object prior to the Map object.");
    }

    if (initial_x < 0 || initial_y < 0) {
        throw std::runtime_error("Position values must be non-negative");
    }

    this->player_ptr = std::make_unique<Player> (
        Player(initial_x, initial_y, initial_rotation, this->map_ptr.get())
    );
    this->renderer_ptr->set_player_ptr(this->player_ptr.get());
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
    
    this->map_ptr = std::make_unique<Map>(
        Map(col_count, row_count, side_length, map_grid_data)
    );
    this->renderer_ptr->set_map_ptr(this->map_ptr.get());
}

Game::Game(
    const uint16_t window_width, 
    const uint16_t window_height, 
    const uint16_t eucliview_height,
    const uint16_t eucliview_width,
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
        eucliview_height,
        eucliview_width, 
        window_title,
        config.vsync_enabled
    );
};

Game::~Game() {
    SDL_Quit();
}

}
