#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "trigonometry.hpp"
#include "config.hpp"
#include "player.hpp"
#include "level.hpp"
#include "minimap.hpp"
#include "raycaster.hpp"
#include "map.hpp"
#include "collidableObject.hpp"
#include "renderer.hpp"
#include "filesystem"

SDL_Window *window;
SDL_Renderer *sdl_renderer;
SDL_Rect game_viewport;
SDL_Rect minimap_viewport;

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
Minimap *minimap;

int main(int argc, char *argv[])
{
    if (!initEverything())
        return -1;

    sample_map = new Map();
    level = new Level();
    renderer = new Renderer();
    ray_caster = new RayCaster();
    minimap = new Minimap();

    game_viewport.x = 0;
    game_viewport.y = 0;
    game_viewport.w = Config::game_width;
    game_viewport.h = Config::game_height;
    minimap_viewport.x = Config::game_width + 16;
    minimap_viewport.y = int(Config::y_center - (Config::minimap_height / 2));
    minimap_viewport.w = Config::minimap_width;
    minimap_viewport.h = Config::minimap_height;

    wall = new CollidableObject();
    const char* wall_image_src = "../sprites/bricks.png";
    SDL_Surface *wall_surface_loader = IMG_Load(wall_image_src);
    SDL_Texture *wall_texture = SDL_CreateTextureFromSurface(sdl_renderer, wall_surface_loader);
    wall->Init(wall_texture, { 0, 0, 0, 0});
    SDL_FreeSurface(wall_surface_loader);
    sample_map->Init({
                        {'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'},
                        {'w', '.', '.', '.', '.', '.', 'w', '.', 'w', '.', '.', '.', 'w'},
                        {'w', '.', '.', '.', '.', '.', 'w', 'w', 'w', '.', 'w', '.', 'w'},
                        {'w', '.', 'w', 'w', 'w', '.', '.', 'w', 'w', '.', 'w', '.', 'w'},
                        {'w', '.', 'w', '.', 'w', '.', '.', 'w', 'w', '.', 'w', '.', 'w'},
                        {'w', '.', 'w', '.', 'w', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', '.', '.', '.', 'w', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', '.', 'w', '.', 'w', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', '.', 'w', '.', '.', '.', 'w', '.', 'w', '.', 'w', '.', 'w'},
                        {'w', '.', 'w', '.', '.', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', '.', '.', '.', 'w', '.', 'w', '.', '.', 'w', 'w', '.', 'w'},
                        {'w', 'w', 'w', '.', 'w', '.', 'w', 'w', 'w', '.', 'w', '.', 'w'},
                        {'w', '.', '.', '.', 'w', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', '.', '.', '.', 'w', '.', '.', '.', '.', '.', 'w', '.', 'w'},
                        {'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', '.', 'w'},
                    },
                    {{'w', wall}});
    ray_caster->Init(sample_map, sdl_renderer);
    renderer->Init(ray_caster, sdl_renderer);
    level->Init(sdl_renderer, renderer, sample_map);
    minimap->Init(sdl_renderer, renderer, level->player(), sample_map);

    runGame();

    close();
}

void render()
{
    // Clear the window
    SDL_RenderClear(sdl_renderer);

    SDL_RenderSetViewport(sdl_renderer, &game_viewport);
    level->render();
    SDL_RenderSetViewport(sdl_renderer, &minimap_viewport);
    minimap->render();

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
        Config::window_width,
        Config::window_height,
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
    SDL_RenderSetLogicalSize(sdl_renderer, Config::window_width, Config::window_height);
    // Set renderer color to black
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
}

void runGame()
{
    bool gameLoop = true;

    while (gameLoop)
    {
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W])
        {
            level->player()->MoveForward();
        }
        if (keys[SDL_SCANCODE_D])
        {
            level->player()->RotateRight();
        }
        if (keys[SDL_SCANCODE_S])
        {
            level->player()->MoveBackwards();
        }
        if (keys[SDL_SCANCODE_A])
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
                    // Do things with the tab button
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