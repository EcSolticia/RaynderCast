#include <player/player.h>
#include <raycaster.h>
#include <map.h>

#ifdef DEBUG_BUILD
#include <iostream>
#endif

namespace Raynder {

void Player::detect_collision() {
    enum MovementDirection hdir = this->get_movement_direction(Axis::HORIZONTAL);
    enum MovementDirection vdir = this->get_movement_direction(Axis::VERTICAL);
    
    int8_t x = 0;
    int8_t y = 0;

    this->reset_collision_dir();
    if (Raycaster::cast_ray_along_axis_in_tile(
        this,
        this->map_ptr,
        hdir
    )) {
        if (hdir == MovementDirection::UP) {
            this->collision_direction.up = true;
            this->collision_direction.y_colliding = true;
            ++y;

            #ifdef DEBUG_BUILD
            std::cout << "[C] Hit UP!\n";
            #endif
        } else {
            this->collision_direction.up = false;
            this->collision_direction.y_colliding = true;
            --y;

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
            --x;
            
            #ifdef DEBUG_BUILD
            std::cout << "[C] Hit LEFT!\n";
            #endif
        } else {
            this->collision_direction.right = true;
            this->collision_direction.x_colliding = true;
            ++x;
            
            #ifdef DEBUG_BUILD
            std::cout << "[C] Hit RIGHT!\n";
            #endif
        }
    }

    IdxPair idx = Raycaster::get_idx(
        CartesianPair{
            this->get_pos_x(),
            this->get_pos_y()
        },
        this->map_ptr->get_side_length(),
        CartesianPair(),
        true
    );

    if (!x && !y) {
        if (this->map_ptr->get_data(idx.x + x, idx.y + y)) {
            #ifdef DEBUG_BUILD
            std::cout << "[C] Woah woah woah, hold on!\n";
            #endif
            collision_direction.x_colliding = true;
            collision_direction.y_colliding = true;
        }
    }
}

}