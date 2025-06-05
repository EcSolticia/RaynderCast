#include <renderer/renderer.h>
#include <player.h>
#include <map.h>
#include <raycaster.h>

#include <raynder/types.h>
#include <raynder/math_helpers.h>

#include <stdexcept>

#include <string>
#include <vector>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES // for Windows
#endif
#include <cmath>

#include <algorithm>

#include <SDL2/SDL.h>

namespace Raynder {

void Renderer::set_drawing_color(const uint8_t r, const uint8_t g, const uint8_t b) const {
    if (SDL_SetRenderDrawColor(this->context, r, g, b, 255)) {
            throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::set_drawing_color(const Color color) const {
    this->set_drawing_color(color.r, color.g, color.b);
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

const Color Renderer::get_line_color(
    const float avg_line_height,
    const bool hit_vertical
) const {
    const float h = this->viewport_indicator == Viewport::MAIN ? this->window_height : this->eucliview_height;
    const float t = avg_line_height/h;

    const SignedColor max = hit_vertical ? 
        this->config.vertical_wall_color_max : 
        this->config.horizontal_wall_color_max;
    const SignedColor min = hit_vertical ?
        this->config.vertical_wall_color_min :
        this->config.horizontal_wall_color_min;

    const SignedColor diff = Math::subtract_color(max, min);

    const SignedColor scol = Math::add_color(
        min, 
        Math::multiply_color(diff, t)
    );

    return Math::get_usable_color(scol);
}

void Renderer::draw_quadri_3d(
    const uint16_t x1,
    const uint16_t line_height1,
    const uint16_t x2,
    const uint16_t line_height2,
    const uint16_t midpoint,
    const bool hit_vertical
) {
    
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

    //const Color col = hit_vertical ? (this->config.vertical_wall_color) : (this->config.horizontal_wall_color);
    const Color col = this->get_line_color((line_height1 + line_height2)/2.0, hit_vertical);

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
    const float distance1,
    const float angle2,
    const float distance2,
    const bool vertical,
    const uint16_t width,
    const uint16_t height,
    const uint16_t offset_x,
    const uint16_t offset_y
) {

    const float t1 = (angle1 + this->config.field_of_view/2)/this->config.field_of_view;
    const uint16_t x1 = width * t1 + offset_x;

    const uint16_t line_height1 = std::clamp((float)30.0 * Math::pos_divide(height, distance1), (float)0.0, (float)height);

    const float t2 = (angle2 + this->config.field_of_view/2)/this->config.field_of_view;
    const uint16_t x2 = width * t2 + offset_x;

    const uint16_t line_height2 = std::clamp((float)30.0 * Math::pos_divide(height, distance2), (float)0.0, (float)height);

    const uint16_t midpoint = height/2.0 + offset_y;

    this->draw_quadri_3d(
        x1, 
        line_height1, 
        x2, 
        line_height2, 
        midpoint,
        vertical
    );
}

void Renderer::draw_eucliview_ceiling() {

    const float max_y = this->eucliview_height/2.0;
    const float max_x = this->eucliview_width;
    const float offx = this->config.eucliview_offset_x;
    const float offy = this->config.eucliview_offset_y;

    this->draw_quad(
        offx, offy,
        offx + max_x, offy,
        offx + max_x, offy + max_y,
        offx, offy + max_y,
        this->config.ceiling_color
    );

}

void Renderer::draw_3d_floor(
    enum Viewport viewport
) {        
    
    float top_y;
    float bottom_y;
    float max_x;
    float min_x = this->config.eucliview_offset_x;

    switch (viewport) {
        case Viewport::MAIN:
            top_y = this->window_height/2.0;
            bottom_y = this->window_height;
            max_x = this->window_width;
            min_x = 0;
            break;
        case Viewport::EUCLI:
            top_y = this->eucliview_height/2.0 + this->config.eucliview_offset_y;
            bottom_y = this->eucliview_height + this->config.eucliview_offset_y;
            max_x = this->eucliview_width + min_x;
            break; 
    }
    
    const Color col = this->config.floor_color;

    this->draw_quad(
        min_x, top_y,
        max_x, top_y,
        max_x, bottom_y,
        min_x, bottom_y,
        col 
    );
}

void Renderer::draw_3d_wall(
    enum Viewport viewport,
    const uint16_t width,
    const uint16_t height,
    const uint16_t origin_x,
    const uint16_t origin_y,
    const float theta_increment
) {
    this->viewport_indicator = viewport;

    float last_theta;
    HitData last_hit_data;
    bool first_theta_iteration = true;

    const float field_of_view = this->config.field_of_view;

    for (float theta = -field_of_view/2.0; theta < field_of_view/2.0 + theta_increment; theta += theta_increment) {
        
        HitData hit_data = Raycaster::cast_ray(
            this->player_ptr, 
            this->map_ptr,
            theta
        );

        const bool same_or_adjacent_blocks = Renderer::same_or_adjacent_blocks(last_hit_data.hit_idx, hit_data.hit_idx);

        bool convex_outline_from_view;

        if (!first_theta_iteration && same_or_adjacent_blocks) {

            convex_outline_from_view = !diagonal_blocks(last_hit_data.hit_idx, hit_data.hit_idx);
            const bool diagonal = !convex_outline_from_view;

            const bool same_orientation = hit_data.vertical == last_hit_data.vertical;

            if (same_orientation && !diagonal) {
                this->draw_quadri_3d_from_angles(
                    last_theta,
                    this->get_renderer_distance(last_hit_data.coords.x, last_hit_data.coords.y, viewport),
                    theta,
                    this->get_renderer_distance(hit_data.coords.x, hit_data.coords.y, viewport),
                    hit_data.vertical,
                    width,
                    height,
                    origin_x,
                    origin_y
                );
            }
        }

        last_hit_data = hit_data;
        last_theta = theta;
        first_theta_iteration = false;
    }

}

const float Renderer::get_renderer_distance(
    const float x, 
    const float y, 
    enum Viewport viewport
) const {
    float result;
    switch (viewport) {
        case Viewport::MAIN:
            result = this->distance_func(x, y);
            break;
        case Viewport::EUCLI:
            result = sqrt(pow(x, 2) + pow(y, 2));
            break;
    }
    return result;
}

void Renderer::clear_display() const {
    if (SDL_RenderClear(this->context)) {
        throw std::runtime_error(SDL_GetError());
    }
}

void Renderer::update_display() const {
    SDL_RenderPresent(this->context);
}

void Renderer::draw_viewport(enum Viewport viewport) {

    const uint16_t w = (viewport == Viewport::MAIN) ? this->window_width : this->eucliview_width;
    const uint16_t h = (viewport == Viewport::MAIN) ? this->window_height : this->eucliview_height;
    const uint16_t ox = (viewport == Viewport::MAIN) ? 0 : this->config.eucliview_offset_x;
    const uint16_t oy = (viewport == Viewport::MAIN) ? 0 : this->config.eucliview_offset_y;

    if (viewport == Viewport::EUCLI) {
        this->draw_eucliview_ceiling();
    }
    this->draw_3d_floor(viewport);

    const float theta_increment = this->config.field_of_view/((viewport == Viewport::MAIN) ? this->config.ray_count : this->config.eucliview_ray_count);

    this->draw_3d_wall(
        viewport,
        w,
        h,
        ox,
        oy,
        theta_increment
    );

}

void Renderer::render_loop() {
    if (!this->map_ptr || !this->player_ptr) {
        throw std::runtime_error("Rendering cannot proceed without valid Map and Player objects.");
    }

    this->set_drawing_color(this->config.ceiling_color);
    this->clear_display();

    this->draw_viewport(Viewport::MAIN);
    this->hud_draw_eucliview();

    this->update_display();
}

Renderer::Renderer(
    
    const uint16_t window_width, 
    const uint16_t window_height,
    const uint16_t eucliview_height,
    const uint16_t eucliview_width,
    std::string window_title,
    const Map* const map_ptr,
    const Player* const player_ptr,
    const RendererConfig config

) : window_width{window_width}, 
    window_height{window_height}, 
    eucliview_height{eucliview_height}, 
    eucliview_width{eucliview_width},
    map_ptr{map_ptr},
    player_ptr{player_ptr},
    config{config}
{

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
        SDL_RENDERER_ACCELERATED | (int(config.vsync_enabled) * SDL_RENDERER_PRESENTVSYNC)
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