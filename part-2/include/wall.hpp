#ifndef GUARD_wall_hpp
#define GUARD_wall_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Wall
{
    SDL_Renderer *_renderer;
    SDL_Texture *_texture;

public:
    void init(SDL_Renderer *);
    SDL_Texture * texture() const;
};

#endif