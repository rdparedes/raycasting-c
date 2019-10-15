#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "config.hpp"
#include "trigonometry.hpp"
#include "player.hpp"

Player::Player(const int &x, const int &y) : _rotation(Config::ANGLE0)
{
    _spriteRectangle.x = x;
    _spriteRectangle.y = y;
    _spriteRectangle.w = Config::SPRITE_SIZE;
    _spriteRectangle.h = Config::SPRITE_SIZE;
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

void Player::init(SDL_Renderer *renderer)
{
    _renderer = renderer;
    SDL_Surface *surfaceLoader = IMG_Load("sprites/player.png");
    _texture = SDL_CreateTextureFromSurface(_renderer, surfaceLoader);
    SDL_FreeSurface(surfaceLoader);
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
}

void Player::moveBackwards()
{
    _spriteRectangle.x -= int(std::round(xDirection() * speed));
    _spriteRectangle.y -= int(std::round(yDirection() * speed));
}