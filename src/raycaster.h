#pragma once
#include <raynder/types.h>

namespace Raynder {

class Player;
class Map;
class Renderer;

namespace Raycaster {

    HitData cast_ray(
        Player* player_ptr, 
        Map* map_ptr,
        #ifdef DEBUG_BUILD
        const Renderer* renderer_ptr,
        #endif
        const float relative_angle_to_player
    );

};
    
}