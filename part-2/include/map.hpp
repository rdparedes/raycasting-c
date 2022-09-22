#ifndef GUARD_map_hpp
#define GUARD_map_hpp

#include <SDL2/SDL.h>
#include <vector>
#include <map>

#include "trigonometry.hpp"
#include "rayCollision.hpp"
#include "collidableObject.hpp"

class Map
{
    std::vector<std::vector<char>> encoded_map_;
    std::map<const char, CollidableObject *> object_dictionary_;
    std::vector<const CollidableObject *> collidable_objects_;

    const CollidableObject *GetCollidableObjectAt(const char &) const;
    const double CalculateDistance(const bool &, const SDL_Point *, double &, const int &) const;

public:
    void Init(const std::vector<std::vector<char>> &, const std::map<const char, CollidableObject *> &);
    const std::vector<std::vector<char>> &encoded_map() const;
    const std::vector<const CollidableObject *> &collidable_objects() const;
    bool CollisionExists(const SDL_Rect &) const;
    const RayCollision *GetRayCollisionAt(const int &,
                                       const int &,
                                       const bool &,
                                       const SDL_Point *,
                                       double &,
                                       const int &) const;
};

#endif