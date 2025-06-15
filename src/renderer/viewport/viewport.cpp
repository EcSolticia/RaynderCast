#include <renderer/viewport/viewport.h>

namespace Raynder {

Viewport::Viewport(
    const uint16_t origin_x,
    const uint16_t origin_y,
    const uint16_t width,
    const uint16_t height,

    const ViewportConfig config,

    Renderer* renderer_ptr
) :
    origin_x{origin_x},
    origin_y{origin_y},
    width{width},
    height{height},

    ray_count{config.ray_count},
    field_of_view{config.field_of_view},
    distance_function{config.distance_function},

    renderer_ptr{renderer_ptr}
{};

};