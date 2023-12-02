#include "test_map.h"
#include <QApplication>
#include "../lib/map.h"

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;
namespace EL = Elevation;
namespace VG = Vegetation;
namespace DP = DielectricPermittivity;

BOOST_AUTO_TEST_SUITE(test_map)

BOOST_AUTO_TEST_CASE(test_simple_map) {
    Point2d point1(2, 2);
    Point2d point2(2, 3);
    double h1 = 0;
    double h2 = 1;


    EL::Plain plain;
    VG::None veg;
    DP::Constant dp(1);

    Map map(&plain, &veg, &dp);
    BOOST_TEST(h1 == map.h(point1), tt::tolerance(1e-6));
    BOOST_TEST(h2 != map.h(point1), tt::tolerance(1e-6));
    BOOST_TEST(map.v(point1) == 0, tt::tolerance(1e-6));
    BOOST_TEST(map.dp(point1) == 1, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_GeoData) {
    Point2d point1(20, 2);
    Point2d point2(2, 2);
    Point2d point3(3, 3);
    double h1 = 13;
    double h2 = 0;
    double h3 = 2;

    EL::GeoData plain;
    VG::GeoData veg;
    DP::GeoData dp;

    Map map(&plain, &veg, &dp);
    BOOST_TEST(h1 == map.h(point1), tt::tolerance(1e-6));
    BOOST_TEST(h2 != map.h(point2), tt::tolerance(1e-6));
    BOOST_TEST(map.v(point2) == 1, tt::tolerance(1e-6));
    BOOST_TEST(map.v(point3) == 0, tt::tolerance(1e-6));
    BOOST_TEST(map.dp(point2) == 0, tt::tolerance(1e-6));
    BOOST_TEST(map.dp(point3) == 1, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_Map1d) {
    EL::GeoData plain;
    VG::GeoData veg;
    DP::GeoData dp;

    Map map(&plain, &veg, &dp);

    Point2d center(20, 0);
    double h = map.h(center);
    PointCartesian rls(center, h);
    PointCartesian target(30, 20, 20);

    Map1d map1d(&map, rls, target);
    double vege = map1d.vegetation(5);
    double pd = map1d.vegetation(5);
    double h_d = map1d.height(0);
    BOOST_TEST(h_d == 15, tt::tolerance(1e-6));
    BOOST_TEST(pd == 0, tt::tolerance(1e-6));
    BOOST_TEST(vege == 0, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_SUITE_END()
