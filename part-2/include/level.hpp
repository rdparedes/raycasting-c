#ifndef GUARD_level_hpp
#define GUARD_level_hpp

#include <SDL2/SDL.h>

#include <vector>
#include "player.hpp"
#include "wall.hpp"
#include "raycaster.hpp"

class Level
{
    Player *_player;
    Wall *_wall;
    RayCaster *_rayCaster;
    std::vector<std::vector<char>> _map;
    SDL_Renderer *_renderer;

public:
    Level(const std::vector<std::vector<char>> &);
    void init(SDL_Renderer *, RayCaster *);
    void render();
    Player *player() const;
};

#endif