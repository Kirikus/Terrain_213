#include <QApplication>

#include "mainwindow.h"
#include "../lib/screenangle.h"

std::vector<std::vector<Point2d>> _screen_angle_search()
{
    std::vector<double> angles = {(M_PI / 2) / 5, (M_PI / 2) / 5 * 2, (M_PI / 2) / 5 * 3, (M_PI / 2) / 5 * 4, (M_PI / 2)};  // angles for the i contour (<= angles[i])
    double angle_iter = 100;  // count of iteration in angle loop
    double R_iter = 100;  // count of iteration in radius loop
    double R_step = 30 / R_iter;  // step for in cycle

    PointCartesian rls_position = PointCartesian(0, 0, 0);
    PointSpheric current_point = PointSpheric(rls_position, PointCartesian(rls_position.get_x() + 1, rls_position.get_y() + 1, rls_position.get_h()));

    size_t n = angles.size();  // count_of_contours
    std::vector<std::vector<Point2d>> contours(n);  // points of contours to draw visibility angle map

    int last_external_contour = -1;  // index of last contour which was external
    PointSpheric previous_external_point(current_point);

    for (double azimuth = 0; azimuth < 2 * M_PI; azimuth += 2 * M_PI / angle_iter)
    {
        current_point.change_azimuth(azimuth);
        bool all_empty = true;

        std::vector<PointScreenAngle> max_points(n); // current max points (with max screening angle) for the i contour
        for (double R = R_step; R <= 30; R += R_step)
        {
            current_point.change_r(R);

            double screening_angle = FindScreeningAngle(current_point, 30);

            for (size_t i = 0; i < n; i++)
            {
                if (screening_angle > angles[i])
                {
                    max_points[i + 1] = PointScreenAngle(current_point.get_target().get_x(), current_point.get_target().get_y(), screening_angle);
                    all_empty = false;
                }
            }
        }

        if (all_empty)
        {
            contours[0].push_back(Point2d(current_point.get_target().get_x(), current_point.get_target().get_y()));
            if (last_external_contour != 0 && last_external_contour != -1)
                contours[0].push_back(Point2d(previous_external_point.get_x(), previous_external_point.get_y()));
            last_external_contour = 0;
            previous_external_point = current_point;
            continue;
        }

        int last_contour = -1;
        for (int i = n - 1; i >= 0; i--)
        {
            if (!max_points[i].empty())  // any point for contour[i] exists
            {
                if (last_contour == -1)  // if haven't add any point before
                {
                    contours[i].push_back(Point2d(current_point.get_target().get_x(), current_point.get_target().get_y()));  // add external point
                    if (last_external_contour != i && last_external_contour != -1)
                        contours[i].push_back(Point2d(previous_external_point.get_x(), previous_external_point.get_y()));
                    last_external_contour = i;
                    previous_external_point = current_point;
                }
                else
                    contours[i].push_back(contours[last_contour].back());  // add point of previous contour

                contours[i].push_back(max_points[i].point2d);  // add own point of contours[i]
                last_contour = i;
            }
            else if (i == 0)  // no point for contours[i] but it's the last one
                contours[0].push_back(contours[last_contour].back());  // add
        }
    }

    return contours;
}


int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
//    _screen_angle_search();
//    return 0;
}
