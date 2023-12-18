#include "reflection_multiplier.h"

namespace RC = ReflectionCoefficients;

std::complex<double> RC::ReflectionMultiplier::reflection_multiplier(Map* map, PointSpheric sp, Polarization pol, double incidence_angle, double wave_l, double sko)
// This function calculates total reflectance coefficient
// function accepts:
// 1) pointer to the map;
// 2) sp - related rls and relfection point;
// 3) pol - antenna polarization;
// 4) incidence angle;
// 5) wave lenght;
// 6) conductivity;
// 7) sko;
// 8) veg.
// function returns:
// 1) total reflectance coefficient.
{
    PointCartesian reflection_point = sp.get_target();
    double el_c = ElevationCoeff.frenel_coefficient(incidence_angle, wave_l, sko);
    double conductivity = map->c(&reflection_point);
    Vegetation::VegetationType veg = map->v(&reflection_point);

    std::complex<double> fr_c;
    switch (pol) {
    case HorizontalPolarization:
        fr_c = FrenelCoeff.horizontal_polarization(map, sp, incidence_angle, wave_l, conductivity);
        break;
    case VerticalPolarization:
        fr_c = FrenelCoeff.vertical_polarization(map, sp, incidence_angle, wave_l, conductivity);
        break;
    case CircularPolarization:
        fr_c = FrenelCoeff.circular_polarization(map, sp, incidence_angle, wave_l, conductivity);
        break;
    case CrossPolarization:
        fr_c = FrenelCoeff.cross_polarization(map, sp, incidence_angle, wave_l, conductivity);
        break;
    }

    double vg_c = VegetationCoeff.vegetation_coeff(incidence_angle, veg, wave_l);
    return el_c * fr_c * vg_c;
}
