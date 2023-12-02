#ifndef DIELECTRIC_PERMITTIVITY_H
#define DIELECTRIC_PERMITTIVITY_H

#include "point.h"

namespace DielectricPermittivity
{
class DielectricPermittivity
{
public:
    virtual double dielectricPermittivity(PointCartesian p) = 0;
};

class Constant: public DielectricPermittivity
{
public:
    double dielectricPermittivity(PointCartesian p) override {return coeff;};
    double coeff;
};

class GeoData: public DielectricPermittivity
{
public:
    double dielectricPermittivity(PointCartesian p) override {return coeff;};
    double coeff; //todo
};
}

#endif // DIELECTRIC_PERMITTIVITY_H
