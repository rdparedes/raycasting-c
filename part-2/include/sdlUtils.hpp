#ifndef GUARD_sdlUtils_hpp
#define GUARD_sdlUtils_hpp

#include <SDL2/SDL.h>

SDL_Point getsize(SDL_Texture *texture)
{
    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return size;
}

#endif
