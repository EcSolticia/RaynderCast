#include <game/renderer.h>
#include <stdexcept>

namespace Raynder {

Renderer::Renderer(const uint16_t window_width, const uint16_t window_height) {       
    this->window_width = window_width;
    this->window_height = window_height;
}

}