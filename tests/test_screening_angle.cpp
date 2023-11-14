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

BOOST_AUTO_TEST_CASE(FindScreeningAngle_simple_test) {
    BOOST_TEST(FindScreeningAngle(rls1, 0, hypot(10, 5)) == atan(5./10), tt::tolerance(1e-4));
    BOOST_TEST(FindScreeningAngle(rls1, 0, hypot(15, 20)) == atan(15./20), tt::tolerance(1e-4));
    BOOST_TEST(FindScreeningAngle(rls1, 0, 30) == atan(15./20), tt::tolerance(1e-4));
}

PointCartesian rls2(10, 0, ModelReliefFunction(10, 0));
PointCartesian rls3(17, 4, ModelReliefFunction(17, 4));
PointCartesian rls4(23, 4, ModelReliefFunction(23, 4));
PointCartesian rls5(35, 0, ModelReliefFunction(35, 0));

BOOST_AUTO_TEST_CASE(FindScreeningAngle_on_mountain_test) {
    BOOST_TEST(FindScreeningAngle(rls2, 0, 20) == atan(1), tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(rls2, 0, 30) == atan(1), tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(rls2, 0, 0) == 0., tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(rls3, 0, hypot(3, 1)) == 0.5403724, tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(rls4, M_PI, hypot(3, 1)) == 0.5403724, tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(rls5, M_PI, hypot(20, 15)) == atan(1), tt::tolerance(1e-6));
}


BOOST_AUTO_TEST_SUITE_END()
