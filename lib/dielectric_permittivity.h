#ifndef DIELECTRIC_PERMITTIVITY_H
#define DIELECTRIC_PERMITTIVITY_H

#include "point.h"
#include <complex>
using complex = std::complex<double>;

namespace DielectricPermittivity
{
class DielectricPermittivity
{
protected:
    virtual complex dielectricPermittivity(PointSpheric p) = 0;
};

class Constant: DielectricPermittivity
{
public:
    complex dielectricPermittivity(PointSpheric p) override;
};

class GeoData: DielectricPermittivity
{
public:
    complex dielectricPermittivity(PointSpheric p) override;
};
}

#endif // DIELECTRIC_PERMITTIVITY_H
