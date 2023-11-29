#ifndef EARTH_MODELS_H
#define EARTH_MODELS_H

class ModelEarth
{
    virtual double find_r() = 0;
    virtual double find_phi() = 0;
};

class ModelFlat: ModelEarth
{
    virtual double find_r() override {return 0;};
    virtual double find_phi() override {return 0;};
};

class ModelSpheric: ModelEarth
{
    virtual double find_r() override {return 0;};
    virtual double find_phi() override {return 0;};
};

class ModelEffectiveRadius: ModelEarth
{
    virtual double find_r() override {return 0;};
    virtual double find_phi() override {return 0;};
};

#endif // EARTH_MODELS_H
