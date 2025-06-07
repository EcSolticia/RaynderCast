#include <raynder/raynder.h>

#include <renderer/renderer.h>
#include <map.h>
#include <player/player.h>
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
        
        this->player_ptr->detect_collision();
        this->player_ptr->move_and_slide();

        SDL_Event event;

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        
        }

        this->renderer_ptr->render_loop();

        this->renderer_ptr->update_display();
    }
}

void Game::set_renderer_distance_func(const RendererDistanceFunc& func) const {
    this->renderer_ptr->distance_func = func;
}

void Game::validate_map_config(const MapConfig& map_config) const {
    if (!map_config.row_count || !map_config.col_count) {
        throw std::runtime_error("Grid dimensions cannot be zero.");
    }
 
    if (!map_config.side_length) {
        throw std::runtime_error("Grid square side-length cannot be zero.");
    }

    if (map_config.map_grid_data.empty()) {
        throw std::runtime_error("Grid data cannot be empty.");
    }
}

void Game::validate_renderer_config(const RendererConfig& config) const {
    if (!config.ray_count) {
        throw std::runtime_error("Number of rays must be greater than zero.");
    }

    if (!config.eucliview_ray_count) {
        throw std::runtime_error("Number of eucliview rays must be greater than zero.");
    }

    if (config.field_of_view <= 0) {
        throw std::runtime_error("Field of view must be greater than zero.");
    }
}

void Game::validate_player_config(const PlayerConfig& config) const {
    if (config.initial_x < 0 || config.initial_y < 0) {
        throw std::runtime_error("Cannot create Player outside the map.");
    }

    if (config.collision_radius <= 0) {
        throw std::runtime_error("Cannot create Player with non-positive collision radius.");
    }
}

Game::Game(
    const GameConfig& config,
    const RendererConfig& renderer_config,
    const PlayerConfig& player_config,
    const MapConfig& map_config
) {

    if (!config.window_width || !config.window_height) {
        throw std::runtime_error("Window resolution cannot be 0x0.");
    }

    if (!config.eucliview_width || !config.eucliview_height) {
        throw std::runtime_error("Eucliview resolution cannot be 0x0.");
    }

    this->validate_map_config(map_config);
    this->validate_player_config(player_config);
    this->validate_renderer_config(renderer_config);

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        throw std::runtime_error(SDL_GetError());
    }

    this->map_ptr = std::make_unique<Map>(
        map_config.col_count,
        map_config.row_count,
        map_config.side_length,
        map_config.map_grid_data
    );
    
    this->player_ptr = std::make_unique<Player>(
        this->map_ptr.get(),
        player_config
    );
    
    this->renderer_ptr = std::make_unique<Renderer>(
        config.window_width, 
        config.window_height,
        config.eucliview_height,
        config.eucliview_width, 
        config.window_title,
        this->map_ptr.get(),
        this->player_ptr.get(),
        renderer_config
    );
};

Game::~Game() {
    SDL_Quit();
}

}
