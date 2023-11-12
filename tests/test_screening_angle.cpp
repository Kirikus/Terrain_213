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
    BOOST_TEST(ModelReliefFunction(20, 0) = 15, tt::tolerance(1e-6));
    BOOST_TEST(ModelReliefFunction(10, 0) = 5, tt::tolerance(1e-6));
    BOOST_TEST(ModelReliefFunction(20, 5) = 10, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(FindCurrAngle_test) {
    BOOST_TEST(FindCurrAngle(0, 0, 0, 20, 0, 15) = 0.6435011088, tt::tolerance(1e-6));
    BOOST_TEST(FindCurrAngle(0, 0, 0, 10, 0, 5) = 0.463647609, tt::tolerance(1e-6));
    BOOST_TEST(FindCurrAngle(0, 0, 0, 17, 4, 5) = 0.2788403149, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(FindScreeningAngle_simple_test) {
    BOOST_TEST(FindScreeningAngle(0, 10) = 0.463647609, tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(0, 20) = 0.6435011088, tt::tolerance(1e-6));
    BOOST_TEST(FindScreeningAngle(0, 30) = 0.6435011088, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_SUITE_END()
