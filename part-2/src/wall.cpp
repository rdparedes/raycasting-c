#include "wall.hpp"

void Wall::init(SDL_Renderer *renderer)
{
  _renderer = renderer;
  SDL_Surface *surfaceLoader = IMG_Load("sprites/bricks.png");
  _texture = SDL_CreateTextureFromSurface(_renderer, surfaceLoader);
  SDL_FreeSurface(surfaceLoader);
}

SDL_Texture *Wall::texture() const { return _texture; }
