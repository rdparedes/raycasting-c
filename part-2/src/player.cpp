#include "config.hpp"
#include "trigonometry.hpp"
#include "player.hpp"
#include "sdlUtils.hpp"

void Player::Init(const Map *map, SDL_Renderer *renderer, const int &initialX, const int &initialY)
{
    SDL_Surface *surfaceLoader = IMG_Load("sprites/player-marker.png");
    marker_texture_ = SDL_CreateTextureFromSurface(renderer, surfaceLoader);
    SDL_FreeSurface(surfaceLoader);
    marker_sprite_size_ = getsize(marker_texture_);
    marker_sprite_offset_x_ = int(marker_sprite_size_.x / 2);
    marker_sprite_offset_y_ = int(marker_sprite_size_.y / 2);
    rotation_ = Config::ANGLE0;
    current_map_ = map;
    Teleport(initialX, initialY);
    CalculateViewArea();
}

// Private methods

const double &Player::x_direction() const
{
    return Trigonometry::fCosTable[rotation_];
}

const double &Player::y_direction() const
{
    return Trigonometry::fSinTable[rotation_];
}

void Player::CalculateViewArea()
{
    int x = position()->x - (k_view_distance_ / 2 * Config::SPRITE_SIZE);
    int y = position()->y - (k_view_distance_ / 2 * Config::SPRITE_SIZE);
    int w = x + (k_view_distance_ * Config::SPRITE_SIZE);
    int h = y + (k_view_distance_ * Config::SPRITE_SIZE);
    view_area_ = {x, y, w, h};
}

void Player::UpdateMarkerRect()
{
    sprite_rectangle_ = {position()->x - marker_sprite_offset_x_,
                         position()->y - marker_sprite_offset_y_,
                         marker_sprite_size_.x,
                         marker_sprite_size_.y};
}

// Public methods

const int &Player::rotation() const { return rotation_; }

const SDL_Rect *Player::collision_box() const { return &sprite_rectangle_; }

/**
 *  Returns rotation_ minus ANGLE90 to account for mismatch between the sprite's rotation (90 deg) versus the player class initial rotation (ANGLE0)
 */
const int Player::sprite_rotation() const
{
    int temp = rotation_;
    if (temp > Config::ANGLE360)
    {
        temp -= Config::ANGLE360;
    }
    return temp;
}

const SDL_Point *Player::position() const { return &position_; }

const SDL_Rect *Player::sprite_rectangle() const { return &sprite_rectangle_; }

const SDL_Rect *Player::view_area() const { return &view_area_; }

SDL_Texture *Player::marker_texture() const { return marker_texture_; }

void Player::RotateRight()
{
    rotation_ += Config::ANGLE5;
    if (rotation_ >= Config::ANGLE360)
    {
        rotation_ -= Config::ANGLE360;
    }
}

void Player::RotateLeft()
{
    rotation_ -= Config::ANGLE5;
    if (rotation_ < Config::ANGLE0)
    {
        rotation_ += Config::ANGLE360;
    }
}

/**
 *  \brief Teleport the player to x, y.
 */
void Player::Teleport(const int &x, const int &y)
{
    position_ = {x, y};
    UpdateMarkerRect();
}

/**
 *  \brief Move the player to x, y. If the body collides, it will slide along the other body rather than stopping.
 */
void Player::MoveAndSlide(const int &x, const int &y)
{
    int new_x = position()->x + x;
    int new_y = position()->y + y;
    SDL_Rect cb_new_x = {collision_box()->x + x, collision_box()->y, collision_box()->w, collision_box()->h};
    SDL_Rect cb_new_y = {collision_box()->x, collision_box()->y + y, collision_box()->w, collision_box()->h};

    if (current_map_->CollisionExists(cb_new_x))
    {
        new_x = position()->x;
    }
    if (current_map_->CollisionExists(cb_new_y))
    {
        new_y = position()->y;
    }
    position_ = {new_x, new_y};
    UpdateMarkerRect();
}

void Player::MoveForward()
{
    MoveAndSlide(int(std::round(x_direction() * k_speed_)), int(std::round(y_direction() * k_speed_)));
    CalculateViewArea();
}

void Player::MoveBackwards()
{
    MoveAndSlide(-int(std::round(x_direction() * k_speed_)), -int(std::round(y_direction() * k_speed_)));
    CalculateViewArea();
}