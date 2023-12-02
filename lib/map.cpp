#include "map.h"

double Map1d::height(double d)
{
    PointSpheric p(rls, target);
    p.change_d(d);
    return data->h(p);
}
