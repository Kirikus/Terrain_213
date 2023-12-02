#ifndef ELEVATION_H
#define ELEVATION_H

#include "point.h"
namespace Elevation
{
class Elevation
{
protected:
    virtual double h(PointSpheric p) = 0;
};

class GeoData: Elevation
{
public:
    double h(PointSpheric p) override {return p.get_h();}
};

class Mountain: Elevation
{
public:
    double h(PointSpheric p) override {return p.get_h();}
};

class Plain: Elevation
{
public:
    double h(PointSpheric p) override {return p.get_h();}
};
}

#endif // ELEVATION_H
