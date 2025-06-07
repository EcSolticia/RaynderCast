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

    IdxPair idx = Raycaster::get_idx(
        CartesianPair{
            this->get_pos_x(),
            this->get_pos_y()
        },
        this->map_ptr->get_side_length(),
        CartesianPair(),
        true
    );

    int8_t dx{0};
    int8_t dy{0};
    if (this->global_basis_dx != 0) {
        dx = (this->global_basis_dx > 0) ? 1 : -1;
    }
    if (this->global_basis_dy != 0) {
        dy = (this->global_basis_dy > 0) ? 1 : -1;
    }

    if (!this->collision_direction.x_colliding && !this->collision_direction.y_colliding) {
        if (this->map_ptr->get_data(idx.x + dx, idx.y + dy)) {
            #ifdef DEBUG_BUILD
            std::cout << "[C] Woah woah woah, hold on!\n";
            std::cout << "[x] " << (int)dx << "\n";
            std::cout << "[y] " << (int)dy << "\n";
            #endif
            collision_direction.x_colliding = true;
            collision_direction.y_colliding = true;
        }
    }
}

}