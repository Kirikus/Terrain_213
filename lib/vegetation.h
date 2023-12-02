#ifndef VEGETATION_H
#define VEGETATION_H

#include "point.h"

namespace Vegetation
{
class Vegetation
{
protected:
    virtual double vegetation(PointSpheric p) = 0;
};

class None: Vegetation
{
public:
    double vegetation(PointSpheric p) override;
};

class Constant: Vegetation
{
public:
    double vegetation(PointSpheric p) override;
};

class GeoData: Vegetation
{
public:
    double vegetation(PointSpheric p) override;
};
}

#endif // VEGETATION_H
