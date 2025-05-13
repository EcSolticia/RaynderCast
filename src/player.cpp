#include <player.h>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <math.h>

namespace Raynder {

float Player::get_pos_x() const {return this->pos_x;}
float Player::get_pos_y() const {return this->pos_y;}
float Player::get_rotation() const {return this->rotation;}
float Player::get_collision_radius() const {return this->collision_radius;}

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

void Player::handle_keypress() {
    float d = 8;

    const uint8_t* keyboard_state = SDL_GetKeyboardState(NULL);

    float basis_dx = 0;
    float basis_dy = 0;

    if (keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_UP]) {
        basis_dy += 1;
    }
    if (keyboard_state[SDL_SCANCODE_S] || keyboard_state[SDL_SCANCODE_DOWN]) {
        basis_dy -= 1;
    }
    if (keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDL_SCANCODE_LEFT]) {
        basis_dx += 1;   
    }
    if (keyboard_state[SDL_SCANCODE_D] || keyboard_state[SDL_SCANCODE_RIGHT]) {
        basis_dx -= 1;   
    }

    if ((basis_dx != 0) && (basis_dy != 0)) {
        basis_dx /= sqrt(2.0);
        basis_dy /= sqrt(2.0);
    }

    const float global_adjusted_rotation = rotation - M_PI/2.0;
    float global_basis_dx = basis_dx * cos(global_adjusted_rotation) - basis_dy * sin(global_adjusted_rotation);
    float global_basis_dy = basis_dx * sin(global_adjusted_rotation) + basis_dy * cos(global_adjusted_rotation);

    this->add_pos(global_basis_dx * d, global_basis_dy * d);

    if (keyboard_state[SDL_SCANCODE_Q] || keyboard_state[SDL_SCANCODE_4]) {
        rotate(-M_PI/16.0);
    }
    if (keyboard_state[SDL_SCANCODE_E] || keyboard_state[SDL_SCANCODE_6]) {
        rotate(M_PI/16.0);
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
}

}