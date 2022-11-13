#ifndef GUARD_minimap_hpp
#define GUARD_minimap_hpp

#include <SDL2/SDL.h>

#include "player.hpp"
#include "renderer.hpp"

class Minimap
{
    const float position_scale_ = 0.25;
    const int minimap_sprite_offset_ = 6; // Ugly magic number to scale player marker down to minimap scale
    const int minimap_unit_ = Config::SPRITE_SIZE * position_scale_;
    const Player *player_;
    SDL_Renderer *sdl_renderer_;
    Renderer *renderer_;
    const Map *map_;

public:
    void Init(SDL_Renderer *, Renderer *, const Player *, const Map *);
    SDL_Rect *scale(const SDL_Rect *);
    void render();
};

#endif