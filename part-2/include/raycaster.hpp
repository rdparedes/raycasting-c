#ifndef GUARD_raycaster_hpp
#define GUARD_raycaster_hpp

#include <vector>
#include <SDL2/SDL.h>
#include <map>
#include "player.hpp"
#include "map.hpp"
#include "rayCollision.hpp"

class RayCaster
{
    const Map *_map;
    SDL_Renderer *_renderer;

    void castSingleRay(const SDL_Point *, const SDL_Rect *, const int &, const int &) const;
    void drawRay(const RayCollision *, const int &, const int &) const;

    const RayCollision *findHorizontalRayCollision(const SDL_Point *, const SDL_Rect *, const int &) const;
    double findFirstHIntersectionX(const int &, const SDL_Point *, const double &) const;
    double findFirstHIntersectionY(const SDL_Point *, const bool &) const;
    double findNextHIntersectionX(const int &) const;
    double findNextHIntersectionY(const bool &) const;

    const RayCollision *findVerticalRayCollision(const SDL_Point *, const SDL_Rect *, const int &) const;
    double findFirstVIntersectionX(const SDL_Point *, const bool &) const;
    double findFirstVIntersectionY(const int &, const SDL_Point *, const double &) const;
    double findNextVIntersectionX(const bool &) const;
    double findNextVIntersectionY(const int &) const;

    bool rayIsOutsideViewArea(const SDL_Rect *, const double&, const double&) const;

public:
    void init(const Map *, SDL_Renderer *);
    void castRays(const Player *) const;
};

#endif