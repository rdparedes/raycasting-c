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
    const Map *map_;
    SDL_Renderer *renderer_;

    void CastSingleRay(const Player *, const int &, const int &) const;
    void DrawWallSlice(const RayCollision *, const double &, const double &, const double &, const double &, const int &) const;

    const RayCollision *FindHorizontalRayCollision(const Player *, const int &) const;
    double FindFirstHIntersectionX(const int &, const SDL_Point *, const double &) const;
    double FindFirstHIntersectionY(const SDL_Point *, const bool &) const;
    double FindNextHIntersectionX(const int &) const;
    double FindNextHIntersectionY(const bool &) const;

    const RayCollision *FindVerticalRayCollision(const Player *, const int &) const;
    double FindFirstVIntersectionX(const SDL_Point *, const bool &) const;
    double FindFirstVIntersectionY(const int &, const SDL_Point *, const double &) const;
    double FindNextVIntersectionX(const bool &) const;
    double FindNextVIntersectionY(const int &) const;

    bool RayIsOutsideViewArea(const SDL_Rect *, const double &, const double &) const;

public:
    void Init(const Map *, SDL_Renderer *);
    void Cast(const Player *) const;
};

#endif