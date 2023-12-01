#ifndef MODELS_EARTH_H
#define MODELS_EARTH_H

#include "point.h"

class ModelEarth
{
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
    double find_earth_angle(PointCartesian center, PointCartesian target);

    double r_e = 6378*1000; //temporary variable, while there is no Data class
};

class ModelEffectiveRadius: public ModelEarth
{
public:
    virtual double find_r(PointCartesian center, PointCartesian target) override;
    virtual double find_phi(PointCartesian center, PointCartesian target) override;
    double find_earth_angle(PointCartesian center, PointCartesian target);

    double r_e = 6378*1000; //temporary variable, while there is no Data class
    double k = 4./3; //convenient coefficient for approximation
};

#endif // MODELS_EARTH_H
