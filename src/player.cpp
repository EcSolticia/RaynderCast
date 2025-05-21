#include <player.h>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <math.h>

#include <raycaster.h>
#include <map.h>

#include <iostream>

namespace Raynder {

float Player::get_pos_x() const {return this->pos_x;}
float Player::get_pos_y() const {return this->pos_y;}
float Player::get_rotation() const {return this->rotation;}

void Player::set_pos_x(const float x) {this->pos_x = x;}
void Player::set_pos_y(const float y) {this->pos_y = y;}
void Player::set_pos(const float x, const float y) {
    this->pos_x = x;
    this->pos_y = y;
}
void Player::set_rotation(const float rotation) {this->rotation = rotation;}

void Player::rotate(const float amount_in_rad) {
    this->rotation += amount_in_rad;
}

void Player::add_pos(const float dx, const float dy) {
    this->pos_x += dx;
    this->pos_y += dy;
}

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

const float Player::get_basis_d_relative_rotation() const {
    if (basis_dy > 0 && basis_dx == 0) { //forward
        return 0;
    } else if (basis_dy < 0 && basis_dx == 0) { //backward
        return M_PI;
    } else if (basis_dy == 0 && basis_dx > 0) { //left
        return -M_PI/2.0;
    } else if (basis_dy == 0 && basis_dx < 0) { //right
        return M_PI/2.0;
    } else if (basis_dx > 0 && basis_dy > 0) {
        return -M_PI/4.0;
    } else if (basis_dx < 0 && basis_dy > 0) {
        return M_PI/4.0;
    } else if (basis_dx < 0 && basis_dy < 0) {
        return 2.0/3.0 * M_PI;
    } else if (basis_dx > 0 && basis_dy < 0) {
        return -2.0/3.0 * M_PI;
    }
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

    const float relative_angle = this->get_basis_d_relative_rotation();

    const float hit_dist = sqrt(pow(hit_data.coords.x, 2) + pow(hit_data.coords.y, 2));

    std::cout << hit_dist << "\n";

    if (hit_dist <= this->config.collision_radius) {
        
        if (hit_data.vertical) {
            this->vel_y = p_vel_y;
        } else {
            this->vel_x = p_vel_x;
        }
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
    const float pos_x, 
    const float pos_y, 
    const float rotation,
    Map* map_ptr
) {
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->rotation = rotation;
    this->map_ptr = map_ptr;
}

}