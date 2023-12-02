#ifndef ELEVATION_H
#define ELEVATION_H

#include "point.h"

namespace Elevation
{
class Elevation
{
public:
  virtual double h(PointSpheric p) = 0;
};

class GeoData: public Elevation
{
public:
    double h(PointSpheric p) override {return p.get_h();}
};

class Mountain: public Elevation
{
public:
    double h(PointSpheric p) override {return p.get_h();}
};

class Plain: public Elevation
{
public:
    double h(PointSpheric p) override {return 0;}
};
}

#endif // ELEVATION_H
