#include "config.hpp"
#include "level.hpp"

void Level::init(SDL_Renderer *renderer, RayCaster *rayCaster)
{
    _renderer = renderer;
    _rayCaster = rayCaster;
    _player = new Player();
    _player->init(_renderer, 128, 128);
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