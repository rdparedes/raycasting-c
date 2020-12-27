#include "config.hpp"
#include "level.hpp"

void Level::Init(SDL_Renderer *sdl_renderer, Renderer *renderer, const Map *map)
{
    sdl_renderer_ = sdl_renderer;
    renderer_ = renderer;
    player_ = new Player();
    map_ = map;
    player_->Init(map_, sdl_renderer_, 96, 160);
}

void Level::render()
{
    if (should_render_minimap)
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
                    const SDL_Rect rect = {i * Config::SPRITE_SIZE, j * Config::SPRITE_SIZE, Config::SPRITE_SIZE, Config::SPRITE_SIZE};
                    SDL_RenderDrawRect(sdl_renderer_, &rect);
                }
                ++j;
            }
            ++i;
        }

        // black
        SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, 0);

        // render the player's marker
        double playerAngle = floor(player_->sprite_rotation() / Config::PROJECTION_TO_360_RATIO);
        SDL_RenderCopyEx(
            sdl_renderer_,
            player_->marker_texture(),
            NULL,
            player_->sprite_rectangle(),
            playerAngle,
            &player_->markerCenter,
            SDL_FLIP_NONE);
    }
    else
    // Render projection
    {
        renderer_->DrawBackground();
        renderer_->CallRayCaster(player_);
    }
}

Player *Level::player() const { return player_; }