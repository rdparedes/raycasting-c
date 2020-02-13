#ifndef GUARD_player_hpp
#define GUARD_player_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Player
{
    int _rotation;
    const int _viewDistance = 20;

    SDL_Point _position;
    SDL_Rect _spriteRectangle;
    SDL_Rect _viewArea;
    SDL_Texture *_texture;

    void calculateViewArea();
    const double &xDirection() const;
    const double &yDirection() const;

public:
    const int speed = 6;
    const SDL_Point center = {32, 32};

    void init(SDL_Renderer *, const int &, const int &);
    const int &rotation() const;
    const int spriteRotation() const;
    const SDL_Point *position() const;
    const SDL_Rect *rectangle() const;
    const SDL_Rect *viewArea() const;
    SDL_Texture *texture() const;
    void rotateRight();
    void rotateLeft();
    void move(const int &, const int &);
    void moveForward();
    void moveBackwards();
};

#endif