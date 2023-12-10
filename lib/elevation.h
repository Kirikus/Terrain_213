#ifndef ELEVATION_H
#define ELEVATION_H

#include "point.h"

namespace Elevation
{
class Elevation
{
public:
  virtual double h(Point* p) = 0;
};

class GeoData: public Elevation
{
public:
  double h(Point* p) override {return relief_func(p);}
private:
  double relief_func(Point* p);
}; // todo

class Mountain: public Elevation
{
public:
  double h(Point* p) override {return H - p->get_x()  * slope;}
    double H;
    double slope;
}; //todo

class Plain: public Elevation
{
public:
    double h(Point* p) override {return 0;}
};
}

#endif // ELEVATION_H
