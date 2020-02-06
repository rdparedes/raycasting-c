#include "config.hpp"
#include "trigonometry.hpp"
#include "player.hpp"

Player::Player(const int &x, const int &y) : _rotation(Config::ANGLE0)
{
    move(x, y);
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
    int x = position()->x - (_viewDistance / 2 * Config::SPRITE_SIZE);
    int y = position()->y - (_viewDistance / 2 * Config::SPRITE_SIZE);
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

/**
 *  Returns _rotation minus ANGLE90 to account for mismatch between the sprite's rotation (90 deg) versus the player class initial rotation (ANGLE0)
 */
const int Player::spriteRotation() const
{
    int temp = _rotation + Config::ANGLE90;
    if (temp > Config::ANGLE360)
    {
        temp -= Config::ANGLE360;
    }
    return temp;
}

const SDL_Point *Player::position() const { return &_position; }

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

void Player::move(const int &x, const int &y)
{
    _position = { x, y };
    _spriteRectangle = {position()->x,
                        position()->y,
                        Config::SPRITE_SIZE,
                        Config::SPRITE_SIZE};
}

void Player::moveForward()
{
    move(position()->x + int(std::round(xDirection() * speed)), position()->y + int(std::round(yDirection() * speed)));
    calculateViewArea();
}

void Player::moveBackwards()
{
    move(position()->x - int(std::round(xDirection() * speed)), position()->y - int(std::round(yDirection() * speed)));
    calculateViewArea();
}