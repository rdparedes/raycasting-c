#ifndef GUARD_trigonometry_hpp
#define GUARD_trigonometry_hpp

#include "config.hpp"

namespace Trigonometry
{
const double PI = std::atan(1.0) * 4;

// Trigonometic tables to hold pre-calculated values
extern double fSinTable[Config::ANGLE360 + 1];
extern double fISinTable[Config::ANGLE360 + 1];
extern double fCosTable[Config::ANGLE360 + 1];
extern double fICosTable[Config::ANGLE360 + 1];
extern double fTanTable[Config::ANGLE360 + 1];
extern double fITanTable[Config::ANGLE360 + 1];
extern double fXStepTable[Config::ANGLE360 + 1];
extern double fYStepTable[Config::ANGLE360 + 1];
extern double fFish[Config::ANGLE60 + 1];

double arcToRad(int);
double degToRad(int);
bool load();
}; // namespace Trigonometry

#endif