#pragma once
#include <raynder/types.h>
#include <optional>

namespace Raynder {

class Player;
class Map;
class Renderer;

namespace Raycaster {

    HitData cast_ray(
        const Player* const player_ptr, 
        const Map* const map_ptr,
        const float relative_angle_to_player
    );

};
    
}