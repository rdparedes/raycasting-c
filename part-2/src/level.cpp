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
    // Render projection
    renderer_->DrawBackground();
    renderer_->CallRayCaster(player_);
}

Player *Level::player() const { return player_; }