#include <stdint.h>
#include <raynder/types.h>

namespace Raynder {

class Renderer;

class Viewport {
    const uint16_t origin_x;
    const uint16_t origin_y;
    const uint16_t width;
    const uint16_t height;

    const uint16_t ray_count;
    const float field_of_view;

    const RendererDistanceFunc& distance_function;

    Renderer* const renderer_ptr;

public:

    Viewport::Viewport(
        const uint16_t origin_x,
        const uint16_t origin_y,
        const uint16_t width,
        const uint16_t height,

        const ViewportConfig config,

        Renderer* renderer_ptr
    ); 
};

}