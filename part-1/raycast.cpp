#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const unsigned int sizeX = 640;
const unsigned int sizeY = 400;
const unsigned int SPRITE_SIZE = 64;

const int ANGLE60 = sizeX;
const int ANGLE30 = floor(ANGLE60 / 2);
const int ANGLE15 = floor(ANGLE30 / 2);
const int ANGLE90 = ANGLE30 * 3;
const int ANGLE180 = ANGLE90 * 2;
const int ANGLE270 = ANGLE90 * 3;
const int ANGLE360 = ANGLE60 * 6;
const int ANGLE0 = 0;
const int ANGLE5 = floor(ANGLE30 / 6);
const int ANGLE10 = ANGLE5 * 2;
const int ANGLE45 = ANGLE15 * 3;

const double PROJECTION_TO_360_RATIO = ANGLE360 / double(360);
const double PI = std::atan(1.0) * 4;

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
double arcToRad(int);

const SDL_Point playerCenter = {32, 32};
SDL_Texture *playerTexture;
SDL_Surface *surfaceLoader;

// Trigonometic tables with pre-calculated values
double fSinTable[ANGLE360 + 1];
double fISinTable[ANGLE360 + 1];
double fCosTable[ANGLE360 + 1];
double fICosTable[ANGLE360 + 1];
double fTanTable[ANGLE360 + 1];
double fITanTable[ANGLE360 + 1];
double fXStepTable[ANGLE360 + 1];
double fYStepTable[ANGLE360 + 1];

class Player
{
    int rotation_;
    SDL_Rect spriteRectangle;

    const double &x_direction() const
    {
        return fCosTable[rotation_];
    }

    const double &y_direction() const
    {
        return fSinTable[rotation_];
    }

public:
    Player(const int &x, const int &y)
    {
        spriteRectangle.x = x;
        spriteRectangle.y = y;
        spriteRectangle.w = SPRITE_SIZE;
        spriteRectangle.h = SPRITE_SIZE;
        rotation_ = ANGLE0;
    }

    const int speed = 6;

    const int &rotation() const { return rotation_; }

    const int sprite_rotation() const
    {
        int temp = rotation_ + ANGLE90;
        if (temp > ANGLE360)
        {
            temp -= ANGLE360;
        }
        return temp;
    }

    const SDL_Rect *rectangle() const { return &spriteRectangle; }

    void RotateRight()
    {
        rotation_ += ANGLE5;
        if (rotation_ >= ANGLE360)
        {
            rotation_ -= ANGLE360;
        }
    }

    void RotateLeft()
    {
        rotation_ -= ANGLE5;
        if (rotation_ < ANGLE0)
        {
            rotation_ += ANGLE360;
        }
    }

    void MoveForward()
    {
        spriteRectangle.x += int(std::round(x_direction() * speed));
        spriteRectangle.y += int(std::round(y_direction() * speed));
    }

    void MoveBackwards()
    {
        spriteRectangle.x -= int(std::round(x_direction() * speed));
        spriteRectangle.y -= int(std::round(y_direction() * speed));
    }
};

Player *player;

int main(int argc, char *argv[])
{
    // Initialize trigonometric tables
    for (int i = 0; i != (ANGLE360 + 1); ++i)
    {
        double radian = arcToRad(i) + (0.0001); // Add 0.0001 to avoid division by zero
        fSinTable[i] = std::sin(radian);
        fISinTable[i] = 1.0 / fSinTable[i];
        fCosTable[i] = std::cos(radian);
        fICosTable[i] = 1.0 / fCosTable[i];
        fTanTable[i] = std::tan(radian);
        fITanTable[i] = 1.0 / fTanTable[i];
    }

    if (!initEverything())
        return -1;

    surfaceLoader = IMG_Load("sprites/player.png");
    playerTexture = SDL_CreateTextureFromSurface(renderer, surfaceLoader);

    SDL_FreeSurface(surfaceLoader);

    player = new Player(20, 20);

    runGame();

    close();
}

double arcToRad(int angle)
{
    return ((double(angle) * PI) / ANGLE180);
}

void render()
{
    // Clear the window and make it all green
    SDL_RenderClear(renderer);

    double playerAngle = floor(player->sprite_rotation() / PROJECTION_TO_360_RATIO);

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
    window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sizeX, sizeY, 0);

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
    SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);
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
            player->MoveForward();
        }
        if (keys[SDL_SCANCODE_RIGHT])
        {
            player->RotateRight();
        }
        if (keys[SDL_SCANCODE_DOWN])
        {
            player->MoveBackwards();
        }
        if (keys[SDL_SCANCODE_LEFT])
        {
            player->RotateLeft();
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