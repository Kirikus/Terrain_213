#include "test_screening_angle.h"
#include "../lib/screenangle.h"
#include <QApplication>

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;


BOOST_AUTO_TEST_SUITE(ScreeningAngleSearch_tests)

BOOST_AUTO_TEST_CASE(ModelReliefFunction_test) {
    BOOST_TEST(ModelReliefFunction(20, 0) == 15, tt::tolerance(1e-6));
    BOOST_TEST(ModelReliefFunction(10, 0) == 5, tt::tolerance(1e-6));
    BOOST_TEST(ModelReliefFunction(20, 5) == 10, tt::tolerance(1e-6));
}

PointCartesian rls1(0, 0, 0);
PointCartesian target1(1, 0, 0);

PointSpheric sp1(rls1, target1);

BOOST_AUTO_TEST_CASE(FindScreeningAngle_simple_test) {
    BOOST_TEST(FindScreeningAngle(sp1, hypot(10, 5)) == atan(5./10), tt::tolerance(1e-4));
    BOOST_TEST(FindScreeningAngle(sp1, hypot(15, 20)) == atan(15./20), tt::tolerance(1e-4));
    BOOST_TEST(FindScreeningAngle(sp1, 30) == atan(15./20), tt::tolerance(1e-4));
}

PointCartesian rls2(10, 0, ModelReliefFunction(10, 0));
PointCartesian rls3(17, 4, ModelReliefFunction(17, 4));
PointCartesian rls4(23, 4, ModelReliefFunction(23, 4));
PointCartesian rls5(35, 0, ModelReliefFunction(35, 0));

PointCartesian target2(11, 0, ModelReliefFunction(10, 0));
PointCartesian target3(18, 4, ModelReliefFunction(17, 4));
PointCartesian target4(24, 4, ModelReliefFunction(23, 4));
PointCartesian target5(36, 0, ModelReliefFunction(35, 0));


PointSpheric sp2(rls2, target2);
PointSpheric sp3(rls3, target3);
PointSpheric sp4(rls4, target4);
PointSpheric sp5(rls5, target5);

BOOST_AUTO_TEST_CASE(FindScreeningAngle_on_mountain_test) {
    sp2.change_azimuth(0);
    sp3.change_azimuth(0);
    sp4.change_azimuth(M_PI);
    sp5.change_azimuth(M_PI);

    double incline3_4 = 0.48568956;

    BOOST_TEST(FindScreeningAngle(sp2, 20) == atan(1), tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(sp2, 30) == atan(1), tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(sp2, 0) == 0., tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(sp3, hypot(3, 1)) == incline3_4, tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(sp4, hypot(3, 1)) == incline3_4, tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(sp5, hypot(20, 15)) == atan(1), tt::tolerance(1e-6));
}


BOOST_AUTO_TEST_SUITE_END()
