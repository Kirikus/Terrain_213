#include "screenangle.h"

namespace EM = EarthModels;

// The FindScreeningAngle function searches for the closing angle for a given approximate relief, returns the angle in radians
// function accepts:
// 1) Map* map - pointer to the map data;
// 2) ModelEarth* model - pointer to the model data;
// 4) PointSpheric sp - associated radar and terrain points;
// 5) R is the radius of the closing angle search (specified in meters).
// function returns:
// 1) screening angle for given R.
// Traversed by a continuous relief function

double FindScreeningAngle(Map* map, EM::ModelEarth* model, PointSpheric sp, double R)
{
    double screening_angle = 0;
    double step = 0.005;

    while (model->find_r(sp.get_center(), sp.get_target()) < R)
    {
        PointCartesian relief_dot = sp.get_target();
        double curr_h = map->h(&relief_dot);
        sp.move_target(sp.get_target().get_x(), sp.get_target().get_y(), curr_h);

        if (model->find_phi(sp.get_center(), sp.get_target()) > screening_angle)
            screening_angle = model->find_phi(sp.get_center(), sp.get_target());

        sp.change_d(sp.get_d() + step);
    }

    return screening_angle;
}
