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

    double h(PointSpheric p) {return 0;}
};

class Map1d
{
    Map data;
    PointCartesian rls;
    PointCartesian target;

    Map1d(Map m, PointCartesian rls, PointCartesian target)
        :data(m), rls(rls), target(target)
    {}

    double height(double new_d)
    {
        PointSpheric p(rls, target);
        p.change_d(new_d);
        return data.h(p);
    }
};

#endif // MAP_H
