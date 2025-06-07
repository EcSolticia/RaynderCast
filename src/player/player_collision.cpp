#include <player/player.h>
#include <raycaster.h>

#ifdef DEBUG_BUILD
#include <iostream>
#endif

namespace Raynder {

void Player::detect_collision() {
    enum MovementDirection hdir = this->get_movement_direction(Axis::HORIZONTAL);
    enum MovementDirection vdir = this->get_movement_direction(Axis::VERTICAL);
    
    this->reset_collision_dir();
    if (Raycaster::cast_ray_along_axis_in_tile(
        this,
        this->map_ptr,
        hdir
    )) {
        if (hdir == MovementDirection::UP) {
            this->collision_direction.up = true;
            this->collision_direction.y_colliding = true;
            #ifdef DEBUG_BUILD
            std::cout << "[C] Hit UP!\n";
            #endif
        } else {
            this->collision_direction.up = false;
            this->collision_direction.y_colliding = true;
            #ifdef DEBUG_BUILD
            std::cout << "[C] Hit DOWN!\n";
            #endif
        }
    }
    if (Raycaster::cast_ray_along_axis_in_tile(
        this,
        this->map_ptr,
        vdir
    )) {
        if (vdir == MovementDirection::LEFT) {
            this->collision_direction.right = false;
            this->collision_direction.x_colliding = true;
            #ifdef DEBUG_BUILD
            std::cout << "[C] Hit LEFT!\n";
            #endif
        } else {
            this->collision_direction.right = true;
            this->collision_direction.x_colliding = true;
            #ifdef DEBUG_BUILD
            std::cout << "[C] Hit RIGHT!\n";
            #endif
        }
    }
}

}