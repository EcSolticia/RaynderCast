#pragma once
#include <raynder/types.h>
#include <optional>

namespace Raynder {

class Player;
class Map;
class Renderer;

namespace Raycaster {

    HitData cast_ray(
        Player* player_ptr, 
        Map* map_ptr,
        std::optional<const Renderer*> renderer_ptr,
        const float relative_angle_to_player
    );

};
    
}