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
    int _rotation;
    SDL_Rect spriteRectangle;

    const double &xDirection() const
    {
        return fCosTable[_rotation];
    }

    const double &yDirection() const
    {
        return fSinTable[_rotation];
    }

public:
    Player(const int &x, const int &y)
    {
        spriteRectangle.x = x;
        spriteRectangle.y = y;
        spriteRectangle.w = SPRITE_SIZE;
        spriteRectangle.h = SPRITE_SIZE;
        _rotation = ANGLE0;
    }

    const int speed = 8;

    const int &rotation() const { return _rotation; }

    const int spriteRotation() const
    {
        int temp = _rotation + ANGLE90;
        if (temp > ANGLE360)
        {
            temp -= ANGLE360;
        }
        return temp;
    }

    const SDL_Rect *rectangle() const { return &spriteRectangle; }

    void rotateRight()
    {
        _rotation += ANGLE5;
        if (_rotation >= ANGLE360)
        {
            _rotation -= ANGLE360;
        }
    }

    void rotateLeft()
    {
        _rotation -= ANGLE5;
        if (_rotation < ANGLE0)
        {
            _rotation += ANGLE360;
        }
    }

    void moveForward()
    {
        spriteRectangle.x += int(std::round(xDirection() * speed));
        spriteRectangle.y += int(std::round(yDirection() * speed));
    }

    void moveBackwards()
    {
        spriteRectangle.x -= int(std::round(xDirection() * speed));
        spriteRectangle.y -= int(std::round(yDirection() * speed));
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
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                gameLoop = false;

            else if (event.type == SDL_KEYDOWN)
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
            }
        }

        render();
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