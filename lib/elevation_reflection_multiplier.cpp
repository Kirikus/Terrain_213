#include "elevation_reflection_multiplier.h"

namespace RP = ReflectionCoefficients;

//  This function calculates Frenel Coefficient in case of uneven surface
//  function accepts:
//  1) incidence angle in radians.
//  2) wave lenght in meters;
//  3) The COE of the height spread from the average value.
//  function returns:
//  1) frenel coefficient;
//  formula (2.13) on the page 48

double RP::ElevationReflectionMultiplier::frenel_coefficient(double incidence_angle, double wave_l, double sko)
{
    double coef = exp((-1./2) * pow((4 * M_PI * sko * sin(incidence_angle) / wave_l), 2));
    return coef;
}
