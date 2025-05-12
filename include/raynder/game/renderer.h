#pragma once
#include <string> //

namespace Raynder {

class Renderer {
    uint16_t window_width;
    uint16_t window_height;

public:

    Renderer() {};
    Renderer(const uint16_t window_width, const uint16_t window_height);
};

}