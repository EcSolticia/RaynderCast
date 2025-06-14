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

struct CollisionDirection {
        bool right;
        bool x_colliding{false};
        bool up;
        bool y_colliding{false};
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

    const float get_basis_d_relative_rotation() const;

    const bool within_collision_distance() const;

public:
    const PlayerConfig config;

    void update_key_status();

    void apply_velocity(const float delta);
    void apply_angular_velocity(const float delta);

    float get_pos_x() const;
    float get_pos_y() const;
    float get_rotation() const;

    void reset_collision_dir();
    CollisionDirection collision_direction;

    void input_to_dir();
    enum MovementDirection get_movement_direction(enum Axis axis) const;

    void detect_collision();
    void move_and_slide();

    Player(
        const Map* const map_ptr,
        const PlayerConfig config
    );
};

}