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

BOOST_AUTO_TEST_CASE(simple_map) {
    PointCartesian point1(2, 2, 0);
    PointCartesian point2(2, 2, 2);

    EL::Plain plain;
    VG::None veg;
    DP::Constant dp(1);

    Map map(&plain, &veg, &dp);
    BOOST_TEST(point1.get_h() == map.h(point1), tt::tolerance(1e-6));
    BOOST_TEST(point2.get_h() != map.h(point1), tt::tolerance(1e-6));
    BOOST_TEST(map.v(point1) == 0, tt::tolerance(1e-6));
    BOOST_TEST(map.dp(point1) == 1, tt::tolerance(1e-6));
}


BOOST_AUTO_TEST_SUITE_END()
