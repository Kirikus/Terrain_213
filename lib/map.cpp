#include "map.h"

double Vegetation::GeoData::veg_func(Point* p)
{
    return p->get_x() == 2 ? 1 : 0;
}

double Elevation::GeoData::relief_func(Point* p)
{
    return fmax(-sqrt(pow((p->get_x() - 20), 2) + pow(p->get_y(), 2)) + 15, 0);
}

double DielectricPermittivity::GeoData::dielectric_func(Point* p)
{
    return p->get_y() == 3 ? 1 : 0;
}

double Map1d::height(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    PointCartesian targ = p.get_target();
    return data->h(&targ);
}

double Map1d::vegetation(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    PointCartesian targ = p.get_target();
    return data->v(&targ);
}

double Map1d::dielectric_permittivity(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    PointCartesian targ = p.get_target();
    return data->dp(&targ);
}
