#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "trigonometry.hpp"
#include "config.hpp"
#include "player.hpp"
#include "level.hpp"
#include "raycaster.hpp"
#include "map.hpp"
#include "collidableObject.hpp"
#include "renderer.hpp"

SDL_Window *window;
SDL_Renderer *sdl_renderer;

bool initEverything();
bool initSDL();
bool createWindow();
bool createRenderer();
void setupRenderer();

void runGame();
void render();
void close();

SDL_Texture *player_texture;
SDL_Surface *wall_surface_loader;

CollidableObject *wall;
Map *sample_map;
Level *level;
Renderer *renderer;
RayCaster *ray_caster;

int main(int argc, char *argv[])
{
    if (!initEverything())
        return -1;

    sample_map = new Map();
    level = new Level();
    renderer = new Renderer();
    ray_caster = new RayCaster();

    wall = new CollidableObject();
    const char *wall_image_src = "sprites/bricks.png";
    SDL_Surface *wall_surface_loader = IMG_Load(wall_image_src);
    SDL_Texture *wall_texture = SDL_CreateTextureFromSurface(sdl_renderer, wall_surface_loader);
    wall->Init(wall_texture, { 0, 0, 0, 0}); // TODO
    SDL_FreeSurface(wall_surface_loader);

    sample_map->Init({
                        {'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'},
                        {'w', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', '.', 'w', 'w', 'w', '.', 'w', '.', 'w'},
                        {'w', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', '.', '.', '.', '.', '.', '.', '.', 'w'},
                        {'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'},
                    },
                    {{'w', wall}});
    ray_caster->Init(sample_map, sdl_renderer);
    renderer->Init(ray_caster, sdl_renderer);
    level->Init(sdl_renderer, renderer, sample_map);

    runGame();

    close();
}

void render()
{
    // Clear the window
    SDL_RenderClear(sdl_renderer);

    level->render();

    // Render changes
    SDL_RenderPresent(sdl_renderer);
}

bool initEverything()
{
    if (!initSDL())
    {
        return false;
    }
    if (!createWindow())
    {
        return false;
    }
    if (!createRenderer())
    {
        return false;
    }
    if (!Trigonometry::load())
    {
        return false;
    }

    setupRenderer();

    return true;
}

bool initSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool createWindow()
{
    window = SDL_CreateWindow(
        "Raycasting",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        Config::screen_width,
        Config::screen_height,
        0);

    if (window == nullptr)
    {
        std::cout << "Failed to create window: " << SDL_GetError();
        return false;
    }

    return true;
}

bool createRenderer()
{
    sdl_renderer = SDL_CreateRenderer(window, -1, 0);

    if (sdl_renderer == nullptr)
    {
        std::cout << "Failed to create renderer: " << SDL_GetError();
        return false;
    }

    return true;
}

void setupRenderer()
{
    SDL_RenderSetLogicalSize(sdl_renderer, Config::screen_width, Config::screen_height);
    // Set renderer color to black
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
}

void runGame()
{
    bool gameLoop = true;

    while (gameLoop)
    {
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_UP])
        {
            level->player()->MoveForward();
        }
        if (keys[SDL_SCANCODE_RIGHT])
        {
            level->player()->RotateRight();
        }
        if (keys[SDL_SCANCODE_DOWN])
        {
            level->player()->MoveBackwards();
        }
        if (keys[SDL_SCANCODE_LEFT])
        {
            level->player()->RotateLeft();
        }

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                gameLoop = false;
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_TAB)
                {
                    level->should_render_minimap = !level->should_render_minimap;
                }
            }
        }

        render();

        SDL_Delay(16);
    }
}

void close()
{
    SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}