#include "map.hpp"

const char EMPTY = '.';

void Map::init(const std::vector<std::vector<char>> &map, const std::map<const char, SolidObject *> &objectMap)
{
    _map = map;
    _objectMap = objectMap;
}

// Private methods

const SolidObject *Map::getSolidObject(const char &key) const
{
    return _objectMap.at(key);
}

const double Map::calculateDistance(const bool &isHorizontalCollision,
                                    const SDL_Point *playerPosition,
                                    double &intersection,
                                    const int &rayDegree) const
{
    if (isHorizontalCollision)
    {
        return (intersection - playerPosition->x) * Trigonometry::fICosTable[rayDegree];
    }
    return (intersection - playerPosition->y) * Trigonometry::fISinTable[rayDegree];
}

// Public methods

const RayCollision *Map::checkSolidExists(const int &x,
                                          const int &y,
                                          const bool &isHorizontalCollision,
                                          const SDL_Point *playerPosition,
                                          double &intersection,
                                          const int &rayDegree) const
{
    if (x < 0 || y < 0 || x >= _map.size() || y >= _map[0].size())
    {
        return NULL;
    }
    const char objectKey = _map[x][y];

    if (objectKey == EMPTY)
    {
        return NULL;
    }

    return new RayCollision(
        {.distance = calculateDistance(isHorizontalCollision, playerPosition, intersection, rayDegree),
         .offset = fmod(intersection, Config::SPRITE_SIZE),
         .object = getSolidObject(objectKey)});
}