#pragma once

namespace Raynder {

class Player {
    float pos_x;
    float pos_y;
    float rotation;

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