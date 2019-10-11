#ifndef GUARD_player_hpp
#define GUARD_player_hpp

class Player
{
    int _rotation = Config::ANGLE0;
    SDL_Rect spriteRectangle;

    const double &xDirection() const;
    const double &yDirection() const;

public:
    const int speed = 6;

    Player(const int &, const int &);
    const int &rotation() const;
    const int spriteRotation() const;
    const SDL_Rect *rectangle() const;
    void rotateRight();
    void rotateLeft();
    void moveForward();
    void moveBackwards();
};

#endif