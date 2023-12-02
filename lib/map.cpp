#include "map.h"

double Map1d::height(double new_d)
{
    PointSpheric p(rls, target);
    p.change_d(new_d);
    return data->h(p);
}
