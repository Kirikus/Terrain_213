#ifndef MAP_H
#define MAP_H

#include "point.h"
#include "elevation.h"
#include "vegetation.h"
#include "dielectric_permittivity.h"
#include "conductivity.h"
#include <complex>

namespace EL = Elevation;
namespace VG = Vegetation;
namespace DP = DielectricPermittivity;
namespace CD = Conductivity;

class Map
{
private:
    EL::Elevation* elevation;
    VG::Vegetation* vegetation;
    DP::DielectricPermittivity* dielectricPermittivity;
    CD::Conductivity* conductivity;

public:
    Map(EL::Elevation* e, VG::Vegetation* v, DP::DielectricPermittivity* d, CD::Conductivity* c)
        :elevation(e), vegetation(v), dielectricPermittivity(d), conductivity(c)
    {}

    double h(Point* p) const {return elevation->h(p);}
    VG::VegetationType v(Point* p) const {return vegetation->vegetation(p);}
    double dp(Point* p) const {return dielectricPermittivity->dielectricPermittivity(p);}
    double c(Point* p) const {return conductivity->conductivity(p);}
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
    VG::VegetationType vegetation(double d);
    double dielectric_permittivity(double d);
    double conductivity(double d);
};

#endif // MAP_H
