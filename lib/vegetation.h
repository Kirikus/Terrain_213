#ifndef VEGETATION_H
#define VEGETATION_H

#include "point.h"

namespace Vegetation
{
class Vegetation
{
public:
    virtual int vegetation(Point* p) = 0;
};

class None: public Vegetation
{
public:
    int vegetation(Point* p) override {return 0;}
};

// coeff - the coefficient is from 1 to 3
// 1 - grass (a = 3.2, b = 1)
// 2 - shrub or dense herbaceous vegetation (a = 0.32, b = 3)
// 3 - dense forest (a = 0.032, b = 5)

class Constant: public Vegetation
{
public:
    Constant(double coeff)
        :coeff(coeff)
    {}

    int vegetation(Point* p) override {return coeff;}
private:
    int coeff;
};

class GeoData: public Vegetation
{
public:
    int vegetation(Point* p) override {return veg_func(p);}
private:
    int veg_func(Point* p); //todo
};
}

#endif // VEGETATION_H
