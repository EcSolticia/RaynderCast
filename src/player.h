#pragma once
#include <raynder/types.h>

union SDL_Event;

namespace Raynder {

class Map;

struct KeyStatus {
    bool w;
    bool s;
    bool a;
    bool d;
    bool q;
    bool e;
};

class Player {
    KeyStatus key_pressed;
    KeyStatus key_just_pressed;
    KeyStatus key_just_released;

    float pos_x = -1;
    float pos_y = -1;
    float rotation = 0;

    float vel_x = 0;
    float vel_y = 0;
    float angular_vel = 0;
    
    Map* map_ptr;

    float basis_dx = 0;
    float basis_dy = 0;
    float global_basis_dx = 0;
    float global_basis_dy = 0;

    void set_pos_x(const float x);
    void set_pos_y(const float y);
    void set_pos(const float x, const float y);
    void set_rotation(const float rotation);

    void rotate(const float amount_in_rad);
    void add_pos(const float dx, const float dy);

public:
    PlayerConfig config;

    HitData hit_data;

    void update_key_status();

    void apply_velocity(const float delta);
    void apply_angular_velocity(const float delta);

    float get_pos_x() const;
    float get_pos_y() const;
    float get_rotation() const;

    const float get_basis_d_relative_rotation() const;
    void input_to_dir();

    void move_and_slide();

    Player() {};
    Player(
        const float pos_x, 
        const float pos_y, 
        const float rotation,
        Map* map_ptr
    );
};

}