#include "solidObject.hpp"

void SolidObject::init(SDL_Renderer *renderer, const char *imageSrc)
{
    _renderer = renderer;
    SDL_Surface *surfaceLoader = IMG_Load(imageSrc);
    _texture = SDL_CreateTextureFromSurface(_renderer, surfaceLoader);
    SDL_FreeSurface(surfaceLoader);
}

SDL_Texture *SolidObject::texture() const { return _texture; }
