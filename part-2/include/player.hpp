#ifndef GUARD_player_hpp
#define GUARD_player_hpp

#include <SDL.h>
#include <SDL_image.h>
#include "map.hpp"

class Player
{
    const int kViewDistance = 20;
    const int kSpeed = 6;
    int rotation_;

    // Values to center the marker sprite when it's rendered
    int marker_sprite_offset_x_;
    int marker_sprite_offset_y_;

    // holds the position of the player
    SDL_Point position_;

    SDL_Rect sprite_rectangle_;
    SDL_Rect view_area_;
    SDL_Texture *marker_texture_;
    SDL_Point marker_sprite_size_;
    const Map *current_map_;

    void CalculateViewArea();
    void UpdateMarkerRect();
    const double &x_direction() const;
    const double &y_direction() const;

public:
    const SDL_Point markerCenter = {8, 8};

    void Init(const Map *, SDL_Renderer *, const int &, const int &);

    const int &rotation() const;
    const int sprite_rotation() const;
    const SDL_Point *position() const;
    const SDL_Rect *collision_box() const;
    const SDL_Rect *sprite_rectangle() const;
    const SDL_Rect *marker_rendering_rect() const;
    const SDL_Rect *view_area() const;
    SDL_Texture *marker_texture() const;

    void RotateRight();
    void RotateLeft();
    void Teleport(const int &, const int &);
    void MoveAndSlide(const int &, const int &);
    void MoveForward();
    void MoveBackwards();
};

#endif