#ifndef GUARD_rayCollision_hpp
#define GUARD_rayCollision_hpp

#include "collidableObject.hpp"

struct RayCollision
{
    const double distance;
    const double offset;
    const CollidableObject *object;
};

#endif