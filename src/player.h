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

    float pos_x;
    float pos_y;
    float rotation;

    float vel_x = 0;
    float vel_y = 0;
    float angular_vel = 0;
    
    const Map* const map_ptr;

    float basis_dx = 0;
    float basis_dy = 0;
    float global_basis_dx = 0;
    float global_basis_dy = 0;

public:
    const PlayerConfig config;

    HitData hit_data;

    void update_key_status();

    void apply_velocity(const float delta);
    void apply_angular_velocity(const float delta);

    float get_pos_x() const;
    float get_pos_y() const;
    float get_rotation() const;

    enum Axis {
        HORIZONTAL,
        VERTICAL
    };

    enum MovementDirection {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum MovementDirection get_movement_direction(enum Axis axis) const;

    const float get_basis_d_relative_rotation() const;
    void input_to_dir();

    void move_and_slide();

    Player(
        const Map* const map_ptr,
        const PlayerConfig config
    );
};

}