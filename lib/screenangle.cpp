#include "screenangle.h"

// The function models the relief with cones
double ModelReliefFunction(double x, double y)
{
    return -sqrt(pow((x - 20), 2) + pow(y, 2)) + 15;
}

//The FindScreeningAngle function searches for the closing angle for a given approximate relief, returns the angle in radians
//Accepts:
//azi - azimuth angle (specified in radians);
//R is the radius of the closing angle search (specified in meters).
//Traversed by a continuous relief function
double FindScreeningAngle(PointCartesian rls, double azi, double R)
{
    double dx = 0.001*cos(azi);
    double dy = 0.001*sin(azi);

    PointSpheric sp(rls, rls);

    double screening_angle = 0;

    while (sp.get_R() < R)
    {
        double curr_h = fmax(ModelReliefFunction(sp.get_target().get_x() + dx, sp.get_target().get_y() + dy), 0);

        sp.move_target(sp.get_target().get_x() + dx, sp.get_target().get_y() + dy, curr_h);

        if (curr_h <= rls.get_h())
            continue;

        if (sp.get_phi() > screening_angle)
            screening_angle = sp.get_phi();
    }

    return screening_angle;
}
