#pragma once

#include <stdint.h>

#include <vector>

#include <stdexcept>

#include <regex>
#include <string>
#include <sstream>

namespace Raynder {

class Map {
    uint8_t row_count = 0;
    uint8_t column_count = 0;
    uint8_t side_length = 0;

    std::vector<int> data;

public:

    const std::uint8_t get_row_count() const {return this->row_count;}
    const std::uint8_t get_column_count() const {return this->column_count;}
    const std::uint8_t get_side_length() const {return this->side_length;}
    
    const int get_data(const std::uint8_t x, const std::uint8_t y) const {
        if (x >= column_count || y >= row_count) {
            throw std::out_of_range("Selected Map coordinate does not exist.");
        }
        return this->data[y * column_count + x];
    }

    void set_data(const std::uint8_t x, const std::uint8_t y, const int value) {
        if (x >= column_count || y >= row_count) {
            throw std::out_of_range("Selected Map coordinate does not exist.");
        }
        this->data[y * column_count + x] = value;
    }

    Map() {};
    Map(
        const uint8_t column_count, 
        const uint8_t row_count,
        const uint8_t side_length,
        const std::string& string_as_data
    ) {

        this->row_count = row_count;
        this->column_count = column_count;
        this->data.resize(row_count * column_count);
            
        this->side_length = side_length;

        std::string row_regex_str = "(^\\s*(\\d+\\s+){" + std::to_string(column_count - 1) + "}\\d+\\s*$)";
        std::regex row_regex(row_regex_str);

        std::stringstream ss(string_as_data);
        std::string line;

        std::uint8_t loc_y = 0;

        while (std::getline(ss, line)) {
            
            if (loc_y >= row_count) {
                throw std::runtime_error("Too many rows.");
            }

            if (!std::regex_match(line, row_regex)) {
                throw std::runtime_error("Invalid row format.");
            }

            std::istringstream iss(line);
            int val;
            std::uint8_t loc_x = 0;

            while (iss >> val) {
                if (loc_x >= column_count) {
                    throw std::runtime_error("Too many columns.");
                }

                this->set_data(loc_x, loc_y, val);

                ++loc_x;
            }

            ++loc_y;

        }

        if (loc_y != row_count) {
            throw std::runtime_error("Too few rows provided.");
        }
    }
};

}