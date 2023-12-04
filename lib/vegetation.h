#ifndef VEGETATION_H
#define VEGETATION_H

#include "point.h"

namespace Vegetation
{
class Vegetation
{
public:
    virtual double vegetation(Point2d p) = 0;
};

class None: public Vegetation
{
public:
    double vegetation(Point2d p) override {return 0;}
};

class Constant: public Vegetation
{
public:
    Constant(double coeff)
        :coeff(coeff)
    {}

    double vegetation(Point2d p) override {return coeff;}
private:
    double coeff;
};

class GeoData: public Vegetation
{
public:
    double vegetation(Point2d p) override {return veg_func(p);}
private:
    double veg_func(Point2d p); //todo
};
}

#endif // VEGETATION_H
