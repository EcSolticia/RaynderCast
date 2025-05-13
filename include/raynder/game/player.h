#pragma once

union SDL_Event;

namespace Raynder {

class Player {
    float pos_x = -1;
    float pos_y = -1;
    float rotation = 0;

    void set_pos_x(const float x);
    void set_pos_y(const float y);
    void set_pos(const float x, const float y);
    void set_rotation(const float rotation);

    void add_pos(const float dx, const float dy);

public:
    float get_pos_x() const;
    float get_pos_y() const;
    float get_rotation() const;

    void handle_keypress(SDL_Event event);

    Player() {};
    Player(const float pos_x, const float pos_y, const float rotation);
};

}