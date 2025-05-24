#include <raynder/raynder.h>
#include <raynder/types.h>
#include <iostream>
#include <string>
#include <stdexcept>

int main() {

    Raynder::GameConfig gconfig;
    gconfig.vsync_enabled = false;

    Raynder::Game game(1280, 720, "New Parameter?", gconfig);

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

        pconfig.translational_speed = 125.0;
        pconfig.collision_radius = 20.0;
        rconfig.floor_color = Raynder::Color{78, 102, 136};
        rconfig.ceiling_color = Raynder::Color{51, 45, 86};

        rconfig.horizontal_wall_color = Raynder::Color{227, 238, 178};
        rconfig.vertical_wall_color = Raynder::Color{255, 255, 255};

        rconfig.ray_count = 1024;

        game.set_renderer_distance_func([](float x, float y) -> float {
            const float length = sqrt(pow(x, 2) + pow(y, 2));
            return length + sin(length) * length/16.0;
        });

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