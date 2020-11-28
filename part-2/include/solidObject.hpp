#ifndef GUARD_solidObject_hpp
#define GUARD_solidObject_hpp

#include <SDL.h>
#include <SDL_image.h>

class SolidObject
{
    SDL_Renderer *_renderer;
    SDL_Texture *_texture;

public:
    void init(SDL_Renderer *, const char *);
    SDL_Texture * texture() const;
};
#endif