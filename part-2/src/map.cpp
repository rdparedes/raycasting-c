#include "map.hpp"
#include <map>
#include <iostream>

const char kEmpty = '.';

void Map::Init(const std::vector<std::vector<char>> &encoded_map, const std::map<const char, CollidableObject *> &object_dictionary)
{
    encoded_map_ = encoded_map;
    object_dictionary_ = std::map(object_dictionary);
    const auto tile_size = Config::SPRITE_SIZE;

    int i = 0;
    try {
        for (auto &row : encoded_map_)
        {
            int j = 0;
            for (auto &elem : row)
            {
                if (elem == 'w')
                {
                        CollidableObject *collidable = new CollidableObject(*GetCollidableObjectAt('w'));
                        collidable->set_collision_box({i * tile_size, j * tile_size, tile_size, tile_size});
                        collidable_objects_.push_back(collidable);
                }
                ++j;
            }
            ++i;
        }
    } catch (const std::exception& e) {
        std::cout << "Could not render objects inside the Map: " << e.what();
    }
}

// Private methods

const CollidableObject *Map::GetCollidableObjectAt(const char &key) const
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
    const auto rows = encoded_map_.size();
    const auto cols = encoded_map_[0].size();
    if (x < 0 || y < 0 || x >= rows || y >= cols)
    {
        return NULL;
    }
    const char objectKey = encoded_map_[x][y];

    if (objectKey == kEmpty)
    {
        return NULL;
    }

    return new RayCollision(
        {.distance = CalculateDistance(isHorizontalCollision, playerPosition, intersection, rayDegree),
         .offset = fmod(intersection, Config::SPRITE_SIZE),
         .object = GetCollidableObjectAt(objectKey)});
}

const std::vector<const CollidableObject *> &Map::collidable_objects() const
{
    return collidable_objects_;
}

bool Map::CollisionExists(const SDL_Rect &rect) const
{
    for (auto o : collidable_objects())
    {
        if (SDL_HasIntersection(&rect, o->collision_box()))
        {
            return true;
        }
    }
    return false;
}
