#include <raynder.h>
#include <iostream>
#include <string>
#include <stdexcept>

int main() {

    Raynder::Game game(1024, 320, "New Parameter?");

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
        game.create_player(72, 72, 0, 8);

        Raynder::RendererConfig rconfig;

        rconfig.floor_color = Raynder::Color{255, 255, 255};

        game.configure_renderer(rconfig);


        game.gameloop(16);
    } catch (const std::exception& e) {
        std::cerr << "Exception raised: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception raised." << std::endl;
    }

    return 0;
}