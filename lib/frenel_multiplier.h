#ifndef FRENEL_MULTIPLIER_H
#define FRENEL_MULTIPLIER_H

#include <cmath>
#include <complex>
#include "map.h"

namespace ReflectionCoefficients
{

enum Polarization
{
  HorizontalPolarization = 1,
  VerticalPolarization = 2,
  CircularPolarization = 3,
  CrossPolarization = 4
};

class FrenelReflectionMultiplier
{
public:
    std::complex<double> horizontal_polarization(Map* map, PointSpheric sp, double incidence_angle, double wave_l, double conductivity);
    std::complex<double> vertical_polarization(Map* map, PointSpheric sp, double incidence_angle, double wave_l, double conductivity);
    std::complex<double> circular_polarization(Map* map, PointSpheric sp, double incidence_angle, double wave_l, double conductivity);
    std::complex<double> cross_polarization(Map* map, PointSpheric sp, double incidence_angle, double wave_l, double conductivity);
};
}
#endif // FRENEL_MULTIPLIER_H
