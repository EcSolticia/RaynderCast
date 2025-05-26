#include <map.h>
#include <player.h>
#include <renderer/renderer.h>

#include <iostream>

namespace Raynder {

void Renderer::hud_draw_minimap_base() {
    const uint16_t side_length = 16;
    const uint16_t height = side_length * 12;
    const uint16_t width = side_length * 16;
    const uint16_t origin_x = 8;
    const uint16_t origin_y = 8;

    this->set_drawing_color(0, 0, 0);

    this->draw_quad(
        origin_x, origin_y,
        origin_x, origin_y + height,
        origin_x + width, origin_y + height,
        origin_x + width, origin_y,
        this->config.hud_minimap_bg_color
    );
}

void Renderer::hud_draw_minimap_ray(const HitData& hit_data, const float theta) {
    const uint8_t map_side_length = this->map_ptr->get_side_length();
    const uint16_t side_length = 16;
    const uint16_t origin_x = 8;
    const uint16_t origin_y = 8;
    const uint16_t height = side_length * 12;
    const uint16_t width = side_length * 16;

    const int ox = width/2;
    const int oy = height * 9/10;

    //const uint16_t posx = this->player_ptr->get_pos_x()/map_side_length * side_length + origin_x;
    //const uint16_t posy = this->player_ptr->get_pos_y()/map_side_length * side_length + origin_y;

    const int x = hit_data.coords.x/(map_side_length) * side_length;
    const int y = hit_data.coords.y/(map_side_length) * side_length;
    float length = sqrt(pow(x, 2) + pow(y, 2));

    const float h = 3.0 * height/4.0;
    if (length >= h) {
        length = h;
    }

    this->draw_line(
        ox,
        oy,
        ox + length * sin(theta),
        oy + -length * cos(theta)
    );
}

void Renderer::hud_draw_minimap_ray_from_buffer() {
    this->set_drawing_color(255, 255, 255);
    for (RayData ray: hud_minimap_ray_buffer) {
        this->hud_draw_minimap_ray(ray.hit_data, ray.relative_theta);
    }
}

}