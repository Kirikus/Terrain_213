#ifndef DIELECTRIC_PERMITTIVITY_H
#define DIELECTRIC_PERMITTIVITY_H

#include "point.h"

namespace DielectricPermittivity
{
class DielectricPermittivity
{
public:
    virtual double dielectricPermittivity(Point* p) = 0;
};

class Constant: public DielectricPermittivity
{
public:
    Constant(double coeff)
        :coeff(coeff)
    {}

    double dielectricPermittivity(Point* p) override {return coeff;}
private:
    double coeff;
};

class GeoData: public DielectricPermittivity
{
public:
    double dielectricPermittivity(Point* p) override {return dielectric_func(p);}
private:
    double dielectric_func(Point* p);//todo
};
}

#endif // DIELECTRIC_PERMITTIVITY_H
