#include <raycaster.h>
#include <player.h>
#include <map.h>
#include <renderer/renderer.h>

namespace Raynder {

const IdxPair Raycaster::get_idx(
    const CartesianPair pos,
    const uint8_t side_length,
    const CartesianPair diff,
    const bool vertical
) {
    IdxPair idx;
    
    if (!vertical) {
        idx.y = (int)((pos.y + diff.y)/side_length) - int(diff.y < 0);
        idx.x = (int)((pos.x + diff.x)/side_length);
    } else {
        idx.y = (int)((pos.y + diff.y)/side_length);
        idx.x = (int)((pos.x + diff.x)/side_length) - int(diff.x < 0);
    }

    return idx;
}

const CartesianPair Raycaster::get_pos_in_tile(
    const CartesianPair pos,
    const uint8_t side_length
) {

    // by constructor design, pos can't be negative and no checks are done in this regard.
    return CartesianPair {
        pos.x - ((int)pos.x/side_length) * side_length,
        pos.y - ((int)pos.y/side_length) * side_length
    };

}

const bool Raycaster::cast_ray_along_axis_in_tile(
    const Player* const player_ptr,
    const Map* const map_ptr,
    enum MovementDirection dir) {

    const CartesianPair pos{
        player_ptr->get_pos_x(),
        player_ptr->get_pos_y()
    };

    const uint8_t side_length{
        map_ptr->get_side_length()
    };

    const CartesianPair pos_in_tile{
        Raycaster::get_pos_in_tile(pos, side_length)
    };

    CartesianPair diff;
    bool vertical;

    switch (dir) {
        case MovementDirection::UP:
            diff.y = side_length - pos_in_tile.y;
            vertical = false;
            break;
        case MovementDirection::DOWN:
            diff.y = -pos_in_tile.y;
            vertical = false;
            break;
        case MovementDirection::LEFT:
            diff.x = -pos_in_tile.x;
            vertical = true;
            break;
        case MovementDirection::RIGHT:
            diff.x = side_length - pos_in_tile.x;
            vertical = true;
            break;
    }

    const IdxPair idx{
        Raycaster::get_idx(pos, side_length, diff, vertical)
    };

    return (bool)map_ptr->get_data(idx.x, idx.y);
}

HitData Raycaster::cast_ray(
    const Player* const player_ptr, 
    const Map* const map_ptr,
    const float relative_angle_to_player
) {

    const float angle = player_ptr->get_rotation() + relative_angle_to_player;
    
    CartesianPair pos{
        pos.x = player_ptr->get_pos_x(),
        pos.y = player_ptr->get_pos_y()
    };

    const uint8_t side_length = map_ptr->get_side_length();

    CartesianPair pos_in_tile = Raycaster::get_pos_in_tile(
        pos,
        side_length
    );

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
    IdxPair idxv;

    while (!hit_wall_v) {
        idxv = Raycaster::get_idx(pos, side_length, Dv, true);

        if (idxv.y >= map_ptr->get_row_count() || idxv.x >= map_ptr->get_column_count()) {
            hit_wall_v = true;
            break;
        }

        if (map_ptr->get_data(idxv.x, idxv.y)) {
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
    IdxPair idxh;

    while (!hit_wall_h) {
        idxh = Raycaster::get_idx(pos, side_length, Dh, false);

        if (idxh.y >= map_ptr->get_row_count() || idxh.x >= map_ptr->get_column_count()) {
            hit_wall_h = true;
            break;
        }

        if (map_ptr->get_data(idxh.x, idxh.y)) {
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
        hit_data.hit_idx = idxv;
    } else {
        hit_data.coords = Dh;
        hit_data.vertical = false;
        hit_data.hit_idx = idxh;
    }

    return hit_data;
}

}