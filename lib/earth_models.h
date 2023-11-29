#ifndef EARTH_MODELS_H
#define EARTH_MODELS_H

class EarthModel
{
    virtual double find_r() = 0;
    virtual double find_phi() = 0;
};
#endif // EARTH_MODELS_H
