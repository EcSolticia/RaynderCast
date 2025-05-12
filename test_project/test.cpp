#include <raynder.h>
#include <iostream>
#include <string>

int main() {

    Raynder::Game game(640, 480);

    std::string map_data = "1 1 1 1 1 1 1 1 1 1\n"
                           "1 0 0 0 1 0 0 0 0 1\n"
                           "1 0 0 0 1 0 0 0 0 1\n"
                           "1 0 0 0 1 0 0 0 0 1\n"
                           "1 0 0 0 1 0 0 0 0 1\n"
                           "1 0 0 0 0 0 0 0 0 1\n"
                           "1 0 0 0 0 1 1 0 0 1\n"
                           "1 0 0 0 0 1 0 0 0 1\n"
                           "1 1 1 1 1 1 1 1 1 1";

    try {
        game.create_map(10, 9, map_data);
        game.create_player(1, 1, 0);

        game.gameloop(16);
    } catch (const std::exception& e) {
        std::cerr << "Exception raised: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception raised." << std::endl;
    }

    return 0;
}