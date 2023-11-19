#include "test_point.h"

#include <QApplication>

#include "../lib/point.h"

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(test_point)

BOOST_AUTO_TEST_CASE(PointCartesian_common_creation) {
    PointCartesian pc_test(1.5, 98, 100.21);
    BOOST_TEST(pc_test.get_x() == 1.5, tt::tolerance(1e-6));
    BOOST_TEST(pc_test.get_y() == 98, tt::tolerance(1e-6));
    BOOST_TEST(pc_test.get_h() == 100.21, tt::tolerance(1e-6));
}

PointCartesian center(0, 0, 0);
PointCartesian target1(2, 2, 2);
PointCartesian target2(-2, 2, 2);
PointCartesian target3(-2, -2, -2);
PointCartesian target4(2, -2, -2);

BOOST_AUTO_TEST_CASE(PointSpheric_common_creation) {
    PointSpheric sp1(center, target1);
    BOOST_TEST(sp1.get_R() == 2*sqrt(3), tt::tolerance(1e-6));
    BOOST_TEST(sp1.get_azimuth() == 45 * M_PI / 180, tt::tolerance(1e-6));
    BOOST_TEST(sp1.get_phi() == atan(1/sqrt(2)), tt::tolerance(1e-6));

    PointSpheric sp2(center, target2);
    BOOST_TEST(sp2.get_R() == 2*sqrt(3), tt::tolerance(1e-6));
    BOOST_TEST(sp2.get_azimuth() == 135 * M_PI / 180, tt::tolerance(1e-6));
    BOOST_TEST(sp2.get_phi() == atan(1/sqrt(2)), tt::tolerance(1e-6));

    PointSpheric sp3(center, target3);
    BOOST_TEST(sp3.get_R() == 2*sqrt(3), tt::tolerance(1e-6));
    BOOST_TEST(sp3.get_azimuth() == 225 * M_PI / 180, tt::tolerance(1e-6));
    BOOST_TEST(sp3.get_phi() == atan(-1/sqrt(2)), tt::tolerance(1e-6));

    PointSpheric sp4(center, target4);
    BOOST_TEST(sp4.get_R() == 2*sqrt(3), tt::tolerance(1e-6));
    BOOST_TEST(sp4.get_azimuth() == 315 * M_PI / 180, tt::tolerance(1e-6));
    BOOST_TEST(sp4.get_phi() == atan(-1/sqrt(2)), tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(PointSpheric_copy_creation) {
    PointSpheric sp(center, target1);
    PointSpheric copied_sp(sp);

    BOOST_TEST(sp.get_R() == copied_sp.get_R(), tt::tolerance(1e-6));
    BOOST_TEST(sp.get_azimuth() == copied_sp.get_azimuth(), tt::tolerance(1e-6));
    BOOST_TEST(sp.get_phi() == copied_sp.get_phi(), tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(PointSpheric_coord_get) {
    PointSpheric sp(center, target1);

    BOOST_TEST(sp.get_x() == 2, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_y() == 2, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_h() == 2, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_d() == 2*sqrt(2), tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(PointSpheric_center_get) {
    PointSpheric sp(center, target1);

    BOOST_TEST(sp.get_center().get_x() == 0, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_center().get_y() == 0, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_center().get_h() == 0, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_d() == 2*sqrt(2), tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(PointSpheric_change_r) {
    PointSpheric sp(center, target1);

    sp.change_r(3*sqrt(3));
    BOOST_TEST(sp.get_x() == 3, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_y() == 3, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_h() == 3, tt::tolerance(1e-6));

    sp.change_r(4*sqrt(3));
    BOOST_TEST(sp.get_x() == 4, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_y() == 4, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_h() == 4, tt::tolerance(1e-6));
}
BOOST_AUTO_TEST_CASE(PointSpheric_move_target) {
    PointSpheric sp(center, target1);
    sp.move_target(sp.get_x() + 1, sp.get_y() + 2, sp.get_h() + 3);

    BOOST_TEST(sp.get_x() == 3, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_y() == 4, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_h() == 5, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_d() == 5, tt::tolerance(1e-6));
    BOOST_TEST(sp.get_R() == 5*sqrt(2), tt::tolerance(1e-6));
    BOOST_TEST(sp.get_azimuth() == acos(3./5), tt::tolerance(1e-6));
    BOOST_TEST(sp.get_phi() == 0.78539, tt::tolerance(1e-4));
}

BOOST_AUTO_TEST_SUITE_END()
