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
    double rls_x = rls.get_x();
    double rls_y = rls.get_y();
    double rls_h = rls.get_h();

    double curr_x = rls_x;
    double curr_y = rls_y;
    double curr_h = rls_h;

    PointCartesian curr_dot(curr_x, curr_y, curr_h);
    PointSpheric sp(rls, curr_dot);

    double dx = 0.001*cos(azi);
    double dy = 0.001*sin(azi);

    double curr_r = 0;
    double screening_angle = 0;

    while (curr_r < R)
    {
        curr_x += dx;
        curr_y += dy;
        curr_h = fmax(ModelReliefFunction(curr_x, curr_y), 0);

        sp.move_target(curr_x, curr_y, curr_h);
        curr_r = sp.get_R();

        if (curr_h <= rls_h)
            continue;

        if (sp.get_phi() <= screening_angle)
            continue;

        screening_angle = sp.get_phi();
    }

    return screening_angle;
}
