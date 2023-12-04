#ifndef SCREENANGLE_H
#define SCREENANGLE_H

#include "point.h"
#include <cmath>

double ModelReliefFunction(double x, double y);
double FindScreeningAngle(PointCartesian rls, double azi, double R);

#endif // SCREENANGLE_H
