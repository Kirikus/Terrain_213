#ifndef DIELECTRIC_PERMITTIVITY_H
#define DIELECTRIC_PERMITTIVITY_H

#include "point.h"

namespace DielectricPermittivity
{
class DielectricPermittivity
{
public:
    virtual double dielectricPermittivity(Point2d p) = 0;
};

class Constant: public DielectricPermittivity
{
public:
    Constant(double coeff)
        :coeff(coeff)
    {}

    double dielectricPermittivity(Point2d p) override {return coeff;}
private:
    double coeff;
};

class GeoData: public DielectricPermittivity
{
public:
    double dielectricPermittivity(Point2d p) override {return dielectric_func(p);}
private:
    double dielectric_func(Point2d p);//todo
};
}

#endif // DIELECTRIC_PERMITTIVITY_H
