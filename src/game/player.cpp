#include <game/player.h>
#include <stdexcept>
#include <SDL2/SDL.h>

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

void Player::add_pos(const float dx, const float dy) {
    this->pos_x += dx;
    this->pos_y += dy;
}

void Player::handle_keypress(SDL_Event event) {
    SDL_Keycode keycode = event.key.keysym.sym;

    float d = 8;

    const uint8_t* keyboard_state = SDL_GetKeyboardState(NULL);

    if (keyboard_state[SDL_SCANCODE_W] || keyboard_state[SDL_SCANCODE_UP]) {
        add_pos(0, -d);
    }
    if (keyboard_state[SDL_SCANCODE_S] || keyboard_state[SDL_SCANCODE_DOWN]) {
        add_pos(0, d);
    }
    if (keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDL_SCANCODE_LEFT]) {
        add_pos(-d, 0);
    }
    if (keyboard_state[SDL_SCANCODE_D] || keyboard_state[SDL_SCANCODE_RIGHT]) {
        add_pos(d, 0);
    }
}

Player::Player(const float pos_x, const float pos_y, const float rotation) {

    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->rotation = rotation;
}

}