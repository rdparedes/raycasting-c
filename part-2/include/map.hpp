#ifndef GUARD_map_hpp
#define GUARD_map_hpp

#include <SDL.h>
#include <vector>
#include <map>

#include "trigonometry.hpp"
#include "rayCollision.hpp"
#include "solidObject.hpp"

class Map
{
    std::vector<std::vector<char>> _map;
    std::map<const char, SolidObject *> _objectMap;

    const SolidObject *getSolidObject(const char &) const;
    const double calculateDistance(const bool &, const SDL_Point *, double &, const int &) const;

public:
    void init(const std::vector<std::vector<char>> &, const std::map<const char, SolidObject *> &);
    const RayCollision *checkSolidExists(const int &,
                                         const int &,
                                         const bool &,
                                         const SDL_Point *,
                                         double &,
                                         const int &) const;
};

#endif