#ifndef MAP_H
#define MAP_H

#include "point.h"
#include <complex>
using complex = std::complex<double>;

class Elevation
{
protected:
    virtual double h(PointSpheric p) = 0;
};

#endif // MAP_H
