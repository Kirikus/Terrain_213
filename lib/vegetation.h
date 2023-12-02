#ifndef VEGETATION_H
#define VEGETATION_H

#include "point.h"

namespace Vegetation
{
class Vegetation
{
public:
    virtual double vegetation(PointSpheric p) = 0;
};

class None: public Vegetation
{
public:
    double vegetation(PointSpheric p) override {return 0;}
};

class Constant: public Vegetation
{
public:
    double vegetation(PointSpheric p) override {return coeff;}
    double coeff;
};

class GeoData: public Vegetation
{
public:
    double vegetation(PointSpheric p) override {return coeff;}
    double coeff; //todo
};
}

#endif // VEGETATION_H
