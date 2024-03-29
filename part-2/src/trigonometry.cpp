#include <cmath>
#include <iostream>
#include "trigonometry.hpp"

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
double fFish[Config::ANGLE60 + 1];

double ArcToRad(int angle)
{
    return ((double(angle) * PI) / Config::ANGLE180);
}

double DegToRad(int degree)
{
    return degree * PI / 180.0;
}

} // namespace Trigonometry

bool Trigonometry::load()
{
    for (int i = 0; i != (Config::ANGLE360 + 1); ++i)
    {
        double radian = ArcToRad(i) + (0.0001); // Add 0.0001 to avoid division by zero
        fSinTable[i] = std::sin(radian);
        fISinTable[i] = 1.0 / fSinTable[i];
        fCosTable[i] = std::cos(radian);
        fICosTable[i] = 1.0 / fCosTable[i];
        fTanTable[i] = std::tan(radian);
        fITanTable[i] = 1.0 / fTanTable[i];

        // Left - Right
        fXStepTable[i] = double(Config::SPRITE_SIZE) / fTanTable[i];
        if (i >= Config::ANGLE90 && i < Config::ANGLE270)
        {
            if (fXStepTable[i] > 0)
                fXStepTable[i] = -fXStepTable[i];
        }
        else if (fXStepTable[i] < 0)
        {
            fXStepTable[i] = -fXStepTable[i];
        }
        // Up - Down
        fYStepTable[i] = double(Config::SPRITE_SIZE) * fTanTable[i];
        if (i >= Config::ANGLE0 && i < Config::ANGLE180)
        {
            if (fYStepTable[i] < 0)
                fYStepTable[i] = -fYStepTable[i];
        }
        else if (fYStepTable[i] > 0)
        {
            fYStepTable[i] = -fYStepTable[i];
        }
    }

    // Fishbowl distortion fix
    for (int i = -Config::ANGLE30; i != (Config::ANGLE30 + 1); ++i)
    {
        double radian = ArcToRad(i);
        int index = i + Config::ANGLE30;
        fFish[index] = 1.0 / std::cos(radian);
    }

    return true;
}