#ifndef HEIGHT_DETERMINATION_H
#define HEIGHT_DETERMINATION_H

#include "point.h"
#include "map.h"
#include "reflection_point.h"

double FindTargetHeight(Map* map, EarthModels::ModelEarth* model, PointCartesian rls, Point2d target, PointCartesian* reflection_point, double r, double r1, double r2, double r_max);

#endif // HEIGHT_DETERMINATION_H
