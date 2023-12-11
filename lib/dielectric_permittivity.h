#ifndef DIELECTRIC_PERMITTIVITY_H
#define DIELECTRIC_PERMITTIVITY_H

#include "point.h"
#include <complex>

namespace DielectricPermittivity
{
class DielectricPermittivity
{
public:
    virtual std::complex<double> dielectricPermittivity(Point* p) = 0;
};

class Constant: public DielectricPermittivity
{
public:
    Constant(double coeff)
        :coeff(coeff)
    {}

    std::complex<double> dielectricPermittivity(Point* p) override {return coeff;}
private:
    std::complex<double> coeff;
};

class GeoData: public DielectricPermittivity
{
public:
    std::complex<double> dielectricPermittivity(Point* p) override {return dielectric_func(p);}
private:
    std::complex<double> dielectric_func(Point* p);//todo
};
}

#endif // DIELECTRIC_PERMITTIVITY_H
