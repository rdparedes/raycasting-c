#ifndef GUARD_level_hpp
#define GUARD_level_hpp

#include <SDL2/SDL.h>

#include "player.hpp"
#include "renderer.hpp"

class Level
{
    Player *player_;
    SDL_Renderer *sdl_renderer_;
    Renderer *renderer_;
    const Map *map_;

public:
    void Init(SDL_Renderer *, Renderer *, const Map *);
    void render();
    Player *player() const;
};

#endif