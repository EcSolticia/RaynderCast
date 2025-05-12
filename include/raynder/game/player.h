#pragma once

namespace Raynder {

class Player {
    float pos_x = -1;
    float pos_y = -1;
    float rotation = 0;

public:
    float get_pos_x() const;
    float get_pos_y() const;
    float get_rotation() const;

    void set_pos_x(const float x);
    void set_pos_y(const float y);
    void set_pos(const float x, const float y);
    void set_rotation(const float rotation);

    Player() {};
    Player(const float pos_x, const float pos_y, const float rotation);
};

}