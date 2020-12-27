#ifndef GUARD_renderer_hpp
#define GUARD_renderer_hpp

#include <SDL.h>
#include "player.hpp"
#include "map.hpp"
#include "raycaster.hpp"

class Renderer
{
    RayCaster *ray_caster_;
    SDL_Renderer *sdl_renderer_;

public:
    void Init(RayCaster *, SDL_Renderer *);
    void DrawBackground();
    void CallRayCaster(const Player *);
};

#endif