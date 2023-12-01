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
    virtual double find_r(PointCartesian center, PointCartesian target) override {return 0;};
    virtual double find_phi(PointCartesian center, PointCartesian target) override {return 0;};
};

class ModelSpheric: ModelEarth
{
    virtual double find_r(PointCartesian center, PointCartesian target) override {return 0;};
    virtual double find_phi(PointCartesian center, PointCartesian target) override {return 0;};
};

class ModelEffectiveRadius: ModelEarth
{
    virtual double find_r(PointCartesian center, PointCartesian target) override {return 0;};
    virtual double find_phi(PointCartesian center, PointCartesian target) override {return 0;};
};

#endif // EARTH_MODELS_H
