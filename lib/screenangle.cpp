#include "screenangle.h"

// The function models the relief with cones
double ModelReliefFunction(double x, double y)
{
    return -sqrt(pow((x - 20), 2) + pow(y, 2)) + 15;
}

// The FindScreeningAngle function searches for the closing angle for a given approximate relief, returns the angle in radians
// Accepts:
// PointSpheric sp - associated radar and terrain points
// R is the radius of the closing angle search (specified in meters).
// Traversed by a continuous relief function
double FindScreeningAngle(PointSpheric sp, double R)
{
    double screening_angle = 0;
    double step = 0.005;

    while (sp.get_R() < R)
    {
        double curr_h = ModelReliefFunction(sp.get_target().get_x(), sp.get_target().get_y());
        sp.move_target(sp.get_target().get_x(), sp.get_target().get_y(), curr_h);

        if (sp.get_phi() > screening_angle)
            screening_angle = sp.get_phi();

        sp.change_d(sp.get_d() + step);
    }

    return screening_angle;
}
