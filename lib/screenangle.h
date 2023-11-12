#ifndef SCREENANGLE_H
#define SCREENANGLE_H

#include <cmath>

double ModelReliefFunction(double x, double y);
double FindCurrAngle(double x1, double y1, double z1, double x2, double y2, double z2);
double FindScreeningAngle(double azi, double R);

#endif // SCREENANGLE_H
