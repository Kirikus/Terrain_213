#ifndef MAP_H
#define MAP_H

#include "point.h"
#include "elevation.h"
#include "vegetation.h"
#include "dielectric_permittivity.h"
#include <complex>

namespace EL = Elevation;
namespace VG = Vegetation;
namespace DP = DielectricPermittivity;

class Map
{
private:
    EL::Elevation* elevation;
    VG::Vegetation* vegetation;
    DP::DielectricPermittivity* dielectricPermittivity;

public:
    Map(EL::Elevation* e, VG::Vegetation* v, DP::DielectricPermittivity* d)
        :elevation(e), vegetation(v), dielectricPermittivity(d)
    {}

    double h(Point* p) {return elevation->h(p);}
    double v(Point* p) {return vegetation->vegetation(p);}
    std::complex<double> dp(Point* p) {return dielectricPermittivity->dielectricPermittivity(p);}
};

class Map1d
{
private:
    Map* data;
    PointCartesian rls;
    PointCartesian target;
public:
    Map1d(Map* m, PointCartesian rls, PointCartesian target)
        :data(m), rls(rls), target(target)
    {}

    double height(double d);
    double vegetation(double d);
    std::complex<double> dielectric_permittivity(double d);
};

#endif // MAP_H
