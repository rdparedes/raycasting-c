#include "config.hpp"
#include "level.hpp"

Level::Level(const std::vector<std::vector<char>> &map)
{
    _map = map;
}

void Level::init(SDL_Renderer *renderer, RayCaster *rayCaster)
{
    _renderer = renderer;
    _player = new Player(128, 128);
    _player->init(_renderer);
    _rayCaster = rayCaster;
}

void Level::render()
{
    // render the player's marker
    double playerAngle = floor(_player->spriteRotation() / Config::PROJECTION_TO_360_RATIO);

    SDL_RenderCopyEx(
        _renderer,
        _player->texture(),
        NULL,
        _player->rectangle(),
        playerAngle,
        &_player->center,
        SDL_FLIP_NONE);

    // Render 3d projection
    _rayCaster->castRays(_player);

}

Player *Level::player() const { return _player; }