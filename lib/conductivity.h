#ifndef CONDUCTIVITY_H
#define CONDUCTIVITY_H

#include "point.h"

namespace Conductivity
{
class Conductivity
{
public:
    virtual double conductivity(Point* p) = 0;
};

class Constant: public Conductivity
{
public:
    Constant(double coeff)
        :coeff(coeff)
    {}

    double conductivity(Point* p) override {return coeff;}
private:
    double coeff;
};

class GeoData: public Conductivity
{
public:
    double conductivity(Point* p) override {return conductivity_func(p);}
private:
    double conductivity_func(Point* p);//todo
};
}

#endif // CONDUCTIVITY_H
