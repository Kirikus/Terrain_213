#include "test_screening_angle.h"
#include "../lib/screenangle.h"
#include "../lib/map.h"
#include "../lib/models_earth.h"
#include <QApplication>

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;
namespace EL = Elevation;
namespace VG = Vegetation;
namespace DP = DielectricPermittivity;
namespace EM = EarthModels;
namespace CD = Conductivity;

BOOST_AUTO_TEST_SUITE(ScreeningAngleSearch_tests)

EL::GeoData geo;
VG::None veg;
DP::Constant dp(1);
CD::Constant c(0);

Map map(&geo, &veg, &dp, &c);

EM::ModelFlat model1;
EM::ModelSpheric model2;
EM::ModelEffectiveRadius model3;

BOOST_AUTO_TEST_CASE(FindScreeningAngle_simple_test) {
    PointCartesian rls1(0, 0, 0);
    PointCartesian target1(1, 0, 0);
    PointSpheric sp1(rls1, target1);

    BOOST_TEST(FindScreeningAngle(&map, &model1, sp1, hypot(10, 5)) == atan(5./10), tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(&map, &model1, sp1, hypot(15, 20)) == atan(15./20), tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(&map, &model1, sp1, 30) == atan(15./20), tt::tolerance(1e-6));

    // In non-planar models, the difference will be already in 5-6 characters
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp1, hypot(10, 5)) == atan(5./10), tt::tolerance(1e-4));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp1, hypot(15, 20)) == atan(15./20), tt::tolerance(1e-4));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp1, 30) == atan(15./20), tt::tolerance(1e-4));

    BOOST_TEST(FindScreeningAngle(&map, &model3, sp1, hypot(10, 5)) == atan(5./10), tt::tolerance(1e-4));
    BOOST_TEST(FindScreeningAngle(&map, &model3, sp1, hypot(15, 20)) == atan(15./20), tt::tolerance(1e-4));
    BOOST_TEST(FindScreeningAngle(&map, &model3, sp1, 30) == atan(15./20), tt::tolerance(1e-4));
}

BOOST_AUTO_TEST_CASE(FindScreeningAngle_on_mountain_test) {
    Point2d p1(10, 0);
    Point2d p2(17, 4);
    Point2d p3(23, 4);
    Point2d p4(35, 0);

    PointCartesian rls2(10, 0, map.h(&p1));
    PointCartesian rls3(17, 4, map.h(&p2));
    PointCartesian rls4(23, 4, map.h(&p3));
    PointCartesian rls5(35, 0, map.h(&p4));

    PointCartesian target2(11, 0, map.h(&p1));
    PointCartesian target3(18, 4, map.h(&p2));
    PointCartesian target4(24, 4, map.h(&p3));
    PointCartesian target5(36, 0, map.h(&p4));

    PointSpheric sp2(rls2, target2);
    PointSpheric sp3(rls3, target3);
    PointSpheric sp4(rls4, target4);
    PointSpheric sp5(rls5, target5);

    sp2.change_azimuth(0);
    sp3.change_azimuth(0);
    sp4.change_azimuth(M_PI);
    sp5.change_azimuth(M_PI);

    double incline3_4 = 0.48568956;

    BOOST_TEST(FindScreeningAngle(&map, &model1, sp2, 20) == atan(1), tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(&map, &model1, sp2, 30) == atan(1), tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(&map, &model1, sp2, 0) == 0., tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(&map, &model1, sp3, hypot(3, 1)) == incline3_4, tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(&map, &model1, sp4, hypot(3, 1)) == incline3_4, tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(&map, &model1, sp5, hypot(20, 15)) == atan(1), tt::tolerance(1e-6));

    // In non-planar models, the difference will be already in 2-3 characters
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp2, 20) == atan(1), tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp2, 30) == atan(1), tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp2, 0) == 0., tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp3, hypot(3, 1)) == incline3_4, tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp4, hypot(3, 1)) == incline3_4, tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp5, hypot(20, 15)) == atan(1), tt::tolerance(1e-2));

    BOOST_TEST(FindScreeningAngle(&map, &model2, sp2, 20) == atan(1), tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp2, 30) == atan(1), tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp2, 0) == 0., tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp3, hypot(3, 1)) == incline3_4, tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp4, hypot(3, 1)) == incline3_4, tt::tolerance(1e-2));
    BOOST_TEST(FindScreeningAngle(&map, &model2, sp5, hypot(20, 15)) == atan(1), tt::tolerance(1e-2));
}

BOOST_AUTO_TEST_SUITE_END()
