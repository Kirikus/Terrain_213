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
namespace CD = Conductivity;

BOOST_AUTO_TEST_SUITE(test_map)

BOOST_AUTO_TEST_CASE(test_DielectricPermittivity) {
    DP::Constant const_dp(4);
    DP::GeoData Geo_data_dp;

    Point2d test1(3, 3);
    Point2d test2(4, 2);
    Point2d test3(20, 0);

    BOOST_TEST(Geo_data_dp.dielectricPermittivity(&test1) == 0, tt::tolerance(1e-6));
    BOOST_TEST(Geo_data_dp.dielectricPermittivity(&test2) == 0, tt::tolerance(1e-6));
    BOOST_TEST(Geo_data_dp.dielectricPermittivity(&test3) == 15, tt::tolerance(1e-6));
    BOOST_TEST(const_dp.dielectricPermittivity(&test1) == 4, tt::tolerance(1e-6));
    BOOST_TEST(const_dp.dielectricPermittivity(&test2) == 4, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_Conductivity) {
    CD::Constant const_dp(4);
    CD::GeoData Geo_data_dp;

    Point2d test1(3, 3);
    Point2d test2(4, 2);
    Point2d test3(20, 0);

    BOOST_TEST(Geo_data_dp.conductivity(&test1) == 0, tt::tolerance(1e-6));
    BOOST_TEST(Geo_data_dp.conductivity(&test2) == 0, tt::tolerance(1e-6));
    BOOST_TEST(Geo_data_dp.conductivity(&test3) == 15, tt::tolerance(1e-6));
    BOOST_TEST(const_dp.conductivity(&test1) == 4, tt::tolerance(1e-6));
    BOOST_TEST(const_dp.conductivity(&test2) == 4, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_Elevation) {
    EL::Plain plain_el;
    EL::GeoData geo_data_el;

    Point2d test1(20, 3);
    Point2d test2(16, 3);

    BOOST_TEST(geo_data_el.h(&test1) == 12, tt::tolerance(1e-6));
    BOOST_TEST(geo_data_el.h(&test2) == 10, tt::tolerance(1e-6));
    BOOST_TEST(plain_el.h(&test1) == 0, tt::tolerance(1e-6));
    BOOST_TEST(plain_el.h(&test2) == 0, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_Vegetation) {
    VG::None non_veg;
    VG::Constant constant_veg(Vegetation::VegetationType::Shrub);
    VG::GeoData geo_data_veg;

    Point2d test1(2, 3);
    Point2d test2(16, 3);

    BOOST_CHECK(non_veg.vegetation(&test1) == Vegetation::VegetationType::None);
    BOOST_CHECK(non_veg.vegetation(&test2) == Vegetation::VegetationType::None);
    BOOST_CHECK(constant_veg.vegetation(&test1) == Vegetation::VegetationType::Shrub);
    BOOST_CHECK(constant_veg.vegetation(&test2) == Vegetation::VegetationType::Shrub);
    BOOST_CHECK(geo_data_veg.vegetation(&test1) == Vegetation::VegetationType::Grass);
    BOOST_CHECK(geo_data_veg.vegetation(&test2) == Vegetation::VegetationType::Forest);
}

BOOST_AUTO_TEST_CASE(test_simple_map) {
    Point2d point1(2, 2);
    Point2d point2(2, 3);
    double h1 = 0;
    double h2 = 1;

    int veg_none = 0;

    EL::Plain plain;
    VG::None veg;
    DP::Constant dp(1);
    CD::Constant c(0);

    Map map(&plain, &veg, &dp, &c);
    BOOST_TEST(h1 == map.h(&point1), tt::tolerance(1e-6));
    BOOST_TEST(h2 != map.h(&point1), tt::tolerance(1e-6));
    BOOST_CHECK(map.v(&point1) == Vegetation::VegetationType::None);
    BOOST_TEST(map.dp(&point1) == 1, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_GeoData) {
    Point2d point1(20, 2);
    Point2d point2(2, 2);
    Point2d point3(3, 3);
    Point2d point4(20, 0);
    double h1 = 13;
    double h2 = 0;
    double h3 = 2;

    int veg_grass = 1;

    EL::GeoData plain;
    VG::GeoData veg;
    DP::GeoData dp;
    CD::Constant c(0);

    Map map(&plain, &veg, &dp, &c);
    BOOST_TEST(h1 == map.h(&point1), tt::tolerance(1e-6));
    BOOST_TEST(h2 == map.h(&point2), tt::tolerance(1e-6));
    BOOST_CHECK(map.v(&point2) == Vegetation::VegetationType::Grass);
    BOOST_CHECK(map.v(&point3) == Vegetation::VegetationType::Grass);
    BOOST_TEST(map.dp(&point2) == 0, tt::tolerance(1e-6));
    BOOST_TEST(map.dp(&point3) == 0, tt::tolerance(1e-6));
    BOOST_TEST(map.dp(&point4) == 15, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_Map1d) {
    EL::GeoData geo;
    EL::Plain plain;
    VG::GeoData veg;
    DP::GeoData dp;
    CD::Constant c(0);

    Map map(&geo, &veg, &dp, &c);
    Map map2(&plain, &veg, &dp, &c);

    Point2d center(20, 0);
    double h = map.h(&center);
    PointCartesian rls(center, h);
    PointCartesian target(30, 20, 20);

    Map1d map1d(&map, rls, target);
    Map1d map1d2(&map2, rls, target);

    int veg_forest = 3;

    Vegetation::VegetationType vege = map1d.vegetation(5);
    Vegetation::VegetationType pd = map1d.vegetation(5);
    double h_d = map1d.height(0);
    BOOST_TEST(h_d == 15, tt::tolerance(1e-6));
    BOOST_CHECK(pd == Vegetation::VegetationType::Forest);
    BOOST_CHECK(vege == Vegetation::VegetationType::Forest);

    double h_d2 = map1d.height(10);
    double h_d3 = map1d.height(5);
    double h_d4 = map1d.height(2);
    BOOST_TEST(h_d2 == 5, tt::tolerance(1e-6));
    BOOST_TEST(h_d3 == 10, tt::tolerance(1e-6));
    BOOST_TEST(h_d4 == 13, tt::tolerance(1e-6));

    double h_d5 = map1d2.height(5);
    BOOST_TEST(h_d5 == 0, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_SUITE_END()
