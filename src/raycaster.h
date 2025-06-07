#pragma once
#include <raynder/types.h>
#include <optional>

namespace Raynder {

class Player;
class Map;
class Renderer;

namespace Raycaster {

    const CartesianPair get_pos_in_tile(
        const CartesianPair pos,
        const uint8_t side_length
    );

    const float cast_ray_along_axis(
        const Player* const player_ptr,
        const Map* const map_tr,
        enum MovementDirection dir
    );

    HitData cast_ray(
        const Player* const player_ptr, 
        const Map* const map_ptr,
        const float relative_angle_to_player
    );

};
    
}