#include <raynder.h>
#include <iostream>

int main() {

    Raynder::Game game;

    try {
        game.create_map(3, 3, "1 1 1\n 1 0 1\n 1 1 1");
        game.create_player(1, 1, 0);
        game.create_renderer(640, 480);
    } catch (const std::exception& e) {
        std::cerr << "Exception raised: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception raised." << std::endl;
    }

    return 0;
}