#include "visibility_map_classes.h"

void Contour::add_point(PointSpheric point, const bool external_point)
{
    double azimuth = point.get_azimuth();
    int key = int(azimuth * angle_iterations);
    if (external_point)
        if (points.find(key) != points.end())
            points[key].second = Point2d(point.get_target().get_x(), point.get_target().get_y());
        else
            points[key] = std::make_pair(rls_position, Point2d(point.get_target().get_x(), point.get_target().get_y()));
    else
        if (points.find(key) != points.end())
            points[key].first = Point2d(point.get_target().get_x(), point.get_target().get_y());
        else
            points[key] = std::make_pair(Point2d(point.get_target().get_x(), point.get_target().get_y()), rls_position);
    last_added_point = point;
}

void Contour::change_point(const double azimuth, PointSpheric point, const bool external_point)
{
    int key = int(azimuth * angle_iterations);
    if (external_point)
        if (points.find(key) != points.end())
            points[key].second = Point2d(point.get_target().get_x(), point.get_target().get_y());
        else
            points[key] = std::make_pair(rls_position, Point2d(point.get_target().get_x(), point.get_target().get_y()));
    else
        if (points.find(key) != points.end())
            points[key].first = Point2d(point.get_target().get_x(), point.get_target().get_y());
        else
            points[key] = std::make_pair(Point2d(point.get_target().get_x(), point.get_target().get_y()), rls_position);
    last_added_point = point;
}

QCPCurveData Contour::get_point(const int key, const int i, const bool external)
{
    double x, y;
    if (external)
    {
            x = points[key].second.get_x();
            y = points[key].second.get_y();
    }
    else
    {
            x = points[key].first.get_x();
            y = points[key].first.get_y();
    }
    return QCPCurveData(i, x, y);
}

QVector<QCPCurveData> Contour::get_data()
{
    if (if_zero())
            return get_zero_data();

    QVector<QCPCurveData> result;

    bool point_before = false;  // if there were point for previous azimuth value
    int i = 0;  // parameter for QCPCurveData
    int first_azimuth = 0;  // key which was the first when algorithms go in reverse
    int max_added_azimuth = -1;  // max azimuth of all added to result point
    for (double azimuth = 0; azimuth < 2 * M_PI * angle_iterations; azimuth += 2 * M_PI)
    {
            if (int(azimuth) == max_added_azimuth)
            break;

            if (points.find(int(azimuth)) != points.end())  // if there is a point for this azimuth
            {
            if (!point_before)
                first_azimuth = int(azimuth);

            // internal point for this azimuth
            result.push_back(get_point(int(azimuth), i, false));
            i++;
            point_before = true;
            continue;
            }

            // if there is no point for this azimuth
            if (!point_before)  // if there was no point before too, than we are beyond contour area
            continue;

            else  // means last time it was edge of contour area
            {
            point_before = false;
            for (double anticlockwise_azimuth = azimuth - 2 * M_PI; ; anticlockwise_azimuth -= 2 * M_PI)  // go behind (anticlockwise) for azimuth
            {
                if (points.find(int(anticlockwise_azimuth)) != points.end())  // if there is a point for this azimuth
                {
                    // external point for this azimuth
                    result.push_back(get_point(int(anticlockwise_azimuth), i, true));
                    i++;
                    if (int(anticlockwise_azimuth) == 0)
                        anticlockwise_azimuth = 2 * M_PI * angle_iterations;
                    continue;
                }

                // if there is no point for this azimuth
                max_added_azimuth = std::max(int(anticlockwise_azimuth + 2 * M_PI), max_added_azimuth);

                anticlockwise_azimuth += 2 * M_PI;
                if (int(anticlockwise_azimuth) == int(2 * M_PI * angle_iterations))
                    anticlockwise_azimuth = 0;

                for (double clockwise_azimuth = anticlockwise_azimuth; int(clockwise_azimuth) != first_azimuth; clockwise_azimuth += 2 * M_PI)
                {
                    if (int(clockwise_azimuth) == int(2 * M_PI * angle_iterations))
                    {
                        clockwise_azimuth = 0;
                        if (int(clockwise_azimuth) == first_azimuth)
                            break;
                    }
                    result.push_back(get_point(int(clockwise_azimuth), i, false));
                    i++;
                }
                result.push_back(get_point(first_azimuth, i, false));
                i++;
                break;
            }
            }
    }
    return result;
}

QVector<QCPCurveData> Contour::get_zero_data()
{
    QVector<QCPCurveData> result;

    int i = 0;  // parameter for QCPCurveData
    bool internal_before = false; // if there was inernal point in last iteration
    for (double azimuth = 0; azimuth < 2 * M_PI * angle_iterations; azimuth += 2 * M_PI)
    {
            QCPCurveData internal_point = get_point(int(azimuth), i, false);
            QCPCurveData external_point = get_point(int(azimuth), i, true);
            if (Point2d(internal_point.key, internal_point.value) != rls_position)  // if internal != default
            {
            if (Point2d(external_point.key, external_point.value) == rls_position)  // if external == default
            {
                internal_before = true;
                result.push_back(get_point(int(azimuth), i, false));   // internal point
                i++;
                continue;
            }

            // if external != default
            if (internal_before)
            {
                result.push_back(get_point(int(azimuth), i, false));  // internal point
                i++;
                result.push_back(get_point(int(azimuth), i, true));  // external point
                i++;
                internal_before = false;
            }
            else
            {
                result.push_back(get_point(int(azimuth), i, true));  // external point
                i++;
                result.push_back(get_point(int(azimuth), i, false));  // internal point
                i++;
                internal_before = true;
            }
            }
            else
            {
            result.push_back(get_point(int(azimuth), i, true));  // external point for this azimuth
            i++;
            }
    }
    QCPCurveData point = result[0];
    point.t = i;
    result.push_back(point);  // костыль
    return result;
}
