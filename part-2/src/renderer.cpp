#include "renderer.hpp"

void Renderer::Init(RayCaster *ray_caster, SDL_Renderer *sdl_renderer)
{
    ray_caster_ = ray_caster;
    sdl_renderer_ = sdl_renderer;
}

void Renderer::DrawBackground()
{
    int color_variant = 255;

    for (int y = 0; y < (Config::screen_height * 0.5); ++y)
    {
        SDL_SetRenderDrawColor(sdl_renderer_, color_variant, 125, 255, 1);
        SDL_RenderDrawLine(sdl_renderer_, 0, y, Config::screen_width, y);
        --color_variant;
    }

    SDL_SetRenderDrawColor(sdl_renderer_, 99, 99, 99, 1);
    for (int y = (Config::screen_height * 0.5); y < Config::screen_height; ++y)
    {
        SDL_RenderDrawLine(sdl_renderer_, 0, y, Config::screen_width, y);
    }

    SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, 0);
}

void Renderer::CallRayCaster(const Player *player)
{
    ray_caster_->Cast(player);
}
