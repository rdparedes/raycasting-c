#include "map.hpp"

const char EMPTY = '.';

void Map::Init(const std::vector<std::vector<char>> &encoded_map, const std::map<const char, CollidableObject *> &object_dictionary)
{
    encoded_map_ = encoded_map;
    object_dictionary_ = object_dictionary;
    collidable_objects_ = {};
}

// Private methods

const CollidableObject *Map::GetCollidableObject(const char &key) const
{
    return object_dictionary_.at(key);
}

const double Map::CalculateDistance(const bool &isHorizontalCollision,
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

const std::vector<std::vector<char>> &Map::encoded_map() const { return encoded_map_; }

const RayCollision *Map::GetRayCollisionAt(const int &x,
                                           const int &y,
                                           const bool &isHorizontalCollision,
                                           const SDL_Point *playerPosition,
                                           double &intersection,
                                           const int &rayDegree) const
{
    if (x < 0 || y < 0 || x >= encoded_map_.size() || y >= encoded_map_[0].size())
    {
        return NULL;
    }
    const char objectKey = encoded_map_[x][y];

    if (objectKey == EMPTY)
    {
        return NULL;
    }

    return new RayCollision(
        {.distance = CalculateDistance(isHorizontalCollision, playerPosition, intersection, rayDegree),
         .offset = fmod(intersection, Config::SPRITE_SIZE),
         .object = GetCollidableObject(objectKey)});
}

const std::vector<CollidableObject> &Map::collidable_objects() const
{
    return collidable_objects_;
}

bool Map::CollisionExists(const SDL_Rect &rect) const
{
    // TODO
    for (auto o : collidable_objects())
    {
        if (SDL_HasIntersection(&rect, o.collision_box()))
        {
            return true;
        }
    }
    return false;
}
