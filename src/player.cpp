#include <player.h>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <math.h>

namespace Raynder {

float Player::get_pos_x() const {return this->pos_x;}
float Player::get_pos_y() const {return this->pos_y;}
float Player::get_rotation() const {return this->rotation;}
float Player::get_collision_radius() const {return this->collision_radius;}
float Player::get_rotation_step() const {return this->rotation_step;}

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

void Player::set_rotation_step(const float rotation_step) {
    this->rotation_step = rotation_step;
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

void Player::handle_keypress() {
    this->vel_x = 0;
    this->vel_y = 0;
    this->angular_vel = 0;
    
    float d = 100;

    float basis_dx = 0;
    float basis_dy = 0;

    if (this->key_pressed.w) {
        basis_dy += 1;
    }
    if (this->key_pressed.s) {
        basis_dy -= 1;
    }
    if (this->key_pressed.a) {
        basis_dx += 1;
    }
    if (this->key_pressed.d) {
        basis_dx -= 1;
    }

    if ((basis_dx != 0) && (basis_dy != 0)) {
        basis_dx /= sqrt(2.0);
        basis_dy /= sqrt(2.0);
    }

    const float global_adjusted_rotation = rotation - M_PI/2.0;
    float global_basis_dx = basis_dx * cos(global_adjusted_rotation) - basis_dy * sin(global_adjusted_rotation);
    float global_basis_dy = basis_dx * sin(global_adjusted_rotation) + basis_dy * cos(global_adjusted_rotation);

    this->vel_x = global_basis_dx * d;
    this->vel_y = global_basis_dy * d;

    if (this->key_pressed.q) {
        this->angular_vel -= this->rotation_step * 20;
    }
    if (this->key_pressed.e) {
        this->angular_vel += this->rotation_step * 20;
    }
}

Player::Player(
    const float pos_x, 
    const float pos_y, 
    const float rotation,
    const float collision_radius
) {

    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->collision_radius = collision_radius;
    this->rotation = rotation;
    
    this->rotation_step = M_PI/16.0;
}

}