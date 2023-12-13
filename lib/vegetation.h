#ifndef VEGETATION_H
#define VEGETATION_H

#include "point.h"

namespace Vegetation
{

enum class VegetationType
{
    None = 0,
    Grass = 1,
    Shrub = 2,
    Forest = 3
};

class Vegetation
{
public:
    virtual VegetationType vegetation(Point* p) = 0;
};

class None: public Vegetation
{
public:
    VegetationType vegetation(Point* p) override {return VegetationType::None;}
};

// coeff - the coefficient is from 1 to 3
// 1 - grass (a = 3.2, b = 1)
// 2 - shrub or dense herbaceous vegetation (a = 0.32, b = 3)
// 3 - dense forest (a = 0.032, b = 5)

class Constant: public Vegetation
{
public:
    Constant(VegetationType  coeff)
        :coeff(coeff)
    {}

    VegetationType vegetation(Point* p) override {return coeff;}
private:
    VegetationType coeff;
};

class GeoData: public Vegetation
{
public:
    VegetationType vegetation(Point* p) override {return veg_func(p);}
private:
    VegetationType veg_func(Point* p);
};
}

#endif // VEGETATION_H
