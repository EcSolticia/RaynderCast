#include <player/player.h>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <math.h>

#include <map.h>

namespace Raynder {

float Player::get_pos_x() const {return this->pos_x;}
float Player::get_pos_y() const {return this->pos_y;}
float Player::get_rotation() const {return this->rotation;}

void Player::update_key_status() {
    const uint8_t* keyboard_state = SDL_GetKeyboardState(NULL);

    KeyStatus new_status;

    new_status.w = (bool)keyboard_state[SDL_SCANCODE_W];
    new_status.s = (bool)keyboard_state[SDL_SCANCODE_S];
    new_status.a = (bool)keyboard_state[SDL_SCANCODE_A];
    new_status.d = (bool)keyboard_state[SDL_SCANCODE_D];
    new_status.q = (bool)keyboard_state[SDL_SCANCODE_Q];
    new_status.e = (bool)keyboard_state[SDL_SCANCODE_E];

    this->key_just_pressed.w = (new_status.w && !this->key_pressed.w);
    this->key_just_pressed.s = (new_status.s && !this->key_pressed.s);
    this->key_just_pressed.a = (new_status.a && !this->key_pressed.a);
    this->key_just_pressed.d = (new_status.d && !this->key_pressed.d);
    this->key_just_pressed.q = (new_status.q && !this->key_pressed.q);
    this->key_just_pressed.e = (new_status.e && !this->key_pressed.e);

    this->key_just_released.w = (!new_status.w && this->key_pressed.w);
    this->key_just_released.s = (!new_status.s && this->key_pressed.s);
    this->key_just_released.a = (!new_status.a && this->key_pressed.a);
    this->key_just_released.d = (!new_status.d && this->key_pressed.d);
    this->key_just_released.q = (!new_status.q && this->key_pressed.q);
    this->key_just_released.e = (!new_status.e && this->key_pressed.e);

    this->key_pressed = new_status;
}


void Player::apply_velocity(const float delta) {
    this->pos_x += vel_x * delta;
    this->pos_y += vel_y * delta;
}

void Player::apply_angular_velocity(const float delta) {
    this->rotation += angular_vel * delta;
}

void Player::reset_collision_dir() {
    this->collision_direction = CollisionDirection();
}

enum MovementDirection Player::get_movement_direction(enum Axis axis) const {
    enum MovementDirection dir;
    switch (axis) {
        case Axis::HORIZONTAL:
            if (!global_basis_dy) break;
            dir = (global_basis_dy > 0) ? MovementDirection::UP : MovementDirection::DOWN;
            break;
        case Axis::VERTICAL:
            if (!global_basis_dx) break;
            dir = (global_basis_dx > 0) ? MovementDirection::RIGHT : MovementDirection::LEFT;
            break;
    }
    return dir;
}

void Player::input_to_dir() {
    this->basis_dx = 0;
    this->basis_dy = 0;

    if (this->key_pressed.w) {
        this->basis_dy += 1;
    }
    if (this->key_pressed.s) {
        this->basis_dy -= 1;
    }
    if (this->key_pressed.a) {
        this->basis_dx += 1;
    }
    if (this->key_pressed.d) {
        this->basis_dx -= 1;
    }

    if ((basis_dx != 0) && (basis_dy != 0)) {
        this->basis_dx /= sqrt(2.0);
        this->basis_dy /= sqrt(2.0);
    }

    const float global_adjusted_rotation = rotation - M_PI/2.0;
    this->global_basis_dx = this->basis_dx * cos(global_adjusted_rotation) - this->basis_dy * sin(global_adjusted_rotation);
    this->global_basis_dy = this->basis_dx * sin(global_adjusted_rotation) + this->basis_dy * cos(global_adjusted_rotation);
}

void Player::move_and_slide() {
    this->vel_x = 0;
    this->vel_y = 0;
    this->angular_vel = 0;

    const float p_vel_x = this->global_basis_dx * this->config.translational_speed;
    const float p_vel_y = this->global_basis_dy * this->config.translational_speed;

    /*const float relative_angle = this->get_basis_d_relative_rotation();

    const float hit_dist_squared = pow(hit_data.coords.x, 2) + pow(hit_data.coords.y, 2);

    if (hit_dist_squared <= pow(this->config.collision_radius, 2)) {
        
        if (hit_data.vertical) {
            this->vel_y = p_vel_y;
        } else {
            this->vel_x = p_vel_x;
        }
    } else {
        this->vel_x = p_vel_x;
        this->vel_y = p_vel_y;
    }*/


    if (collision_direction.y_colliding && collision_direction.x_colliding) {
        
    } else if (collision_direction.y_colliding) {
        this->vel_x = p_vel_x;
    } else if (collision_direction.x_colliding) {
        this->vel_y = p_vel_y;
    } else {
        this->vel_x = p_vel_x;
        this->vel_y = p_vel_y;
    }
    
    if (this->key_pressed.q) {
        this->angular_vel = -this->config.rotational_speed;
    }
    if (this->key_pressed.e) {
        this->angular_vel = this->config.rotational_speed;
    }
}

Player::Player(
    const Map* const map_ptr,
    const PlayerConfig config
) : 
    pos_x{config.initial_x},
    pos_y{config.initial_y},
    rotation{config.initial_rotation},
    map_ptr{map_ptr},
    config{config}
{}

}