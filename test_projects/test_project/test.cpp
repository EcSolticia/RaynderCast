#include <raynder/raynder.h>
#include <raynder/types.h>
#include <iostream>
#include <string>
#include <stdexcept>

int main() {

    Raynder::GameConfig gconfig;
    gconfig.vsync_enabled = false;

    Raynder::Game game(1024, 320, "New Parameter?", gconfig);

    std::string map_data = "1 1 1 1 1 1 1 1 1 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 1 0 0 0 0 0 1\n"
                           "1 0 0 0 1 1 0 1 1 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 1 0 0 0 1\n"
                           "1 0 0 0 1 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n";

    try {
        game.create_map(10, 13, 32, map_data);
        game.create_player(72, 72, 0);

        Raynder::RendererConfig rconfig;
        Raynder::PlayerConfig pconfig;

        pconfig.translational_speed = 75.0;
        pconfig.collision_radius = 10.0;
        rconfig.floor_color = Raynder::Color{133, 133, 255};
        rconfig.topdown_ray_color = Raynder::Color{0, 0, 0};

        rconfig.horizontal_wall_color = Raynder::Color{255, 255, 255};
        rconfig.vertical_wall_color = Raynder::Color{0, 0, 0};

        rconfig.ray_count = 1024;
        rconfig.field_of_view = M_PI/3.0;

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