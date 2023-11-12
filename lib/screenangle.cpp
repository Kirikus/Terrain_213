#include "screenangle.h"

// The function simulates the relief
double ModelReliefFunction(double x, double y)
{
    return -sqrt(pow((x-20), 2) + pow(y, 2)) + 15;
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

