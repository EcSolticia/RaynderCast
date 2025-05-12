#include <game/player.h>

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

Player::Player(const float pos_x, const float pos_y, const float rotation) {
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->rotation = rotation;
}

}