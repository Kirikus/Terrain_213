#ifndef FRENEL_MULTIPLIER_H
#define FRENEL_MULTIPLIER_H

#include <cmath>
#include <complex>
#include "map.h"

using complex = std::complex<double>;

namespace ReflectionCoefficients
{
class FrenelReflectionMultiplier
{
public:
    complex horizontal_polarization(Map* map, PointSpheric sp, double incidence_angle);
    complex vertical_polarization(Map* map, PointSpheric sp, double incidence_angle);
};
}
#endif // FRENEL_MULTIPLIER_H
