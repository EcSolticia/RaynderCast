#include <renderer/renderer.h>
#include <player.h>
#include <map.h>
#include <raycaster.h>

#include <stdexcept>

#include <string>
#include <vector>

#include <cmath>
#include <algorithm>

#include <SDL2/SDL.h>

namespace Raynder {

void Renderer::set_drawing_color(const uint8_t r, const uint8_t g, const uint8_t b) {
    if (SDL_SetRenderDrawColor(this->context, r, g, b, 255)) {
            throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::set_drawing_color(const Color color) {
    this->set_drawing_color(color.r, color.g, color.b);
}

void Renderer::draw_rectangle(
    const uint16_t origin_x, 
    const uint16_t origin_y, 
    const uint8_t width, 
    const uint8_t height,
    FillType filled) 
{
    SDL_Rect rect;
    rect.x = origin_x;
    rect.y = origin_y;
    rect.w = width;
    rect.h = height;

    if (!filled) {
        if (SDL_RenderDrawRect(this->context, &rect)) {
                throw std::runtime_error(SDL_GetError());
        }
    } else {
        if (SDL_RenderFillRect(this->context, &rect)) {
            throw std::runtime_error(SDL_GetError());
        }
    }
    
}

void Renderer::draw_line(
    const uint16_t x1, 
    const uint16_t y1, 
    const int16_t x2, 
    const int16_t y2
) {
    if (SDL_RenderDrawLine(this->context, x1, y1, x2, y2)) {
        throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::draw_point(const uint16_t x, const uint16_t y) {
    if (SDL_RenderDrawPoint(this->context, x, y)) {
        throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::draw_quad(
        const float x1, const float y1,
        const float x2, const float y2,
        const float x3, const float y3,
        const float x4, const float y4,
        const Color color
) {
    const SDL_Color draw_color{color.r, color.g, color.b, 255};
    
    this->quad_buffer[0] = SDL_Vertex{SDL_FPoint{x1, y1}, draw_color, SDL_FPoint{0}};
    this->quad_buffer[1] = SDL_Vertex{SDL_FPoint{x2, y2}, draw_color, SDL_FPoint{0}};
    this->quad_buffer[2] = SDL_Vertex{SDL_FPoint{x3, y3}, draw_color, SDL_FPoint{0}};
    this->quad_buffer[3] = SDL_Vertex{SDL_FPoint{x4, y4}, draw_color, SDL_FPoint{0}};
    
    if (SDL_RenderGeometry(this->context, nullptr, this->quad_buffer.data(), 4, this->quad_indices.data(), 6)) {
        throw std::runtime_error("SDL_RenderGeometry is not supported.");
    }
}

void Renderer::draw_quadri_3d(
    const uint16_t x1,
    const uint16_t line_height1,
    const uint16_t x2,
    const uint16_t line_height2,
    const bool hit_vertical
) {
    const float midpoint = this->window_height/2;
    
    const float half_line_height1 = line_height1/2;
    const float half_line_height2 = line_height2/2;

    float top_point1 = midpoint - half_line_height1;
    float top_point2 = midpoint - half_line_height2;
    if (top_point1 < 0) {
        top_point1 = 0;
    }
    if (top_point2 < 0) {
        top_point2 = 0;
    }

    const Color col = hit_vertical ? (this->config.vertical_wall_color) : (this->config.horizontal_wall_color);

    this->draw_quad(
        (float)x1, top_point1,
        (float)x2, top_point2,
        (float)x2, midpoint + half_line_height2,
        (float)x1, midpoint + half_line_height1,
        col
    );
}

void Renderer::draw_quadri_3d_from_angles(
    const float angle1, 
    const CartesianPair hit_coords1,
    const float angle2,
    const CartesianPair hit_coords2,
    const bool vertical
) {
    
    const float t1 = (angle1 + this->config.field_of_view/2)/this->config.field_of_view;
    const uint16_t x1 = this->window_width * t1;

    const float distance1 = this->distance_func(hit_coords1.x, hit_coords1.y);

    const uint16_t line_height1 = this->config.line_height_scalar * this->window_height/distance1;

    const float t2 = (angle2 + this->config.field_of_view/2)/this->config.field_of_view;
    const uint16_t x2 = this->window_width * t2;

    const float distance2 = this->distance_func(hit_coords2.x, hit_coords2.y);

    const uint16_t line_height2 = this->config.line_height_scalar * this->window_height/distance2;

    this->draw_quadri_3d(
        x1, 
        line_height1, 
        x2, 
        line_height2, 
        vertical
    );
}

void Renderer::draw_3d_floor() {        
    const float top_y = this->window_height/2.0;
    const float bottom_y = this->window_height;
    const float max_x = this->window_width;

    const Color col = this->config.floor_color;

    this->draw_quad(
        0, top_y,
        max_x, top_y,
        max_x, bottom_y,
        0, bottom_y,
        col 
    );
}

void Renderer::draw_3d() {

    this->draw_3d_floor();

    float last_theta;
    HitData last_hit_data;
    bool first_theta_iteration = true;

    const float field_of_view = this->config.field_of_view;
    const float theta_increment = field_of_view/this->config.ray_count;

    size_t i = 0;

    for (float theta = -field_of_view/2.0; theta < field_of_view/2.0 + theta_increment; theta += theta_increment) {
        
        HitData hit_data = Raycaster::cast_ray(
            this->player_ptr, 
            this->map_ptr,
            theta
        );

        this->hud_minimap_ray_buffer[i].hit_data = hit_data;
        this->hud_minimap_ray_buffer[i].relative_theta = theta;
        ++i;

        const bool same_or_adjacent_blocks = Renderer::same_or_adjacent_blocks(last_hit_data.hit_idx, hit_data.hit_idx);

        bool convex_outline_from_view;

        if (!first_theta_iteration && same_or_adjacent_blocks) {

            convex_outline_from_view = !diagonal_blocks(last_hit_data.hit_idx, hit_data.hit_idx);
            const bool diagonal = !convex_outline_from_view;

            const bool same_orientation = hit_data.vertical == last_hit_data.vertical;

            if (same_orientation && !diagonal) {
                this->draw_quadri_3d_from_angles(
                    last_theta,
                    last_hit_data.coords,
                    theta,
                    hit_data.coords,
                    hit_data.vertical
                );
            }
        }

        last_hit_data = hit_data;
        last_theta = theta;
        first_theta_iteration = false;
    }

    this->hud_draw_minimap_base();
    this->hud_draw_minimap_ray_from_buffer();

}

void Renderer::set_map_ptr(Map* map_ptr) {
    this->map_ptr = map_ptr;
}

void Renderer::set_player_ptr(Player* player_ptr) {
    this->player_ptr = player_ptr;
}

void Renderer::clear_display() {
    if (SDL_RenderClear(this->context)) {
        throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::update_display() {
    SDL_RenderPresent(this->context);
}

void Renderer::render_loop() {
    if (!this->map_ptr || !this->player_ptr) {
        throw std::runtime_error("Rendering cannot proceed without valid Map and Player objects.");
    }

    this->set_drawing_color(this->config.ceiling_color);
    this->clear_display();

    this->draw_3d();
    
    this->update_display();
}

Renderer::Renderer(
    const uint16_t window_width, 
    const uint16_t window_height, 
    std::string window_title,
    const bool enable_vsync
) : window_width{window_width}, window_height{window_height} {       

    this->window = SDL_CreateWindow(
        window_title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        0
    );
    if (this->window == NULL) {
        throw std::runtime_error(SDL_GetError());
    }

    this->context = SDL_CreateRenderer(
        this->window, 
        -1, 
        SDL_RENDERER_ACCELERATED | (int(enable_vsync) * SDL_RENDERER_PRESENTVSYNC)
    );
    if (this->context == NULL) {
        throw std::runtime_error(SDL_GetError());
    }

    this->quad_buffer.resize(4);
}

Renderer::~Renderer() {
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->context);
}

}