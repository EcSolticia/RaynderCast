#include <raynder.h>
#include <iostream>
#include <string>
#include <stdexcept>

int main() {

    Raynder::GameConfig gconfig;
    gconfig.vsync_enabled = true;

    Raynder::Game game(1024, 320, "New Parameter?", gconfig);

    std::string map_data = "1 1 1 1 1 1 1 1 1 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 1 1 1 0 1 1 1\n"
                           "1 0 0 1 0 0 0 0 0 1\n"
                           "1 0 0 1 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 1 1 1 1 1 1 1 1 1";

    try {
        game.create_map(10, 9, 32, map_data);
        game.create_player(72, 72, 0);

        Raynder::RendererConfig rconfig;
        Raynder::PlayerConfig pconfig;

        pconfig.translational_speed = 75.0;
        pconfig.collision_radius = 2.0;
        rconfig.floor_color = Raynder::Color{255, 255, 255};
        rconfig.topdown_ray_color = Raynder::Color{0, 0, 0};

        game.configure_player(pconfig);
        game.configure_renderer(rconfig);

        game.gameloop();
    } catch (const std::exception& e) {
        std::cerr << "Exception raised: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception raised." << std::endl;
    }

    return 0;
}