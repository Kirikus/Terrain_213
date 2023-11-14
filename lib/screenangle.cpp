#include "screenangle.h"

// The function models the relief with cones
double ModelReliefFunction(double x, double y)
{
    return -sqrt(pow((x - 20), 2) + pow(y, 2)) + 15;
}


//The FindCurrAngle function searches for the angle for the current position of the radius, returns the angle in radians
//Searches for the tangent of the angle through the ratio of the legs and returns the angle in degrees
//Takes the coordinates of the radar and the point that is considered the highest for a given radius
double FindCurrAngle(double x1, double y1, double z1, double x2, double y2, double z2)
{
    double opposite_cathet = z2 - z1;
    double adjacent_cathet = hypot(x2 - x1, y2 - y1);
    return atan(opposite_cathet/adjacent_cathet);
}


//The FindScreeningAngle function searches for the closing angle for a given approximate relief, returns the angle in degrees
//Accepts:
//azi - azimuth angle (specified in radians);
//R is the radius of the closing angle search (specified in meters).
//Traversed by a continuous relief function
//Now azi = 0
double FindScreeningAngle(double azi, double R)
{
    double rls_x = 0, rls_y = 0, rls_z = 0;
    double dx = 0.05*cos(azi), dy = 0.05*sin(azi);
    double curr_x = rls_x, curr_y = rls_y, curr_z = rls_z;
    double curr_r = 0;
    double screening_angle = 0;

    while (curr_r <= R)
    {
        curr_x += dx;
        curr_y += dy;
        curr_r = sqrt(pow((curr_x - rls_x), 2) + pow((curr_y - rls_y), 2));

        curr_z = ModelReliefFunction(curr_x, curr_y);
        if (curr_z <= rls_z)
            continue;

        double curr_angle = FindCurrAngle(rls_x, rls_y, rls_z, curr_x, curr_y, curr_z);
        if (curr_angle <= screening_angle)
            continue;

        screening_angle = curr_angle;
    }

    return screening_angle;
}
