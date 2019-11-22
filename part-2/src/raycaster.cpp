#include "config.hpp"
#include "trigonometry.hpp"
#include "raycaster.hpp"
#include <iostream>

RayCaster::RayCaster(const std::vector<std::vector<char>> &map)
{
    _map = map;
}

// Private methods

void RayCaster::castSingleRay(const SDL_Rect *playerRectangle, const SDL_Rect *playerViewArea, int &rayDegree) const
{
    const RayCollision *horizontalRayCollision = findHorizontalRayCollision(playerRectangle,
                                                                            playerViewArea,
                                                                            rayDegree);
    const RayCollision *verticalRayCollision = findVerticalRayCollision(playerRectangle,
                                                                        playerViewArea,
                                                                        rayDegree);
    const RayCollision *rayCollision = nullptr;

    if (horizontalRayCollision != NULL && verticalRayCollision == NULL)
    {
        drawRay(horizontalRayCollision);
    }
    else if (verticalRayCollision != NULL && horizontalRayCollision == NULL)
    {
        drawRay(verticalRayCollision);
    }
    else if (horizontalRayCollision != NULL && verticalRayCollision != NULL)
    {
        if (horizontalRayCollision->distance < verticalRayCollision->distance)
        {
            drawRay(horizontalRayCollision);
        }
        else
        {
            drawRay(verticalRayCollision);
        }
    }
}

void RayCaster::drawRay(const RayCollision * rayCollision) const
{
    std::cout << "TODO: draw ray: Distance: " << rayCollision->distance << ", Offset: " << rayCollision->offset << std::endl ;
}

const RayCollision *RayCaster::findHorizontalRayCollision(const SDL_Rect *playerRectangle,
                                                          const SDL_Rect *playerViewArea,
                                                          const int &rayDegree) const
{
    bool isFacingDown = rayDegree > Config::ANGLE0 && rayDegree < Config::ANGLE180;

    // First time it intersects
    double yIntersection = findFirstHIntersectionY(playerRectangle, isFacingDown);
    double xIntersection = findFirstHIntersectionX(rayDegree, playerRectangle, yIntersection);

    // std::cout << "yIntersection: " << yIntersection << std::endl;
    // std::cout << "xIntersection: " << xIntersection << std::endl;

    if (!isFacingDown)
        yIntersection -= 1.0;

    int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
    int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

    if (solidExists(mapX, mapY))
    {
        return new RayCollision(
            {.distance = (xIntersection - playerRectangle->x) * Trigonometry::fICosTable[rayDegree],
             .offset = fmod(xIntersection, Config::SPRITE_SIZE)});
    }

    // Subsequent intersections
    while (true)
    {
        yIntersection += findNextHIntersectionY(isFacingDown);
        xIntersection += findNextHIntersectionX(rayDegree);

        // std::cout << "yIntersection NEXt: " << yIntersection << std::endl;
        // std::cout << "xIntersection Next: " << xIntersection << std::endl;

        int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
        int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

        if (solidExists(mapX, mapY))
        {
            return new RayCollision(
                {.distance = (xIntersection - playerRectangle->x) * Trigonometry::fICosTable[rayDegree],
                 .offset = fmod(xIntersection, Config::SPRITE_SIZE)});
        }

        if (rayIsOutsideViewArea(playerViewArea, xIntersection, yIntersection))
        {
            // std::cout << "ray is outside boundaries";
            return NULL;
        }
    }
}

double RayCaster::findFirstHIntersectionY(const SDL_Rect *playerRectangle,
                                          const bool &isFacingDown) const
{
    double intersection = floor(playerRectangle->y / double(Config::SPRITE_SIZE)) * Config::SPRITE_SIZE;
    if (isFacingDown)
        intersection += Config::SPRITE_SIZE;
    return intersection;
}

double RayCaster::findFirstHIntersectionX(const int &rayDegree,
                                          const SDL_Rect *playerRectangle,
                                          const double &yIntersection) const
{
    return (Trigonometry::fITanTable[rayDegree] * (yIntersection - playerRectangle->y)) + playerRectangle->x;
}

double RayCaster::findNextHIntersectionY(const bool &isFacingDown) const
{
    return (isFacingDown) ? double(Config::SPRITE_SIZE) : double(-Config::SPRITE_SIZE);
}

double RayCaster::findNextHIntersectionX(const int &rayDegree) const
{
    return Trigonometry::fXStepTable[rayDegree];
}

const RayCollision *RayCaster::findVerticalRayCollision(const SDL_Rect *playerRectangle,
                                                        const SDL_Rect *playerViewArea,
                                                        const int &rayDegree) const
{
    bool isFacingLeft = rayDegree > Config::ANGLE90 && rayDegree < Config::ANGLE270;

    // First time it intersects
    double xIntersection = findFirstVIntersectionX(playerRectangle, isFacingLeft);
    double yIntersection = findFirstVIntersectionY(rayDegree, playerRectangle, xIntersection);

    // std::cout << "yIntersection: " << yIntersection << std::endl;
    // std::cout << "xIntersection: " << xIntersection << std::endl;

    if (isFacingLeft)
        xIntersection -= 1.0;

    int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
    int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

    if (solidExists(mapX, mapY))
    {
        return new RayCollision(
            {.distance = (yIntersection - playerRectangle->y) * Trigonometry::fISinTable[rayDegree],
             .offset = fmod(yIntersection, Config::SPRITE_SIZE)});
    }

    // Subsequent intersections
    while (true)
    {
        xIntersection += findNextVIntersectionX(isFacingLeft);
        yIntersection += findNextVIntersectionY(rayDegree);

        // std::cout << "yIntersection NEXt: " << yIntersection << std::endl;
        // std::cout << "xIntersection Next: " << xIntersection << std::endl;

        int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
        int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

        if (solidExists(mapX, mapY))
        {
            return new RayCollision(
                {.distance = (yIntersection - playerRectangle->y) * Trigonometry::fISinTable[rayDegree],
                 .offset = fmod(yIntersection, Config::SPRITE_SIZE)});
        }

        if (rayIsOutsideViewArea(playerViewArea, xIntersection, yIntersection))
        {
            // std::cout << "ray is outside boundaries";
            return NULL;
        }
    }
}

double RayCaster::findFirstVIntersectionX(const SDL_Rect *playerRectangle,
                                          const bool &isFacingLeft) const
{
    double intersection = floor(playerRectangle->x / double(Config::SPRITE_SIZE)) * Config::SPRITE_SIZE;
    if (!isFacingLeft)
        intersection += Config::SPRITE_SIZE;
    return intersection;
}

double RayCaster::findFirstVIntersectionY(const int &rayDegree,
                                          const SDL_Rect *playerRectangle,
                                          const double &xIntersection) const
{
    return (Trigonometry::fTanTable[rayDegree] * (xIntersection - playerRectangle->x)) + playerRectangle->y;
}

double RayCaster::findNextVIntersectionX(const bool &isFacingLeft) const
{
    return (isFacingLeft) ? double(-Config::SPRITE_SIZE) : double(Config::SPRITE_SIZE);
}

double RayCaster::findNextVIntersectionY(const int &rayDegree) const
{
    return Trigonometry::fYStepTable[rayDegree];
}

bool RayCaster::rayIsOutsideViewArea(const SDL_Rect *playerViewArea,
                                     const double &xIntersection,
                                     const double &yIntersection) const
{
    // If ray has gone out of view area, stop casting it
    // std::cout << playerViewArea->x << ", " << playerViewArea->y << ", " << playerViewArea->w << ", " << playerViewArea->h << std::endl;
    return (yIntersection < playerViewArea->y
        || yIntersection > playerViewArea->h
        || xIntersection < playerViewArea->x
        || xIntersection > playerViewArea->w);
}

bool RayCaster::solidExists(const int &x, const int &y) const
{
    if (x < 0 || y < 0 || x >= _map.size() || y >= _map[0].size())
    {
        return false;
    }
    return (_map[x][y] == 'w');
}

// Public methods

void RayCaster::castRays(const Player *player) const
{
    // Start casting rays from left to right of FOV
    int rayDegree = player->rotation() - Config::ANGLE30;
    if (rayDegree < 0)
        rayDegree += Config::ANGLE360;

    for (int ray_index = 0; ray_index != Config::screenSizeX; ++ray_index)
    {
        castSingleRay(player->rectangle(), player->viewArea(), rayDegree);
    }
}