#include <SDL2/SDL.h>

#include "config.hpp"
#include "trigonometry.hpp"
#include "player.hpp"

Player::Player(const int &x, const int &y)
{
    spriteRectangle.x = x;
    spriteRectangle.y = y;
    spriteRectangle.w = Config::SPRITE_SIZE;
    spriteRectangle.h = Config::SPRITE_SIZE;
    _rotation = Config::ANGLE0;
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

// Public methods

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

const SDL_Rect *Player::rectangle() const { return &spriteRectangle; }

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
    spriteRectangle.x += int(std::round(xDirection() * speed));
    spriteRectangle.y += int(std::round(yDirection() * speed));
}

void Player::moveBackwards()
{
    spriteRectangle.x -= int(std::round(xDirection() * speed));
    spriteRectangle.y -= int(std::round(yDirection() * speed));
}