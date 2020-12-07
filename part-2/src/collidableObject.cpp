#include "collidableObject.hpp"

void CollidableObject::Init(SDL_Texture * texture, const SDL_Rect &collision_box)
{
    texture_ = texture;
    collision_box_ = collision_box;
}

SDL_Texture *CollidableObject::texture() const { return texture_; }

const SDL_Rect *CollidableObject::collision_box() const { return &collision_box_; }

void CollidableObject::set_collision_box(const SDL_Rect & collision_box)
{
    collision_box_ = collision_box;
}
