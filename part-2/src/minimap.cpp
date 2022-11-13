#include "config.hpp"
#include "minimap.hpp"

void Minimap::Init(SDL_Renderer *sdl_renderer, Renderer *renderer, const Player *player, const Map *map)
{
    sdl_renderer_ = sdl_renderer;
    renderer_ = renderer;
    player_ = player;
    map_ = map;
}

void Minimap::render()
{
    // wall color
    SDL_SetRenderDrawColor(sdl_renderer_, 0, 128, 255, 1);

    // render the map's walls
    int i = 0;
    for (auto &row : map_->encoded_map())
    {
        int j = 0;
        for (auto &elem : row)
        {
            if (elem == 'w')
            {
                const SDL_Rect wall_rectangle = {i * minimap_unit_, j * minimap_unit_, minimap_unit_, minimap_unit_};
                SDL_RenderDrawRect(sdl_renderer_, &wall_rectangle);
            }
            ++j;
        }
        ++i;
    }

    // black background
    SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, 0);

    // render the player
    double player_angle = floor(player_->sprite_rotation() / Config::PROJECTION_TO_360_RATIO);
    const SDL_Rect *player_rect = player_->minimap_rectangle();
    const SDL_Rect player_dst_rect = {
        int(player_rect->x * position_scale_) - minimap_sprite_offset_,
        int(player_rect->y * position_scale_) - minimap_sprite_offset_,
        int(player_rect->w),
        int(player_rect->h)};
    SDL_RenderCopyEx(
        sdl_renderer_,
        player_->marker_texture(),
        NULL,
        &player_dst_rect,
        player_angle,
        &player_->markerCenter,
        SDL_FLIP_NONE);
}
