#include "collidableObject.hpp"

void CollidableObject::Init(SDL_Renderer *renderer, const char *imageSrc, const SDL_Rect &collision_box)
{
    renderer_ = renderer;
    SDL_Surface *surfaceLoader = IMG_Load(imageSrc);
    texture_ = SDL_CreateTextureFromSurface(renderer_, surfaceLoader);
    SDL_FreeSurface(surfaceLoader);
    collision_box_ = collision_box;
}

SDL_Texture *CollidableObject::texture() const { return texture_; }

const SDL_Rect *CollidableObject::collision_box() const { return &collision_box_; }
