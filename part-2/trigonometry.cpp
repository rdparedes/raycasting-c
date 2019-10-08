#include <cmath>
#include "trigonometry.hpp"
#include <iostream>

namespace Trigonometry
{

double fSinTable[Config::ANGLE360 + 1];
double fISinTable[Config::ANGLE360 + 1];
double fCosTable[Config::ANGLE360 + 1];
double fICosTable[Config::ANGLE360 + 1];
double fTanTable[Config::ANGLE360 + 1];
double fITanTable[Config::ANGLE360 + 1];
double fXStepTable[Config::ANGLE360 + 1];
double fYStepTable[Config::ANGLE360 + 1];

double arcToRad(int angle)
{
    return ((double(angle) * PI) / Config::ANGLE180);
}

} // namespace Trigonometry

bool Trigonometry::load()
{
    for (int i = 0; i != (Config::ANGLE360 + 1); ++i)
    {
        double radian = arcToRad(i) + (0.0001); // Add 0.0001 to avoid division by zero
        fSinTable[i] = std::sin(radian);
        fISinTable[i] = 1.0 / fSinTable[i];
        fCosTable[i] = std::cos(radian);
        fICosTable[i] = 1.0 / fCosTable[i];
        fTanTable[i] = std::tan(radian);
        fITanTable[i] = 1.0 / fTanTable[i];
    }

    return true;
}