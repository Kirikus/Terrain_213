#include "earth_models.h"

//This function calculates angle between two radii of earth
double ModelSpheric::find_earth_angle(PointCartesian center, PointCartesian target)
{
    double x = center.get_x() - target.get_x();
    double y = center.get_y() - target.get_y();
    double distance = hypot(x, y);

    double earth_angle = acos(1 - pow(distance, 2)/(2 * pow(r_e, 2)));
    return earth_angle;
}

