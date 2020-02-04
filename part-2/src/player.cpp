#include "config.hpp"
#include "trigonometry.hpp"
#include "player.hpp"

Player::Player(const int &x, const int &y) : _rotation(Config::ANGLE0)
{
    _spriteRectangle = {x,
                        y,
                        Config::SPRITE_SIZE,
                        Config::SPRITE_SIZE};
}

// Private methods

const double &Player::xDirection() const
{
    return Trigonometry::fCosTable[_rotation];
}

const double &Player::yDirection() const
{
    return Trigonometry::fSinTable[_rotation];
}

void Player::calculateViewArea()
{
    int x = _spriteRectangle.x - (_viewDistance / 2 * Config::SPRITE_SIZE);
    int y = _spriteRectangle.y - (_viewDistance / 2 * Config::SPRITE_SIZE);
    int w = x + (_viewDistance * Config::SPRITE_SIZE);
    int h = y + (_viewDistance * Config::SPRITE_SIZE);
    _viewArea = {x, y, w, h};
}

// Public methods

void Player::init(SDL_Renderer *renderer)
{
    SDL_Surface *surfaceLoader = IMG_Load("sprites/player.png");
    _texture = SDL_CreateTextureFromSurface(renderer, surfaceLoader);
    SDL_FreeSurface(surfaceLoader);
    calculateViewArea();
}

const int &Player::rotation() const { return _rotation; }

const int Player::spriteRotation() const
{
    int temp = _rotation + Config::ANGLE90;
    if (temp > Config::ANGLE360)
    {
        temp -= Config::ANGLE360;
    }
    return temp;
}

const SDL_Rect *Player::rectangle() const { return &_spriteRectangle; }

const SDL_Rect *Player::viewArea() const { return &_viewArea; }

SDL_Texture *Player::texture() const { return _texture; }

void Player::rotateRight()
{
    _rotation += Config::ANGLE5;
    if (_rotation >= Config::ANGLE360)
    {
        _rotation -= Config::ANGLE360;
    }
}

void Player::rotateLeft()
{
    _rotation -= Config::ANGLE5;
    if (_rotation < Config::ANGLE0)
    {
        _rotation += Config::ANGLE360;
    }
}

void Player::moveForward()
{
    _spriteRectangle.x += int(std::round(xDirection() * speed));
    _spriteRectangle.y += int(std::round(yDirection() * speed));
    calculateViewArea();
}

void Player::moveBackwards()
{
    _spriteRectangle.x -= int(std::round(xDirection() * speed));
    _spriteRectangle.y -= int(std::round(yDirection() * speed));
    calculateViewArea();
}