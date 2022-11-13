#ifndef GUARD_config_hpp
#define GUARD_config_hpp

#include <math.h>

namespace Config
{
const int game_width = 640;
const int game_height = 400;
const int minimap_width = 320;
const int minimap_height = 240;
const int window_width = game_width + minimap_width + 16;
const int window_height = game_height;
const int xCenter = game_width / 2;
const int y_center = game_height / 2;

const int SPRITE_SIZE = 64;
const int FOV = 60;
const int ANGLE60 = game_width;
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
}; // namespace Config

#endif
