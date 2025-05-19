#pragma once

#include <stdint.h>

#include <vector>

#include <stdexcept>

#include <regex>
#include <string>
#include <sstream>

namespace Raynder {

class Map {
    uint8_t row_count;
    uint8_t column_count;
    uint8_t side_length;

    std::vector<int> data;

public:

    const uint8_t get_row_count() const;
    const uint8_t get_column_count() const;
    const uint8_t get_side_length() const;
    
    const int get_data(const uint8_t x, const uint8_t y) const;

    void set_data(const uint8_t x, const uint8_t y, const int value);

    Map();
    Map(
        const uint8_t column_count, 
        const uint8_t row_count,
        const uint8_t side_length,
        const std::string& string_as_data
    );
};

}