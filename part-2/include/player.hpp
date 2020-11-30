#ifndef GUARD_player_hpp
#define GUARD_player_hpp

#include <SDL.h>
#include <SDL_image.h>

class Player
{
    int _rotation;
    const int _viewDistance = 20;

    // Values to center the marker sprite when it's rendered
    int _markerSpriteOffsetX;
    int _markerSpriteOffsetY;

    // holds the position of the player
    SDL_Point _position;
    SDL_Rect _markerRect;
    SDL_Rect _viewArea;
    SDL_Texture *_markerTexture;
    SDL_Point _markerSpriteSize;

    void calculateViewArea();
    void updateMarkerRect();
    const double &xDirection() const;
    const double &yDirection() const;

public:
    const int speed = 6;
    const SDL_Point markerCenter = {8, 8};

    void init(SDL_Renderer *, const int &, const int &);
    const int &rotation() const;
    const int spriteRotation() const;
    const SDL_Point *position() const;
    const SDL_Rect *markerRect() const;
    const SDL_Rect *markerRenderingRect() const;
    const SDL_Rect *viewArea() const;
    SDL_Texture *markerTexture() const;
    void rotateRight();
    void rotateLeft();
    void teleport(const int &, const int &);
    void moveAndSlide(const int &, const int &);
    void moveForward();
    void moveBackwards();
};

#endif