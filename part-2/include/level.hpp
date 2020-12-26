#ifndef GUARD_level_hpp
#define GUARD_level_hpp

#include <SDL.h>

#include "player.hpp"
#include "raycaster.hpp"

class Level
{
    Player *_player;
    RayCaster *_rayCaster;
    SDL_Renderer *renderer_;
    const Map *map_;

public:
    bool shouldRenderMinimap = false;

    void Init(SDL_Renderer *, RayCaster *, const Map *);
    void render();
    Player *player() const;
};

#endif