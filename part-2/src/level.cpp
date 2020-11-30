#include "config.hpp"
#include "level.hpp"

void Level::init(SDL_Renderer *renderer, RayCaster *rayCaster, const Map *map)
{
    _renderer = renderer;
    _rayCaster = rayCaster;
    _player = new Player();
    _player->init(_renderer, 96, 160);
    _map = map;
}

void Level::render()
{
    if (shouldRenderMinimap)
    {
        SDL_SetRenderDrawColor(_renderer, 0, 128, 255, 1); // wall color

        // render the map's walls
        int i = 0;
        for (auto &row : _map->getMap())
        {
            int j = 0;
            for (auto &elem : row)
            {
                if (elem == 'w')
                {
                    const SDL_Rect rect = {i * Config::SPRITE_SIZE, j * Config::SPRITE_SIZE, Config::SPRITE_SIZE, Config::SPRITE_SIZE};
                    SDL_RenderDrawRect(_renderer, &rect);
                }
                ++j;
            }
            ++i;
        }

        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0); // black

        // render the player's marker
        double playerAngle = floor(_player->spriteRotation() / Config::PROJECTION_TO_360_RATIO);
        SDL_RenderCopyEx(
            _renderer,
            _player->markerTexture(),
            NULL,
            _player->markerRect(),
            playerAngle,
            &_player->markerCenter,
            SDL_FLIP_NONE);
    }
    else
    {
        // Render 'projection' of what the player sees
        _rayCaster->castRays(_player);
    }
}

Player *Level::player() const { return _player; }