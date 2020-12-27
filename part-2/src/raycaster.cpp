#include "config.hpp"
#include "trigonometry.hpp"
#include "raycaster.hpp"

const double PROJECTION_PLANE_DISTANCE = floor(Config::xCenter / tan(Trigonometry::degToRad(Config::FOV / 2.0)));

void RayCaster::Init(const Map *map, SDL_Renderer *renderer)
{
    map_ = map;
    renderer_ = renderer;
}

// Private methods

void RayCaster::CastSingleRay(const SDL_Point *playerPosition,
                              const SDL_Rect *playerViewArea,
                              const int &rayDegree,
                              const int &ray_index) const
{
    const RayCollision *horizontalRayCollision = FindHorizontalRayCollision(playerPosition,
                                                                            playerViewArea,
                                                                            rayDegree);
    const RayCollision *verticalRayCollision = FindVerticalRayCollision(playerPosition,
                                                                        playerViewArea,
                                                                        rayDegree);

    if (horizontalRayCollision == NULL && verticalRayCollision == NULL)
        return;

    if (horizontalRayCollision != NULL && verticalRayCollision == NULL)
        Draw(horizontalRayCollision, ray_index, rayDegree);
    else if (verticalRayCollision != NULL && horizontalRayCollision == NULL)
        Draw(verticalRayCollision, ray_index, rayDegree);
    else
    {
        // Draw the ray which is closest to the player
        if (horizontalRayCollision->distance < verticalRayCollision->distance)
            Draw(horizontalRayCollision, ray_index, rayDegree);
        else
            Draw(verticalRayCollision, ray_index, rayDegree);
    }
}

void RayCaster::Draw(const RayCollision *rayCollision,
                        const int &ray_index,
                        const int &rayDegree) const
{
    double corrected_distance = rayCollision->distance / Trigonometry::fFish[ray_index];
    double projected_wall_height = Config::SPRITE_SIZE * PROJECTION_PLANE_DISTANCE / corrected_distance;
    double bottom_of_wall = Config::y_center + int(projected_wall_height * 0.5);
    double top_of_wall = Config::y_center - int(projected_wall_height * 0.5);

    if (bottom_of_wall >= Config::screen_height)
    {
        bottom_of_wall = Config::screen_height-1;
    }
    if (top_of_wall < 0)
    {
        top_of_wall = 0;
    }

    // srcRect: part of the texture to be rendered in dstRect
    // dstRect: slice to be projected
    const SDL_Rect srcRect = {rayCollision->offset, 0, 1, Config::SPRITE_SIZE};
    const SDL_Rect dstRect = {ray_index, top_of_wall, 1, (bottom_of_wall - top_of_wall) + 1};

    // Draw wall
    SDL_RenderCopy(
        renderer_,
        rayCollision->object->texture(),
        &srcRect,
        &dstRect);
}

const RayCollision *RayCaster::FindHorizontalRayCollision(const SDL_Point *playerPosition,
                                                          const SDL_Rect *playerViewArea,
                                                          const int &rayDegree) const
{
    bool isFacingDown = rayDegree > Config::ANGLE0 && rayDegree < Config::ANGLE180;

    // First time it intersects
    double yIntersection = FindFirstHIntersectionY(playerPosition, isFacingDown);
    double xIntersection = FindFirstHIntersectionX(rayDegree, playerPosition, yIntersection);

    if (!isFacingDown)
        yIntersection -= 1.0;

    int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
    int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

    const RayCollision *collision = map_->GetRayCollisionAt(mapX, mapY, true, playerPosition, xIntersection, rayDegree);

    if (collision != NULL)
        return collision;

    // Subsequent intersections
    while (true)
    {
        yIntersection += FindNextHIntersectionY(isFacingDown);
        xIntersection += FindNextHIntersectionX(rayDegree);

        int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
        int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

        collision = map_->GetRayCollisionAt(mapX, mapY, true, playerPosition, xIntersection, rayDegree);

        if (collision != NULL)
        {
            return collision;
        }

        if (RayIsOutsideViewArea(playerViewArea, xIntersection, yIntersection))
        {
            return NULL;
        }
    }
}

double RayCaster::FindFirstHIntersectionY(const SDL_Point *playerPosition,
                                          const bool &isFacingDown) const
{
    double intersection = floor(playerPosition->y / double(Config::SPRITE_SIZE)) * Config::SPRITE_SIZE;
    if (isFacingDown)
        intersection += Config::SPRITE_SIZE;
    return intersection;
}

double RayCaster::FindFirstHIntersectionX(const int &rayDegree,
                                          const SDL_Point *playerPosition,
                                          const double &yIntersection) const
{
    return (Trigonometry::fITanTable[rayDegree] * (yIntersection - playerPosition->y)) + playerPosition->x;
}

double RayCaster::FindNextHIntersectionY(const bool &isFacingDown) const
{
    return (isFacingDown) ? double(Config::SPRITE_SIZE) : double(-Config::SPRITE_SIZE);
}

double RayCaster::FindNextHIntersectionX(const int &rayDegree) const
{
    return Trigonometry::fXStepTable[rayDegree];
}

const RayCollision *RayCaster::FindVerticalRayCollision(const SDL_Point *playerPosition,
                                                        const SDL_Rect *playerViewArea,
                                                        const int &rayDegree) const
{
    bool isFacingLeft = rayDegree > Config::ANGLE90 && rayDegree < Config::ANGLE270;

    // First time it intersects
    double xIntersection = FindFirstVIntersectionX(playerPosition, isFacingLeft);
    double yIntersection = FindFirstVIntersectionY(rayDegree, playerPosition, xIntersection);

    if (isFacingLeft)
        xIntersection -= 1.0;

    int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
    int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

    const RayCollision *collision = map_->GetRayCollisionAt(mapX, mapY, false, playerPosition, yIntersection, rayDegree);

    if (collision != NULL)
    {
        return collision;
    }

    // Subsequent intersections
    while (true)
    {
        xIntersection += FindNextVIntersectionX(isFacingLeft);
        yIntersection += FindNextVIntersectionY(rayDegree);

        int mapX = floor(xIntersection / double(Config::SPRITE_SIZE));
        int mapY = floor(yIntersection / double(Config::SPRITE_SIZE));

        collision = map_->GetRayCollisionAt(mapX, mapY, false, playerPosition, yIntersection, rayDegree);

        if (collision != NULL)
        {
            return collision;
        }

        if (RayIsOutsideViewArea(playerViewArea, xIntersection, yIntersection))
        {
            return NULL;
        }
    }
}

double RayCaster::FindFirstVIntersectionX(const SDL_Point *playerPosition,
                                          const bool &isFacingLeft) const
{
    double intersection = floor(playerPosition->x / double(Config::SPRITE_SIZE)) * Config::SPRITE_SIZE;
    if (!isFacingLeft)
        intersection += Config::SPRITE_SIZE;
    return intersection;
}

double RayCaster::FindFirstVIntersectionY(const int &rayDegree,
                                          const SDL_Point *playerPosition,
                                          const double &xIntersection) const
{
    return (Trigonometry::fTanTable[rayDegree] * (xIntersection - playerPosition->x)) + playerPosition->y;
}

double RayCaster::FindNextVIntersectionX(const bool &isFacingLeft) const
{
    return (isFacingLeft) ? double(-Config::SPRITE_SIZE) : double(Config::SPRITE_SIZE);
}

double RayCaster::FindNextVIntersectionY(const int &rayDegree) const
{
    return Trigonometry::fYStepTable[rayDegree];
}

bool RayCaster::RayIsOutsideViewArea(const SDL_Rect *playerViewArea,
                                     const double &xIntersection,
                                     const double &yIntersection) const
{
    // If ray has gone out of view area, stop casting it
    return (yIntersection < playerViewArea->y || yIntersection > playerViewArea->h || xIntersection < playerViewArea->x || xIntersection > playerViewArea->w);
}

// Public methods

void RayCaster::Cast(const Player *player) const
{
    // Start casting rays from left to right of FOV
    int rayDegree = player->rotation() - Config::ANGLE30;
    if (rayDegree < 0)
        rayDegree += Config::ANGLE360;

    for (int ray_index = 0; ray_index != Config::screen_width; ++ray_index)
    {
        CastSingleRay(player->position(), player->view_area(), rayDegree, ray_index);
        ++rayDegree;
        if (rayDegree >= Config::ANGLE360)
        {
            rayDegree -= Config::ANGLE360;
        }
    }
}