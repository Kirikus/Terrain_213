#include "test_earth_models.h"

#include <QApplication>

#include "../lib/earth_models.h"

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(test_earth_models)

BOOST_AUTO_TEST_CASE(test_ModelFlat_short_distance) {
    ModelFlat fe;

    PointCartesian center(2, 2, 4);
    PointCartesian target(60, 60, 100);

    double x = target.get_x() - center.get_x();
    double y = target.get_y() - center.get_y();
    double h = target.get_h() - center.get_h();

    double r = sqrt(pow(x, 2) + pow(y, 2) + pow(h, 2));
    BOOST_TEST(fe.find_r(center, target) == r, tt::tolerance(1e-6));
    BOOST_TEST(fe.find_phi(center, target) == asin(h/r), tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_ModelSpheric_short_distance) {
    ModelSpheric se;

    PointCartesian center(2, 2, 4);
    PointCartesian target(60, 60, 100);

    double r = 126.269986;
    double phi = 0.8637309;

    BOOST_TEST(se.find_r(center, target) == r, tt::tolerance(1e-6));
    BOOST_TEST(se.find_phi(center, target) == phi, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_ModelEffectiveRadius_short_distance) {
    ModelEffectiveRadius ere;

    PointCartesian center(2, 2, 4);
    PointCartesian target(60, 60, 100);

    double r = 126.269986;
    double phi = 0.8637331;

    BOOST_TEST(ere.find_r(center, target) == r, tt::tolerance(1e-6));
    BOOST_TEST(ere.find_phi(center, target) == phi, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_ModelFlat_long_distance) {
    ModelFlat fe;

    PointCartesian center(2, 2, 4);
    PointCartesian target(60000, 6000, 1000);

    double x = target.get_x() - center.get_x();
    double y = target.get_y() - center.get_y();
    double h = target.get_h() - center.get_h();

    double r = sqrt(pow(x, 2) + pow(y, 2) + pow(h, 2));
    BOOST_TEST(fe.find_r(center, target) == r, tt::tolerance(1e-6));
    BOOST_TEST(fe.find_phi(center, target) == asin(h/r), tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_ModelSpheric_long_distance) {
    ModelSpheric se;

    PointCartesian center(2, 2, 4);
    PointCartesian target(60000, 6000, 1000);

    double r = 60310.0352163;
    double phi = 0.01178825;

    BOOST_TEST(se.find_r(center, target) == r, tt::tolerance(1e-6));
    BOOST_TEST(se.find_phi(center, target) == phi, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_SUITE_END()
