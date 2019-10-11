#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "config.hpp"
#include "trigonometry.hpp"
#include "player.hpp"

const double PROJECTION_TO_360_RATIO = Config::ANGLE360 / double(360);

SDL_Window *window;
SDL_Renderer *renderer;

bool initEverything();
bool initSDL();
bool createWindow();
bool createRenderer();
void setupRenderer();

void runGame();
void render();
void close();

const SDL_Point playerCenter = {32, 32};
SDL_Texture *playerTexture;
SDL_Surface *surfaceLoader;

Player *player;

int main(int argc, char *argv[])
{
    if (!initEverything())
        return -1;

    surfaceLoader = IMG_Load("sprites/player.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, surfaceLoader);

    SDL_FreeSurface(surfaceLoader);

    player = new Player(20, 20);

    runGame();

    close();
}


void render()
{
    // Clear the window and make it all green
    SDL_RenderClear(renderer);

    double playerAngle = floor(player->spriteRotation() / PROJECTION_TO_360_RATIO);

    SDL_RenderCopyEx(
        renderer, playerTexture, NULL, player->rectangle(), playerAngle, &playerCenter, SDL_FLIP_NONE);

    // Render changes
    SDL_RenderPresent(renderer);
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
        Config::screenSizeX,
        Config::screenSizeY,
        0
    );

    if (window == nullptr)
    {
        std::cout << "Failed to create window: " << SDL_GetError();
        return false;
    }

    return true;
}

bool createRenderer()
{
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == nullptr)
    {
        std::cout << "Failed to create renderer: " << SDL_GetError();
        return false;
    }

    return true;
}

void setupRenderer()
{
    SDL_RenderSetLogicalSize(renderer, Config::screenSizeX, Config::screenSizeY);
    // Set renderer color to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void runGame()
{
    bool gameLoop = true;

    while (gameLoop)
    {
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_UP])
        {
            player->moveForward();
        }
        if (keys[SDL_SCANCODE_RIGHT])
        {
            player->rotateRight();
        }
        if (keys[SDL_SCANCODE_DOWN])
        {
            player->moveBackwards();
        }
        if (keys[SDL_SCANCODE_LEFT])
        {
            player->rotateLeft();
        }

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                gameLoop = false;
        }

        render();

        SDL_Delay(16);
    }
}

void close()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}