#pragma once
#include <stdexcept>

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

}

}