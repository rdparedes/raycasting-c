#ifndef GUARD_collidableObject_hpp
#define GUARD_collidableObject_hpp

#include <SDL.h>
#include <SDL_image.h>

class CollidableObject
{
    SDL_Renderer *renderer_;
    SDL_Texture *texture_;
    SDL_Rect collision_box_;

public:
    void Init(SDL_Renderer *, const char *, const SDL_Rect &);
    SDL_Texture * texture() const;
    const SDL_Rect * collision_box() const;
};
#endif