#pragma once
#include <stdint.h>
#include <cmath>

namespace Raynder {

    struct CartesianPair {
        float x;
        float y;
    };

    struct HitData {
        CartesianPair coords;
        bool vertical;
    };

    struct Color {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
    };

    struct RendererConfig {
        Color floor_color;
        Color ceiling_color{37, 44, 44};
        Color topdown_player_square_color{0, 255, 0};
        
        Color vertical_wall_color{100, 90, 90};
        Color horizontal_wall_color{100, 100, 100};

        float ray_count{60};

        float field_of_view{M_PI * 1/2};

        float line_height_scalar{30.0};
        
        //std::string window_title;
        //uint16_t window_width;
        //uint16_t window_height;
    };

}