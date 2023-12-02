#include "map.h"

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

