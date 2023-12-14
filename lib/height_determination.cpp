#include "height_determination.h"

namespace EM = EarthModels;
namespace RP = ReflectionPoint;

double FindTargetHeight(Map* map, EM::ModelEarth* model, PointCartesian rls, Point2d target, double r, double r1, double r2, double r_max)
{
    PointCartesian target_cartesian(target.get_x(), target.get_y(), 0);
    PointSpheric sp(rls, target_cartesian);

    Map1d map1d(map, sp.get_center(), sp.get_target());

    double r_diff = r1 + r2 - r;
    double deviation_min = 1000;
    double step_d = 0.05;

    double target_height = -1;

    for (double curr_d = step_d; curr_d < sp.get_d(); curr_d += step_d)
    {
        double h = map1d.height(curr_d);
        double x = sp.get_center().get_x() + curr_d * std::cos(sp.get_azimuth());
        double y = sp.get_center().get_y() + curr_d * std::sin(sp.get_azimuth());

        PointCartesian relief_dot(x, y, h);
        PointSpheric rp1(sp.get_center(), relief_dot);

        double dh1 = model->find_r(rp1.get_center(), rp1.get_target()) * std::sin(model->find_phi(rp1.get_center(), rp1.get_target()));
        double k1 = dh1 / rp1.get_d();
        double derivative = RP::FindDerivative(rp1, map);

        double curr_incidence_angle = std::abs(RP::FindPhi(derivative, k1));
        double curr_reflection_angle = 0;

        double step_h = 0.005;
        double curr_h = 0.005;
        for (; r_max >= curr_h;) {
            sp.move_target(sp.get_target().get_x(), sp.get_target().get_y(), curr_h);
            PointSpheric rp2(sp.get_target(), relief_dot);
            double dh2 = model->find_r(rp2.get_center(), rp2.get_target()) * std::sin(model->find_phi(rp2.get_center(), rp2.get_target()));
            double k2 = - dh2 / rp2.get_d();
            curr_reflection_angle = std::abs(RP::FindPhi(derivative, k2));

            double curr_r_diff = model->find_r(rp1.get_center(), rp1.get_target()) + model->find_r(rp2.get_center(), rp2.get_target()) - model->find_r(sp.get_center(), sp.get_target());
            double curr_angle_diff = curr_reflection_angle - curr_incidence_angle;

            double curr_deviation = std::abs(std::pow(curr_r_diff - r_diff, 2) + std::pow(curr_angle_diff, 2));
            if (curr_deviation < deviation_min)
            {
                deviation_min = curr_deviation;
                target_height = curr_h;
            }

            curr_h += step_h;
        }

    }

    return target_height;
}
