#include "reflection_multiplier.h"

namespace RP = ReflectionCoefficients;

std::complex<double> RP::ReflectionMultiplier::reflection_multiplier(Map* map, PointSpheric sp, double incidence_angle, double wave_l, double sko, double a, double b)
{
    double el_c = ElevationCoeff->frenel_coefficient(incidence_angle, wave_l, sko);
    std::complex<double> fr_c = FrenelCoeff->horizontal_polarization(map, sp, incidence_angle);
    double vg_c = VegetationCoeff->vegetation_coeff(incidence_angle, a, b, wave_l);
    return el_c * fr_c * vg_c;
}
