#ifndef VEGETATION_H
#define VEGETATION_H

#include "point.h"
#include <complex>

using complex = std::complex<double>;

namespace Vegetation
{
class Vegetation
{
protected:
    virtual complex vegetation(PointSpheric p) = 0;
};

class None: Vegetation
{
public:
    complex vegetation(PointSpheric p) override;
};

class Constant: Vegetation
{
public:
    complex vegetation(PointSpheric p) override;
};

class GeoData: Vegetation
{
public:
    complex vegetation(PointSpheric p) override;
};
}

#endif // VEGETATION_H
