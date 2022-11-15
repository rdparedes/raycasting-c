#include "config.hpp"
#include "trigonometry.hpp"
#include "raycaster.hpp"

const double kProjectionPlaneDistance = floor(Config::xCenter / tan(Trigonometry::DegToRad(Config::FOV / 2.0)));
const char *kFloorImageSrc = "../sprites/floor.png";

void RayCaster::Init(const Map *map, SDL_Renderer *renderer)
{
    map_ = map;
    renderer_ = renderer;
    SDL_Surface *floor_surface_loader = IMG_Load(kFloorImageSrc);
    SDL_Texture *floor_texture = SDL_CreateTextureFromSurface(renderer_, floor_surface_loader);
    SDL_FreeSurface(floor_surface_loader);
}

// Private methods

void RayCaster::CastSingleRay(const Player *player,
                              const int &ray_degree,
                              const int &ray_index) const
{
    const RayCollision *horizontal_ray_collision = FindHorizontalRayCollision(player,
                                                                              ray_degree);
    const RayCollision *vertical_ray_collision = FindVerticalRayCollision(player,
                                                                          ray_degree);

    if (horizontal_ray_collision == NULL && vertical_ray_collision == NULL)
        return;

    bool is_vertical_collision;
    if (horizontal_ray_collision != NULL && vertical_ray_collision == NULL)
        is_vertical_collision = false;
    else if (vertical_ray_collision != NULL && horizontal_ray_collision == NULL)
        is_vertical_collision = true;
    else
    {
        // Draw the ray which is closest to the player
        if (horizontal_ray_collision->distance < vertical_ray_collision->distance)
            is_vertical_collision = false;
        else
            is_vertical_collision = true;
    }

    int light_value;
    const RayCollision *ray_collision;

    if (is_vertical_collision)
    {
        light_value = 180;
        ray_collision = vertical_ray_collision;
    }
    else
    {
        light_value = 120;
        ray_collision = horizontal_ray_collision;
    }

    const double corrected_distance = ray_collision->distance / Trigonometry::fFish[ray_index];
    const double brightness_level = light_value / floor(corrected_distance);
    const double projected_wall_height = Config::SPRITE_SIZE * kProjectionPlaneDistance / corrected_distance;
    const double bottom_of_wall = Config::y_center + int(projected_wall_height * 0.5);
    const double top_of_wall = Config::y_center - int(projected_wall_height * 0.5);

    DrawWallSlice(ray_collision, corrected_distance, brightness_level, bottom_of_wall, top_of_wall, ray_index);

    const int last_bottom_of_wall = floor(bottom_of_wall);

    // Floor Casting

    // TODO: Continue working on floor casting
    // for (int row = last_bottom_of_wall; row < Config::game_height; row++)
    // {
    //     const double ratio =
    // }
}

void RayCaster::DrawWallSlice(const RayCollision *ray_collision,
                              const double &corrected_distance,
                              const double &brightness_level,
                              const double &bottom_of_wall,
                              const double &top_of_wall,
                              const int &ray_index) const
{
    double intensity = 1.3;
    if (corrected_distance > 600)
    {
        intensity = 1;
    }
    else if (corrected_distance > 900)
    {
        intensity = 0.75;
    }
    else if (corrected_distance > 1200)
    {
        intensity = 0.5;
    }

    // srcRect: part of the texture to be rendered in dstRect
    const SDL_Rect srcRect = {int(ray_collision->offset), 0, 1, Config::SPRITE_SIZE};
    // dstRect: slice to be projected on screen
    const SDL_Rect dstRect = {ray_index, int(top_of_wall), 1, int(bottom_of_wall - top_of_wall) + 1};

    // Draw wall
    int color = 255 * brightness_level * intensity;

    if (color > 255)
        color = 255;
    if (color < 60)
        color = 60;
    const Uint8 r = color;
    const Uint8 g = color;
    const Uint8 b = color;
    SDL_SetTextureColorMod(ray_collision->object->texture(), r, g, b);
    SDL_RenderCopy(
        renderer_,
        ray_collision->object->texture(),
        &srcRect,
        &dstRect);
}

const RayCollision *RayCaster::FindHorizontalRayCollision(const Player *player,
                                                          const int &ray_degree) const
{
    const bool is_facing_down = ray_degree > Config::ANGLE0 && ray_degree < Config::ANGLE180;

    // First time it intersects
    double y_intersection = FindFirstHIntersectionY(player->position(), is_facing_down);
    double x_intersection = FindFirstHIntersectionX(ray_degree, player->position(), y_intersection);

    if (!is_facing_down)
        y_intersection -= 1.0;

    int mapX = floor(x_intersection / double(Config::SPRITE_SIZE));
    int mapY = floor(y_intersection / double(Config::SPRITE_SIZE));

    const RayCollision *collision = map_->GetRayCollisionAt(mapX, mapY, true, player->position(), x_intersection, ray_degree);

    if (collision != NULL)
        return collision;

    // Subsequent intersections
    while (true)
    {
        y_intersection += FindNextHIntersectionY(is_facing_down);
        x_intersection += FindNextHIntersectionX(ray_degree);

        int mapX = floor(x_intersection / double(Config::SPRITE_SIZE));
        int mapY = floor(y_intersection / double(Config::SPRITE_SIZE));

        collision = map_->GetRayCollisionAt(mapX, mapY, true, player->position(), x_intersection, ray_degree);

        if (collision != NULL)
        {
            return collision;
        }

        if (RayIsOutsideViewArea(player->view_area(), x_intersection, y_intersection))
        {
            return NULL;
        }
    }
}

double RayCaster::FindFirstHIntersectionY(const SDL_Point *player_position,
                                          const bool &is_facing_down) const
{
    double intersection = floor(player_position->y / double(Config::SPRITE_SIZE)) * Config::SPRITE_SIZE;
    if (is_facing_down)
        intersection += Config::SPRITE_SIZE;
    return intersection;
}

double RayCaster::FindFirstHIntersectionX(const int &ray_degree,
                                          const SDL_Point *player_position,
                                          const double &y_intersection) const
{
    return (Trigonometry::fITanTable[ray_degree] * (y_intersection - player_position->y)) + player_position->x;
}

double RayCaster::FindNextHIntersectionY(const bool &is_facing_down) const
{
    return (is_facing_down) ? double(Config::SPRITE_SIZE) : double(-Config::SPRITE_SIZE);
}

double RayCaster::FindNextHIntersectionX(const int &ray_degree) const
{
    return Trigonometry::fXStepTable[ray_degree];
}

const RayCollision *RayCaster::FindVerticalRayCollision(const Player *player,
                                                        const int &ray_degree) const
{
    const bool is_facing_left = ray_degree > Config::ANGLE90 && ray_degree < Config::ANGLE270;

    // First time it intersects
    double x_intersection = FindFirstVIntersectionX(player->position(), is_facing_left);
    double y_intersection = FindFirstVIntersectionY(ray_degree, player->position(), x_intersection);

    if (is_facing_left)
        x_intersection -= 1.0;

    int mapX = floor(x_intersection / double(Config::SPRITE_SIZE));
    int mapY = floor(y_intersection / double(Config::SPRITE_SIZE));

    const RayCollision *collision = map_->GetRayCollisionAt(mapX, mapY, false, player->position(), y_intersection, ray_degree);

    if (collision != NULL)
    {
        return collision;
    }

    // Subsequent intersections
    while (true)
    {
        x_intersection += FindNextVIntersectionX(is_facing_left);
        y_intersection += FindNextVIntersectionY(ray_degree);

        int mapX = floor(x_intersection / double(Config::SPRITE_SIZE));
        int mapY = floor(y_intersection / double(Config::SPRITE_SIZE));

        collision = map_->GetRayCollisionAt(mapX, mapY, false, player->position(), y_intersection, ray_degree);

        if (collision != NULL)
        {
            return collision;
        }

        if (RayIsOutsideViewArea(player->view_area(), x_intersection, y_intersection))
        {
            return NULL;
        }
    }
}

double RayCaster::FindFirstVIntersectionX(const SDL_Point *player_position,
                                          const bool &is_facing_left) const
{
    double intersection = floor(player_position->x / double(Config::SPRITE_SIZE)) * Config::SPRITE_SIZE;
    if (!is_facing_left)
        intersection += Config::SPRITE_SIZE;
    return intersection;
}

double RayCaster::FindFirstVIntersectionY(const int &ray_degree,
                                          const SDL_Point *player_position,
                                          const double &x_intersection) const
{
    return (Trigonometry::fTanTable[ray_degree] * (x_intersection - player_position->x)) + player_position->y;
}

double RayCaster::FindNextVIntersectionX(const bool &is_facing_left) const
{
    return (is_facing_left) ? double(-Config::SPRITE_SIZE) : double(Config::SPRITE_SIZE);
}

double RayCaster::FindNextVIntersectionY(const int &ray_degree) const
{
    return Trigonometry::fYStepTable[ray_degree];
}

bool RayCaster::RayIsOutsideViewArea(const SDL_Rect *player_view_area,
                                     const double &x_intersection,
                                     const double &y_intersection) const
{
    // If ray has gone out of view area, stop casting it
    return (y_intersection < player_view_area->y || y_intersection > player_view_area->h || x_intersection < player_view_area->x || x_intersection > player_view_area->w);
}

// Public methods

void RayCaster::Cast(const Player *player) const
{
    // Start casting rays from left to right of FOV
    int ray_degree = player->rotation() - Config::ANGLE30;
    if (ray_degree < 0)
        ray_degree += Config::ANGLE360;

    for (int ray_index = 0; ray_index != Config::game_width; ++ray_index)
    {
        CastSingleRay(player, ray_degree, ray_index);
        ++ray_degree;
        if (ray_degree >= Config::ANGLE360)
        {
            ray_degree -= Config::ANGLE360;
        }
    }
}