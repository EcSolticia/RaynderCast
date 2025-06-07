#include <raynder/raynder.h>
#include <raynder/types.h>
#include <raynder/math_helpers.h>
#include <iostream>
#include <string>
#include <stdexcept>

int main() {

    Raynder::GameConfig gconfig;

    std::string map_data = "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 1\n"
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

    rconfig.vsync_enabled = false;

    Raynder::PlayerConfig pconfig;
    pconfig.initial_x = 72;
    pconfig.initial_y = 72;
    pconfig.initial_rotation = M_PI/4.0;

    pconfig.translational_speed = 125.0;
    pconfig.collision_radius = 2.0;
    rconfig.floor_color = Raynder::Color{78, 102, 136};
    rconfig.ceiling_color = Raynder::Color{51, 45, 86};

    rconfig.horizontal_wall_color_max = Raynder::Color{255, 0, 0};
    rconfig.horizontal_wall_color_min = Raynder::Color{0, 0, 255};
    rconfig.vertical_wall_color_max = Raynder::Color{255, 0, 0};
    rconfig.vertical_wall_color_min = Raynder::Color{0, 0, 255};

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
            
            const float n = 4.0;

            const float length = pow(
                pow(x, n) + pow(y, n),
                1.0/n
            );

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