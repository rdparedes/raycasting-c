#ifndef GUARD_rayCollision_hpp
#define GUARD_rayCollision_hpp

#include "solidObject.hpp"

struct RayCollision
{
    const double distance;
    const double offset;
    const SolidObject *object;
};

#endif