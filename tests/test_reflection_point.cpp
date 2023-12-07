#include "test_reflection_point.h"
#include <QApplication>
#include "../lib/reflection_point.h"

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;
namespace EL = Elevation;
namespace VG = Vegetation;
namespace DP = DielectricPermittivity;
namespace RP = ReflectionPoint;

BOOST_AUTO_TEST_SUITE(test_reflection_point)

BOOST_AUTO_TEST_CASE(test_null_derivative) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::Plain plain;

    Map map(&plain, &non_veg, &const_dp);

    PointCartesian rls(3, 3, 0);
    PointCartesian reflection_point(4, 2, 0);
    PointSpheric rp(rls, reflection_point);
    double derivative = RP::FindDerivative(rp, &map);

    BOOST_TEST(derivative == 0, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_GeoData_simple_derivative) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::GeoData GeoData;

    Map map(&GeoData, &non_veg, &const_dp);

    PointCartesian rls(0, 0, 0);
    PointCartesian reflection_point(16, 0, 11);
    PointSpheric rp(rls, reflection_point);
    double derivative = RP::FindDerivative(rp, &map);

    BOOST_TEST(derivative == 1, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_GeoData_derivative) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::GeoData GeoData;

    Map map(&GeoData, &non_veg, &const_dp);

    double delta_d = 0.001;
    PointCartesian rls(0, 0, 0);
    PointCartesian reflection_point(17, 4, 10);
    PointSpheric rp(rls, reflection_point);
    rp.change_d(rp.get_d() + delta_d);

    Point2d rp2d(17, 4);
    Point2d rp2d2(rp.get_target().get_x(), rp.get_target().get_y());

    double theory_derivative = (map.h(rp2d2) - map.h(rp2d))/delta_d;
    rp.change_d(rp.get_d() - delta_d);
    double derivative = RP::FindDerivative(rp, &map);

    BOOST_TEST(derivative == theory_derivative, tt::tolerance(1e-6));
}


BOOST_AUTO_TEST_CASE(test_simple_FindPhi) {
    double k1 = 0;
    double k2 = 2;
    double k3 = -2;

    BOOST_TEST(std::abs(RP::FindPhi(k1, k2)) == std::abs(RP::FindPhi(k1, k3)), tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_SUITE_END()
