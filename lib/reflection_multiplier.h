#ifndef REFLECTION_MULTIPLIER_H
#define REFLECTION_MULTIPLIER_H

#include <cmath>
#include <complex>
#include "elevation_reflection_multiplier.h"
#include "frenel_multiplier.h"
#include "vegetation_reflection_multiplier.h"

using complex = std::complex<double>;

namespace ReflectionCoefficients
{
class ReflectionMultiplier
{
public:
    complex reflection_multiplier();
    ReflectionMultiplier(ElevationReflectionMultiplier* ElevationCoeff, FrenelReflectionMultiplier* FrenelCoeff, VegetationReflectionMultiplier* VegetationCoeff)
        :ElevationCoeff(ElevationCoeff), FrenelCoeff(FrenelCoeff), VegetationCoeff(VegetationCoeff)
    {}
private:
    ElevationReflectionMultiplier* ElevationCoeff = nullptr;
    FrenelReflectionMultiplier* FrenelCoeff = nullptr;
    VegetationReflectionMultiplier* VegetationCoeff = nullptr;
};
}

#endif // REFLECTION_MULTIPLIER_H
