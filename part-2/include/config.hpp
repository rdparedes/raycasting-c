#ifndef GUARD_config_hpp
#define GUARD_config_hpp

namespace Config
{
const unsigned int screenSizeX = 640;
const unsigned int screenSizeY = 400;
const unsigned int SPRITE_SIZE = 64;

const int ANGLE60 = screenSizeX;
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
}; // namespace Config

#endif