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
  virtual double h(Point* p) override {return relief_func(p);}
private:
  double relief_func(Point* p);
}; // todo

class Mountain: public Elevation
{
public:
  Mountain(double H, double slope)
        :H(H), slope(slope)
    {}

  virtual double h(Point* p) override {return H - std::hypot(p->get_x(), p->get_y()) * slope;}
private:
    double H;
    double slope;
};

class Plain: public Elevation
{
public:
    virtual double h(Point* p) override {return 0;}
};
}

#endif // ELEVATION_H
