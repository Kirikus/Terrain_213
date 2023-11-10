#include "test_point.h"

#include <QApplication>

#include "../lib/point.h"

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(test_point)

CartesianPoint center(0, 0, 0);
CartesianPoint target(2, 2, 2);

BOOST_AUTO_TEST_CASE(SphericPoint_common_creation) {


    SphericPoint sp(center, target);
    BOOST_TEST(sp.get_R() == 2*sqrt(3), tt::tolerance(1e-6));
    BOOST_TEST(sp.get_azimuth() == 45, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(SphericPoint_copy_creation) {

    SphericPoint sp(center, target);
    SphericPoint copied_sp(sp);

    BOOST_TEST(sp.get_R() == copied_sp.get_R(), tt::tolerance(1e-6));
    BOOST_TEST(sp.get_azimuth() == copied_sp.get_azimuth(), tt::tolerance(1e-6));
    BOOST_TEST(sp.get_phi() == copied_sp.get_phi(), tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_SUITE_END()
