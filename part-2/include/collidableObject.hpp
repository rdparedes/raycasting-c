#ifndef GUARD_collidableObject_hpp
#define GUARD_collidableObject_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class CollidableObject
{
    SDL_Texture *texture_;
    SDL_Rect collision_box_;

public:
    void Init(SDL_Texture *, const SDL_Rect &);
    SDL_Texture * texture() const;
    const SDL_Rect * collision_box() const;
    void set_collision_box(const SDL_Rect &);
};
#endif