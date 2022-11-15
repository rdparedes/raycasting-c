#ifndef GUARD_player_hpp
#define GUARD_player_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "map.hpp"

class Player
{
    int rotation_;

    // Centers the marker sprite (x coordinate) when it's rendered
    int minimap_sprite_offset_x_;
    // Centers the marker sprite (y coordinate) when it's rendered
    int minimap_sprite_offset_y_;

    // holds the position of the player
    SDL_Point position_;

    SDL_Rect minimap_sprite_rectangle_;
    SDL_Rect view_area_;
    SDL_Texture *marker_texture_;
    SDL_Point minimap_sprite_size_;
    const Map *current_map_;

    void CalculateViewArea();
    void UpdateMinimapPosition();
    const double &x_direction() const;
    const double &y_direction() const;

public:
    const int kViewDistance = 25;
    const int kSpeed = 6;
    const int kHeight = 32;
    const SDL_Point markerCenter = {8, 8};

    void Init(const Map *, SDL_Renderer *, const int &, const int &);

    const int &rotation() const;
    const int sprite_rotation() const;
    const SDL_Point *position() const;
    const SDL_Rect *collision_box() const;
    const SDL_Rect *minimap_rectangle() const;
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