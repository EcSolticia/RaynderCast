#pragma once
#include <stdexcept>
#include <raynder/types.h>

namespace Raynder {

namespace Math {
    
    inline float pos_divide(const float numerator, const float denominator) {
        if (denominator <= 0) {
            throw std::domain_error("Cannot divide by zero or less in pos_divide.");
        }

        return numerator/denominator;
    }

    inline float divide(const float numerator, const float denominator) {
        if (!denominator) {
            throw std::domain_error("Cannot divide by zero.");
        }

        return numerator/denominator;
    }

    inline SignedColor subtract_color(const SignedColor A, const SignedColor B) {
        return SignedColor{
            (int16_t)(A.r - B.r),
            (int16_t)(B.g - B.g),
            (int16_t)(A.b - B.b)
        };
    }

    inline SignedColor add_color(const SignedColor A, const SignedColor B) {
        return SignedColor{
            (int16_t)(A.r + B.r),
            (int16_t)(A.g + B.g),
            (int16_t)(A.b + B.b)
        };
    }

    inline SignedColor multiply_color(const SignedColor A, const float c) {
        return SignedColor{
            (int16_t)(A.r * c),
            (int16_t)(A.g * c),
            (int16_t)(A.b * c)
        };
    }

    inline Color get_usable_color(const SignedColor A) {
        return Color{
            (uint8_t)A.r,
            (uint8_t)A.g,
            (uint8_t)A.b
        };
    }

}

}