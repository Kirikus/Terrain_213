#include "map.h"
#include "elevation.h"

double Vegetation::GeoData::veg_func(PointCartesian p)
{
    return p.get_x() == 2 ? 1 : 0;
}

double Elevation::GeoData::relief_func(PointCartesian p)
{
    return -sqrt(pow((p.get_x() - 20), 2) + pow(p.get_y(), 2)) + 15;
}

double Map1d::height(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    return data->h(p.get_target());
}

double Map1d::vegetation(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    return data->v(p.get_target());
}

double Map1d::dielectric_permittivity(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    return data->dp(p.get_target());
}
