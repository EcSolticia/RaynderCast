#pragma once
#include <raynder/export_raynder.h>

#include <stdint.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for Windows
#endif
#include <cmath>

#include <functional>
#include <string>

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
        uint8_t r;
        uint8_t g;
        uint8_t b;

        Color() : r{0}, g{0}, b{0} {};
        Color(const uint8_t r, const uint8_t g, const uint8_t b) : r{r}, g{g}, b{b} {};
    };

    struct SignedColor {
        int16_t r = 0;
        int16_t g = 0;
        int16_t b = 0;

        SignedColor();
        SignedColor(const int16_t r, const int16_t g, const int16_t b) : r{r}, g{g}, b{b} {}  
        SignedColor(const Color color) : 
            r{(int16_t)color.r}, 
            b{(int16_t)color.b}, 
            g{(int16_t)color.g} 
        {}
    };

    struct RAYNDERCAST_EXPORT RendererConfig {
        Color floor_color;
        Color ceiling_color{37, 44, 44};
        
        Color vertical_wall_color{100, 90, 90};
        Color horizontal_wall_color{100, 100, 100};

        uint16_t ray_count{1024};
        uint16_t eucliview_ray_count{128};

        uint16_t eucliview_offset_x{8};
        uint16_t eucliview_offset_y{8};

        float field_of_view{M_PI * 1/2};

        bool vsync_enabled{false};
    };

    typedef std::function<float(float, float)> RendererDistanceFunc;

    struct RAYNDERCAST_EXPORT PlayerConfig {
        float collision_radius{10.0};
        
        float translational_speed{100.0};
        float rotational_speed{M_PI * 3/4};

        float initial_x{-10};
        float initial_y{-10};
        float initial_rotation{0};
    };

    struct RAYNDERCAST_EXPORT MapConfig {
        uint8_t col_count{0};
        uint8_t row_count{0};
        uint8_t side_length{0};
        std::string& map_grid_data;
    };

    struct RAYNDERCAST_EXPORT GameConfig {
        uint16_t window_width{640};
        uint16_t window_height{480};
        uint16_t eucliview_height{16 * 4};
        uint16_t eucliview_width{16 * 6};
        std::string window_title{"RaynderCast Default Title"};
    };

}