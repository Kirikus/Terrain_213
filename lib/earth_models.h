#ifndef EARTH_MODELS_H
#define EARTH_MODELS_H

class EarthModel
{
    virtual double find_r() = 0;
    virtual double find_phi() = 0;
};

class FlatModel: EarthModel
{
    virtual double find_r() override {return 0;};
    virtual double find_phi() override {return 0;};
};

class SphericModel: EarthModel
{
    virtual double find_r() override {return 0;};
    virtual double find_phi() override {return 0;};
};

class EffectiveRadiusModel: EarthModel
{
    virtual double find_r() override {return 0;};
    virtual double find_phi() override {return 0;};
};

#endif // EARTH_MODELS_H
