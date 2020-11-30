#include "config.hpp"
#include "trigonometry.hpp"
#include "player.hpp"
#include "sdlUtils.hpp"

void Player::init(SDL_Renderer *renderer, const int &initialX, const int &initialY)
{
    SDL_Surface *surfaceLoader = IMG_Load("sprites/player-marker.png");
    _markerTexture = SDL_CreateTextureFromSurface(renderer, surfaceLoader);
    SDL_FreeSurface(surfaceLoader);
    _markerSpriteSize = getsize(_markerTexture);
    _markerSpriteOffsetX = int(_markerSpriteSize.x / 2);
    _markerSpriteOffsetY = int(_markerSpriteSize.y / 2);
    _rotation = Config::ANGLE0;
    teleport(initialX, initialY);
    calculateViewArea();
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


void Player::updateMarkerRect()
{
    _markerRect = {position()->x - _markerSpriteOffsetX,
                   position()->y - _markerSpriteOffsetY,
                   _markerSpriteSize.x,
                   _markerSpriteSize.y};
}

// Public methods

const int &Player::rotation() const { return _rotation; }

/**
 *  Returns _rotation minus ANGLE90 to account for mismatch between the sprite's rotation (90 deg) versus the player class initial rotation (ANGLE0)
 */
const int Player::spriteRotation() const
{
    int temp = _rotation;
    if (temp > Config::ANGLE360)
    {
        temp -= Config::ANGLE360;
    }
    return temp;
}

const SDL_Point *Player::position() const { return &_position; }

const SDL_Rect *Player::markerRect() const { return &_markerRect; }

const SDL_Rect *Player::viewArea() const { return &_viewArea; }

SDL_Texture *Player::markerTexture() const { return _markerTexture; }

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

/**
 *  \brief Teleport the player to x, y.
 */
void Player::teleport(const int &x, const int &y)
{
    _position = {x, y};
    updateMarkerRect();
}

/**
 *  \brief Move the player along directions x, y. If the body collides with another,
 *  it will slide along the other body rather than stopping.
 */
void Player::moveAndSlide(const int &x, const int &y)
{
    _position = {position()->x + x, position()->y + y};
    updateMarkerRect();
}

void Player::moveForward()
{
    moveAndSlide(int(std::round(xDirection() * speed)), int(std::round(yDirection() * speed)));
    calculateViewArea();
}

void Player::moveBackwards()
{
    moveAndSlide(-int(std::round(xDirection() * speed)), -int(std::round(yDirection() * speed)));
    calculateViewArea();
}