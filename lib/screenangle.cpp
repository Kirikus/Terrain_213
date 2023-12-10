#include "screenangle.h"

// The FindScreeningAngle function searches for the closing angle for a given approximate relief, returns the angle in radians
// Accepts:
// PointSpheric sp - associated radar and terrain points
// R is the radius of the closing angle search (specified in meters).
// Traversed by a continuous relief function
double FindScreeningAngle(Map* map, PointSpheric sp, double R)
{
    double screening_angle = 0;
    double step = 0.005;

    while (sp.get_R() < R)
    {
        PointCartesian relief_dot = sp.get_target();
        double curr_h = map->h(&relief_dot);
        sp.move_target(sp.get_target().get_x(), sp.get_target().get_y(), curr_h);

        if (sp.get_phi() > screening_angle)
            screening_angle = sp.get_phi();

        sp.change_d(sp.get_d() + step);
    }

    return screening_angle;
}
