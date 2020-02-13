#include "config.hpp"
#include "trigonometry.hpp"
#include "raycaster.hpp"

const double PROJECTION_PLANE_DISTANCE = floor(Config::xCenter / tan(Trigonometry::degToRad(Config::FOV / 2.0)));

void RayCaster::init(const Map *map, SDL_Renderer *renderer)
{
    _map = map;
    _renderer = renderer;
}

// Private methods

void RayCaster::castSingleRay(const SDL_Point *playerPosition,
                              const SDL_Rect *playerViewArea,
                              const int &rayDegree,
                              const int &rayIndex) const
{
    const RayCollision *horizontalRayCollision = findHorizontalRayCollision(playerPosition,
                                                                            playerViewArea,
                                                                            rayDegree);
    const RayCollision *verticalRayCollision = findVerticalRayCollision(playerPosition,
                                                                        playerViewArea,
                                                                        rayDegree);
    if (horizontalRayCollision != NULL && verticalRayCollision == NULL)
    {
        drawRay(horizontalRayCollision, rayIndex, rayDegree);
    }
    else if (verticalRayCollision != NULL && horizontalRayCollision == NULL)
    {
        drawRay(verticalRayCollision, rayIndex, rayDegree);
    }
    else if (horizontalRayCollision != NULL && verticalRayCollision != NULL)
    {
        if (horizontalRayCollision->distance < verticalRayCollision->distance)
        {
            drawRay(horizontalRayCollision, rayIndex, rayDegree);
        }
        else
        {
            drawRay(verticalRayCollision, rayIndex, rayDegree);
        }
    }
}

void RayCaster::drawRay(const RayCollision *rayCollision,
                        const int &rayIndex,
                        const int &rayDegree) const
{
    double correctedDistance = rayCollision->distance / Trigonometry::fFish[rayIndex];
    double projectedSliceHeight = Config::SPRITE_SIZE * PROJECTION_PLANE_DISTANCE / correctedDistance;

    // srcRect: part of the texture to be rendered in dstRect
    // dstRect: slice to be projected
    const SDL_Rect srcRect = {rayCollision->offset, 0, 1, Config::SPRITE_SIZE};
    const SDL_Rect dstRect = {rayIndex, Config::yCenter - int(projectedSliceHeight / 2), 1, projectedSliceHeight};

    SDL_RenderCopy(
        _renderer,
        rayCollision->object->texture(),
        &srcRect,
        &dstRect);
}

const RayCollision *RayCaster::findHorizontalRayCollision(const SDL_Point *playerPosition,
                                                          const SDL_Rect *playerViewArea,
                                                          const int &rayDegree) const
{
    bool isFacingDown = rayDegree > Config::ANGLE0 && rayDegree < Config::ANGLE180;

    // First time it intersects
    double yIntersection = findFirstHIntersectionY(playerPosition, isFacingDown);
    double xIntersection = findFirstHIntersectionX(rayDegree, playerPosition, yIntersection);

    if (!isFacingDown)
        yIntersection -= 1.0;

    int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
    int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

    const RayCollision *collision = _map->checkSolidExists(mapX, mapY, true, playerPosition, xIntersection, rayDegree);

    if (collision != NULL)
        return collision;

    // Subsequent intersections
    while (true)
    {
        yIntersection += findNextHIntersectionY(isFacingDown);
        xIntersection += findNextHIntersectionX(rayDegree);

        int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
        int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

        collision = _map->checkSolidExists(mapX, mapY, true, playerPosition, xIntersection, rayDegree);

        if (collision != NULL)
        {
            return collision;
        }

        if (rayIsOutsideViewArea(playerViewArea, xIntersection, yIntersection))
        {
            return NULL;
        }
    }
}

double RayCaster::findFirstHIntersectionY(const SDL_Point *playerPosition,
                                          const bool &isFacingDown) const
{
    double intersection = floor(playerPosition->y / double(Config::SPRITE_SIZE)) * Config::SPRITE_SIZE;
    if (isFacingDown)
        intersection += Config::SPRITE_SIZE;
    return intersection;
}

double RayCaster::findFirstHIntersectionX(const int &rayDegree,
                                          const SDL_Point *playerPosition,
                                          const double &yIntersection) const
{
    return (Trigonometry::fITanTable[rayDegree] * (yIntersection - playerPosition->y)) + playerPosition->x;
}

double RayCaster::findNextHIntersectionY(const bool &isFacingDown) const
{
    return (isFacingDown) ? double(Config::SPRITE_SIZE) : double(-Config::SPRITE_SIZE);
}

double RayCaster::findNextHIntersectionX(const int &rayDegree) const
{
    return Trigonometry::fXStepTable[rayDegree];
}

const RayCollision *RayCaster::findVerticalRayCollision(const SDL_Point *playerPosition,
                                                        const SDL_Rect *playerViewArea,
                                                        const int &rayDegree) const
{
    bool isFacingLeft = rayDegree > Config::ANGLE90 && rayDegree < Config::ANGLE270;

    // First time it intersects
    double xIntersection = findFirstVIntersectionX(playerPosition, isFacingLeft);
    double yIntersection = findFirstVIntersectionY(rayDegree, playerPosition, xIntersection);

    if (isFacingLeft)
        xIntersection -= 1.0;

    int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
    int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

    const RayCollision *collision = _map->checkSolidExists(mapX, mapY, false, playerPosition, yIntersection, rayDegree);

    if (collision != NULL)
    {
        return collision;
    }

    // Subsequent intersections
    while (true)
    {
        xIntersection += findNextVIntersectionX(isFacingLeft);
        yIntersection += findNextVIntersectionY(rayDegree);

        int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
        int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

        collision = _map->checkSolidExists(mapX, mapY, false, playerPosition, yIntersection, rayDegree);

        if (collision != NULL)
        {
            return collision;
        }

        if (rayIsOutsideViewArea(playerViewArea, xIntersection, yIntersection))
        {
            return NULL;
        }
    }
}

double RayCaster::findFirstVIntersectionX(const SDL_Point *playerPosition,
                                          const bool &isFacingLeft) const
{
    double intersection = floor(playerPosition->x / double(Config::SPRITE_SIZE)) * Config::SPRITE_SIZE;
    if (!isFacingLeft)
        intersection += Config::SPRITE_SIZE;
    return intersection;
}

double RayCaster::findFirstVIntersectionY(const int &rayDegree,
                                          const SDL_Point *playerPosition,
                                          const double &xIntersection) const
{
    return (Trigonometry::fTanTable[rayDegree] * (xIntersection - playerPosition->x)) + playerPosition->y;
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
    return (yIntersection < playerViewArea->y || yIntersection > playerViewArea->h || xIntersection < playerViewArea->x || xIntersection > playerViewArea->w);
}

// Public methods

void RayCaster::castRays(const Player *player) const
{
    // Start casting rays from left to right of FOV
    int rayDegree = player->rotation() - Config::ANGLE30;
    if (rayDegree < 0)
        rayDegree += Config::ANGLE360;

    for (int rayIndex = 0; rayIndex != Config::screenSizeX; ++rayIndex)
    {
        castSingleRay(player->position(), player->viewArea(), rayDegree, rayIndex);
        ++rayDegree;
        if (rayDegree >= Config::ANGLE360)
        {
            rayDegree -= Config::ANGLE360;
        }
    }
}