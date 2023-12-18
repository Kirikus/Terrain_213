#ifndef MODELS_EARTH_H
#define MODELS_EARTH_H

#include "point.h"

namespace EarthModels
{

class ModelEarth
{
public:
    virtual double find_r(PointCartesian center, PointCartesian target) = 0;
    virtual double find_phi(PointCartesian center, PointCartesian target) = 0;
};

class ModelFlat: public ModelEarth
{
public:
    virtual double find_r(PointCartesian center, PointCartesian target) override;
    virtual double find_phi(PointCartesian center, PointCartesian target) override;
};

class ModelSpheric: public ModelEarth
{
public:
    virtual double find_r(PointCartesian center, PointCartesian target) override;
    virtual double find_phi(PointCartesian center, PointCartesian target) override;

private:
    double find_earth_angle(PointCartesian center, PointCartesian target);
    const double r_e = 6378000; // Earth radius in meters
};

class ModelEffectiveRadius: public ModelEarth
{
public:
    virtual double find_r(PointCartesian center, PointCartesian target) override;
    virtual double find_phi(PointCartesian center, PointCartesian target) override;

private:
    double find_earth_angle(PointCartesian center, PointCartesian target);
    const double r_e = 6378000; // Earth radius in meters
    const double k = 4./3; // Convenient coefficient for approximation
};

}

#endif // MODELS_EARTH_H
