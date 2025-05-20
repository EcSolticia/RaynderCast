#include <raycaster.h>
#include <player.h>
#include <map.h>
#include <renderer.h>

namespace Raynder {

HitData Raycaster::cast_ray(
    Player* player_ptr, 
    Map* map_ptr,
    #ifdef DEBUG_BUILD
    const Renderer* renderer_ptr,
    #endif
    const float relative_angle_to_player
) {

    const float angle = player_ptr->get_rotation() + relative_angle_to_player;
    
    CartesianPair pos;
    pos.x = player_ptr->get_pos_x();
    pos.y = player_ptr->get_pos_y();

    const uint8_t side_length = map_ptr->get_side_length();

    // by constructor design, pos can't be negative and no checks are done in this regard.
    CartesianPair pos_in_tile;
    pos_in_tile.x = pos.x - ((int)pos.x/side_length) * side_length;
    pos_in_tile.y = pos.y - ((int)pos.y/side_length) * side_length;

    const float cos_angle = cos(angle);
    const float sin_angle = sin(angle);
    const float tan_angle = sin_angle/cos_angle;
    const float cot_angle = cos_angle/sin_angle;

    CartesianPair Dv;
    float Dv_x_increment = side_length;

    if (cos_angle < 0) {
        Dv.x = -pos_in_tile.x;
        Dv.y = Dv.x * tan_angle;

        Dv_x_increment *= -1.0;
    
    } else if (cos_angle > 0) {
        Dv.x = side_length - pos_in_tile.x;
        Dv.y = Dv.x * tan_angle;
    }

    const float Dv_y_increment = Dv_x_increment * tan_angle;
    bool hit_wall_v = false;

    while (!hit_wall_v) {
        const uint8_t index_y = (int)((pos.y + Dv.y)/side_length);
        const uint8_t index_x = (int)((pos.x + Dv.x)/side_length) - int(Dv.x < 0);

        if (index_y >= map_ptr->get_row_count() || index_x >= map_ptr->get_column_count()) {
            hit_wall_v = true;
            break;
        }

        if (map_ptr->get_data(index_x, index_y)) {
            hit_wall_v = true;
        } else {
            Dv.x += Dv_x_increment;
            Dv.y += Dv_y_increment;
        }
    }

    CartesianPair Dh;
    float Dh_y_increment = side_length;

    if (sin_angle < 0) {
        Dh.y = -pos_in_tile.y;
        Dh.x = Dh.y * cot_angle;

        Dh_y_increment *= -1.0;
    } else if (sin_angle > 0) {
        Dh.y = side_length - pos_in_tile.y;
        Dh.x = Dh.y * cot_angle;
    }

    const float Dh_x_increment = Dh_y_increment * cot_angle;
    bool hit_wall_h = false;

    while (!hit_wall_h) {
        const uint8_t index_y = (int)((pos.y + Dh.y)/side_length) - int(Dv.y < 0);
        const uint8_t index_x = (int)((pos.x + Dh.x)/side_length);

        if (index_y >= map_ptr->get_row_count() || index_x >= map_ptr->get_column_count()) {
            hit_wall_h = true;
            break;
        }

        if (map_ptr->get_data(index_x, index_y)) {
            hit_wall_h = true;
        } else {
            Dh.y += Dh_y_increment;
            Dh.x += Dh_x_increment;
        }
    }

    HitData hit_data;

    if (
        pow(Dv.x, 2) + pow(Dv.y, 2) <= pow(Dh.x, 2) + pow(Dh.y, 2)
    ) {
        hit_data.coords = Dv;
        hit_data.vertical = true;
    } else {
        hit_data.coords = Dh;
        hit_data.vertical = false;
    }

    #ifdef DEBUG_BUILD
    renderer_ptr->set_drawing_color(renderer_ptr->config.topdown_ray_color);
    renderer_ptr->draw_line(
        pos.x, 
        pos.y, 
        pos.x + hit_data.coords.x, 
        pos.y + hit_data.coords.y
    );
    #endif

    return hit_data;
}

}