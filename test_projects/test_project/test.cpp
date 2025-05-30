#include <raynder/raynder.h>
#include <raynder/types.h>
#include <iostream>
#include <string>
#include <stdexcept>

int main() {

    Raynder::GameConfig gconfig;
    gconfig.vsync_enabled = false;

    std::string map_data = "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 1 0 0 0 0 0 1 1 1 1\n"
                           "1 0 0 0 0 0 1 0 0 0 0 1 0 0 0 1\n"
                           "1 0 0 0 0 0 1 0 0 0 1 0 0 0 0 1\n"
                           "1 0 0 0 0 0 1 0 0 1 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 1 1 1 1 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\n";

    Raynder::RendererConfig rconfig;

    Raynder::PlayerConfig pconfig;
    pconfig.initial_x = 72;
    pconfig.initial_y = 72;

    pconfig.translational_speed = 125.0;
    pconfig.collision_radius = 20.0;
    rconfig.floor_color = Raynder::Color{78, 102, 136};
    rconfig.ceiling_color = Raynder::Color{51, 45, 86};

    rconfig.horizontal_wall_color = Raynder::Color{227, 238, 178};
    rconfig.vertical_wall_color = Raynder::Color{255, 255, 255};

    rconfig.ray_count = 1024;

    rconfig.eucliview_offset_x = 8;
    rconfig.eucliview_offset_y = 8;

    Raynder::MapConfig mconfig{16, 16, 32, map_data};

    try {

        Raynder::Game game(
            gconfig,
            rconfig,
            pconfig,
            mconfig
        );

        game.set_renderer_distance_func([](float x, float y) -> float {
            const float length = sqrt(pow(x, 2) + pow(y, 2));
            return length;
        });

        game.gameloop();
    } catch (const std::exception& e) {
        std::cerr << "Exception raised: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception raised." << std::endl;
    }

    return 0;
}