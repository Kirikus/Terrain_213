#ifndef FRENEL_MULTIPLIER_H
#define FRENEL_MULTIPLIER_H

#include <cmath>
#include <complex>
#include "map.h"

namespace ReflectionCoefficients
{
class FrenelReflectionMultiplier
{
public:
    std::complex<double> horizontal_polarization(Map* map, PointSpheric sp, double incidence_angle);
    std::complex<double> vertical_polarization(Map* map, PointSpheric sp, double incidence_angle);
};
}
#endif // FRENEL_MULTIPLIER_H
