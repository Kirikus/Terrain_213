#ifndef REFLECTION_POINT_H
#define REFLECTION_POINT_H

#include "point.h"
#include "map.h"

namespace ReflectionPoint
{
double FindDerivative(PointSpheric rp, Map* map);
PointCartesian FindReflectionPoint(PointSpheric sp, Map* map);
double FindPhi(double k1, double k2);
double FindIncidenceAngle(PointSpheric sp, Map* map);
}

#endif // REFLECTION_POINT_H
