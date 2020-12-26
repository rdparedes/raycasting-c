#include "config.hpp"
#include "level.hpp"

void Level::Init(SDL_Renderer *renderer, RayCaster *rayCaster, const Map *map)
{
    renderer_ = renderer;
    _rayCaster = rayCaster;
    _player = new Player();
    map_ = map;
    _player->Init(map_, renderer_, 96, 160);
}

void Level::render()
{
    if (shouldRenderMinimap)
    {
        // wall color
        SDL_SetRenderDrawColor(renderer_, 0, 128, 255, 1);

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
                    SDL_RenderDrawRect(renderer_, &rect);
                }
                ++j;
            }
            ++i;
        }

        // black
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);

        // render the player's marker
        double playerAngle = floor(_player->sprite_rotation() / Config::PROJECTION_TO_360_RATIO);
        SDL_RenderCopyEx(
            renderer_,
            _player->marker_texture(),
            NULL,
            _player->sprite_rectangle(),
            playerAngle,
            &_player->markerCenter,
            SDL_FLIP_NONE);
    }
    else
    // Render 'projection' of what the player sees
    {
        _rayCaster->Cast(_player);
    }
}

Player *Level::player() const { return _player; }