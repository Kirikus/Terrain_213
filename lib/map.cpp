#include "map.h"

// This function simulates vegetation
// function accepts:
// 1) Point* p - a pointer to the point where we are looking at vegetation.
// function returns:
// a number that determines the vegetation in the area:
// 1 - grass (a = 3.2, b = 1)
// 2 - shrub or dense herbaceous vegetation (a = 0.32, b = 3)
// 3 - dense forest (a = 0.032, b = 5)

VG::VegetationType Vegetation::GeoData::veg_func(Point* p)
{
    if (-10 <= p->get_x() && p->get_x() <= 10)
        return VegetationType::Grass;
    if (-15 <= p->get_x() && p->get_x() <= -10 || 10 <= p->get_x() && p->get_x() <= 15)
        return VegetationType::Shrub;
    else
        return VegetationType::Forest;
}

// This function simulates relief
// function accepts:
// 1) Point* p - a pointer to the point where we are looking at vegetation.
// function returns:
// 1) double h - the height of the relief point.

double Elevation::GeoData::relief_func(Point* p)
{
    return fmax(-sqrt(pow((p->get_x() - 20), 2) + pow(p->get_y(), 2)) + 15, 0); // Plug
}

// This function simulates distribution of the dielectric constant over the terrain
// function accepts:
// 1) Point* p - a pointer to the point where we are looking at vegetation.
// function returns:
// 1) complex dp - the complex value of the dielectric constant.

double DielectricPermittivity::GeoData::dielectric_func(Point* p)
{
    return fmax(-sqrt(pow((p->get_x() - 20), 2) + pow(p->get_y(), 2)) + 15, 0); // Plig
}

// This function simulates distribution of the cunductivity over the terrain
// function accepts:
// 1) Point* p - a pointer to the point where we are looking at vegetation.
// function returns:
// 1) double cond - the value of the conductivity.

double Conductivity::GeoData::conductivity_func(Point* p)
{
    return fmax(-sqrt(pow((p->get_x() - 20), 2) + pow(p->get_y(), 2)) + 15, 0); // Plug
}

double Map1d::height(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    PointCartesian targ = p.get_target();
    return data->h(&targ);
}

VG::VegetationType Map1d::vegetation(double d)
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

double Map1d::conductivity(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    PointCartesian targ = p.get_target();
    return data->c(&targ);
}
