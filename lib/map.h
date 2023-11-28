#ifndef MAP_H
#define MAP_H

#include "point.h"
#include <complex>
using complex = std::complex<double>;

class Elevation
{
protected:
    virtual double h(PointSpheric p) = 0;
};

class Vegetation
{
protected:
    virtual complex vegetation(PointSpheric p) = 0;
};

class DielectricPermittivity
{
protected:
    virtual complex dielectricPermittivity(PointSpheric p) = 0;
};

class None: Vegetation
{
public:
    complex vegetation(PointSpheric p) override;
};

class Constant: Vegetation, DielectricPermittivity
{
public:
    complex vegetation(PointSpheric p) override;
    complex dielectricPermittivity(PointSpheric p) override;
};

class GeoData: Elevation, Vegetation, DielectricPermittivity
{
public:
    double h(PointSpheric p) override {return p.get_h();}
    complex vegetation(PointSpheric p) override;
    complex dielectricPermittivity(PointSpheric p) override;
};

#endif // MAP_H
