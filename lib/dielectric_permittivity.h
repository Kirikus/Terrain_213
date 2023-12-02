#ifndef DIELECTRIC_PERMITTIVITY_H
#define DIELECTRIC_PERMITTIVITY_H

#include "point.h"

namespace DielectricPermittivity
{
class DielectricPermittivity
{
public:
    virtual double dielectricPermittivity(PointSpheric p) = 0;
};

class Constant: public DielectricPermittivity
{
public:
    double dielectricPermittivity(PointSpheric p) override {return coeff;};
    double coeff;
};

class GeoData: public DielectricPermittivity
{
public:
    double dielectricPermittivity(PointSpheric p) override {return coeff;};
    double coeff; //todo
};
}

#endif // DIELECTRIC_PERMITTIVITY_H
