#ifndef SCREENANGLE_H
#define SCREENANGLE_H

#include "point.h"
#include "map.h"
#include "models_earth.h"

double FindScreeningAngle(Map* map, EarthModels::ModelEarth* model, PointSpheric sp, double R);

#endif // SCREENANGLE_H
