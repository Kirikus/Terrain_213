#ifndef EARTH_MODELS_H
#define EARTH_MODELS_H

#include "point.h"

class ModelEarth
{
    virtual double find_r(PointCartesian center, PointCartesian target) = 0;
    virtual double find_phi(PointCartesian center, PointCartesian target) = 0;
};

class ModelFlat: ModelEarth
{
public:
    virtual double find_r(PointCartesian center, PointCartesian target) override {return 0;};
    virtual double find_phi(PointCartesian center, PointCartesian target) override {return 0;};
};

class ModelSpheric: ModelEarth
{
public:
    virtual double find_r(PointCartesian center, PointCartesian target) override;
    virtual double find_phi(PointCartesian center, PointCartesian target) override;
    double find_earth_angle(PointCartesian center, PointCartesian target);

    double r_e = 6378*1000; //temporary variable, while there is no Data class
};

class ModelEffectiveRadius: ModelEarth
{
public:
    virtual double find_r(PointCartesian center, PointCartesian target) override {return 0;};
    virtual double find_phi(PointCartesian center, PointCartesian target) override {return 0;};
};

#endif // EARTH_MODELS_H
