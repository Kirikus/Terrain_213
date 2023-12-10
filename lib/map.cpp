#include "map.h"

double Vegetation::GeoData::veg_func(Point2d p)
{
    return p.get_x() == 2 ? 1 : 0;
}

double Elevation::GeoData::relief_func(Point2d p)
{
    return fmax(-sqrt(pow((p.get_x() - 20), 2) + pow(p.get_y(), 2)) + 15, 0);
}

double DielectricPermittivity::GeoData::dielectric_func(Point2d p)
{
    return p.get_y() == 3 ? 1 : 0;
}

double Map1d::height(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    PointCartesian targ = p.get_target();
    Point2d point(targ.get_x(), targ.get_y());
    return data->h(point);
}

double Map1d::vegetation(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    PointCartesian targ = p.get_target();
    Point2d point(targ.get_x(), targ.get_y());
    return data->v(point);
}

double Map1d::dielectric_permittivity(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    PointCartesian targ = p.get_target();
    Point2d point(targ.get_x(), targ.get_y());
    return data->dp(point);
}
