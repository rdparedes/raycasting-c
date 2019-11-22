#ifndef GUARD_player_hpp
#define GUARD_player_hpp

class Player
{
    int _rotation;
    const int _viewDistance = 20;

    SDL_Renderer *_renderer;
    SDL_Rect _spriteRectangle;
    SDL_Rect _viewArea;
    SDL_Texture *_texture;

    void calculateViewArea();
    const double &xDirection() const;
    const double &yDirection() const;

public:
    const int speed = 6;
    const SDL_Point center = {32, 32};

    Player(const int &, const int &);
    void init(SDL_Renderer *);
    const int &rotation() const;
    const int spriteRotation() const;
    const SDL_Rect *rectangle() const;
    const SDL_Rect *viewArea() const;
    SDL_Texture *texture() const;
    void rotateRight();
    void rotateLeft();
    void moveForward();
    void moveBackwards();
};

#endif