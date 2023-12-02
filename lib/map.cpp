#include "map.h"
#include "elevation.h"

double Elevation::GeoData::relief_func(PointCartesian p)
{
    return -sqrt(pow((p.get_x() - 20), 2) + pow(p.get_y(), 2)) + 15;
}

double Map1d::height(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    return data->h(p);
}

double Map1d::vegetation(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    return data->v(p);
}

double Map1d::dielectric_permittivity(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    return data->dp(p);
}
