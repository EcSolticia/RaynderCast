#include <renderer/renderer.h>

namespace Raynder {

void Renderer::hud_draw_eucliview() {
    
    const float outline_size = 2;
        
    this->draw_quad(
        this->config.eucliview_offset_x - outline_size, this->config.eucliview_offset_y - outline_size,
        this->config.eucliview_offset_x - outline_size, this->config.eucliview_offset_y + this->eucliview_height + outline_size,
        this->config.eucliview_offset_x + this->eucliview_width + outline_size, this->config.eucliview_offset_y + this->eucliview_height + outline_size,
        this->config.eucliview_offset_x + this->eucliview_width + outline_size, this->config.eucliview_offset_y - outline_size,
        Color()
    );
    
    // draw viewport
    this->draw_viewport(Viewport::EUCLI);
}

}