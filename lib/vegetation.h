#ifndef VEGETATION_H
#define VEGETATION_H

#include "point.h"

namespace Vegetation
{
class Vegetation
{
public:
    virtual double vegetation(PointCartesian p) = 0;
};

class None: public Vegetation
{
public:
    double vegetation(PointCartesian p) override {return 0;}
};

class Constant: public Vegetation
{
public:
    Constant(double coeff)
        :coeff(coeff)
    {}

    double vegetation(PointCartesian p) override {return coeff;}
private:
    double coeff;
};

class GeoData: public Vegetation
{
public:
    double vegetation(PointCartesian p) override {return coeff;}
    double coeff; //todo
};
}

#endif // VEGETATION_H
