#pragma once
#include <raynder/export_raynder.h>

#include <stdint.h>
#include <cmath>
#include <functional>

namespace Raynder {

    struct RAYNDERCAST_EXPORT IdxPair {
        uint8_t x;
        uint8_t y;
    };

    struct RAYNDERCAST_EXPORT CartesianPair {
        float x;
        float y;
    };

    struct RAYNDERCAST_EXPORT HitData {
        CartesianPair coords;
        IdxPair hit_idx;
        bool vertical;
    };

    struct RAYNDERCAST_EXPORT Color {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
    };

    struct RAYNDERCAST_EXPORT RendererConfig {
        Color floor_color;
        Color ceiling_color{37, 44, 44};
        
        Color topdown_player_square_color{0, 255, 0};
        Color topdown_ray_color{255, 255, 255};

        Color vertical_wall_color{100, 90, 90};
        Color horizontal_wall_color{100, 100, 100};

        float ray_count{60};

        float field_of_view{M_PI * 1/2};

        float line_height_scalar{30.0};
        
        uint16_t render_origin_on_window_x{320};
        uint16_t render_origin_on_window_y{0};
        uint16_t render_width_on_window{700};
        uint16_t render_height_on_window{400};
    };

    typedef std::function<float(float, float)> RendererDistanceFunc;

    struct RAYNDERCAST_EXPORT PlayerConfig {
        float collision_radius{10.0};
        
        float translational_speed{100.0};
        float rotational_speed{M_PI * 3/4};
    };

    struct RAYNDERCAST_EXPORT GameConfig {
        bool vsync_enabled{false};
    };

}