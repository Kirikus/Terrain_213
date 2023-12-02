#ifndef DIELECTRIC_PERMITTIVITY_H
#define DIELECTRIC_PERMITTIVITY_H

#include "point.h"

namespace DielectricPermittivity
{
class DielectricPermittivity
{
protected:
    virtual double dielectricPermittivity(PointSpheric p) = 0;
};

class Constant: DielectricPermittivity
{
public:
    double dielectricPermittivity(PointSpheric p) override;
};

class GeoData: DielectricPermittivity
{
public:
    double dielectricPermittivity(PointSpheric p) override;
};
}

#endif // DIELECTRIC_PERMITTIVITY_H
