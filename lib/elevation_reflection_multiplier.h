#ifndef ELEVATION_REFLECTION_MULTIPLIER_H
#define ELEVATION_REFLECTION_MULTIPLIER_H

#include <cmath>
#include <complex>

namespace ReflectionCoefficients
{
class ElevationReflectionMultiplier
{
public:
    double frenel_coefficient(double incidence_angle, double wave_l, double sko);
};
}

#endif // ELEVATION_REFLECTION_MULTIPLIER_H
