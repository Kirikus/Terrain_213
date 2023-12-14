#include "test_height_determination.h"

#include <QApplication>

#include "../lib/height_determination.h"

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;
namespace EM = EarthModels;
namespace RP = ReflectionPoint;

BOOST_AUTO_TEST_SUITE(test_height_determination)

EM::ModelFlat fm;
EM::ModelSpheric spm;
EM::ModelEffectiveRadius efrm;

BOOST_AUTO_TEST_CASE(test_plain_height_determination) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::Plain plain;
    CD::Constant c(0);

    Map map(&plain, &non_veg, &const_dp, &c);

    PointCartesian center1(0, 0, 4);
    Point2d target1(8, 0);

    PointCartesian center2(2, 2, 2);
    Point2d target2(6, 6);

    double h1_fm = FindTargetHeight(&map, &fm, center1, target1, 8, 4 * std::sqrt(2), 4 * std::sqrt(2), 10);
    double h1_spm = FindTargetHeight(&map, &spm, center1, target1, 8, 4 * std::sqrt(2), 4 * std::sqrt(2), 10);
    double h1_efrm = FindTargetHeight(&map, &efrm, center1, target1, 8, 4 * std::sqrt(2), 4 * std::sqrt(2), 10);

    double h2_fm = FindTargetHeight(&map, &fm, center2, target2, 4 * std::sqrt(2), 2 * sqrt(3), 2 * sqrt(3), 10);
    double h2_spm = FindTargetHeight(&map, &spm, center2, target2, 4 * std::sqrt(2), 2 * sqrt(3), 2 * sqrt(3), 10);
    double h2_efrm = FindTargetHeight(&map, &efrm, center2, target2, 4 * std::sqrt(2), 2 * sqrt(3), 2 * sqrt(3), 10);


    BOOST_TEST(h1_fm == 4, tt::tolerance(1e-4));
    BOOST_TEST(h1_spm == 4, tt::tolerance(1e-4));
    BOOST_TEST(h1_efrm == 4, tt::tolerance(1e-4));

    BOOST_TEST(h2_fm == 2, tt::tolerance(1e-2));
    BOOST_TEST(h2_spm == 2, tt::tolerance(1e-2));
    BOOST_TEST(h2_efrm == 2, tt::tolerance(1e-2));
}

BOOST_AUTO_TEST_CASE(test_geo_data_height_determination) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::GeoData geo_data;
    CD::Constant c(0);

    Map map(&geo_data, &non_veg, &const_dp, &c);

    // Finding r, r1, r2:
    PointCartesian center1(0, 0, 0);
    PointCartesian target1(20, 0, 40);

    PointSpheric sp1(center1, target1);
    PointCartesian reflection_point1_fm = RP::FindReflectionPoint(sp1, &map, &fm);
    PointCartesian reflection_point1_spm = RP::FindReflectionPoint(sp1, &map, &spm);
    PointCartesian reflection_point1_efrm = RP::FindReflectionPoint(sp1, &map, &efrm);

    PointSpheric sp_c(center1, reflection_point1_fm);
    PointSpheric sp_t(target1, reflection_point1_fm);

    double r_fm = fm.find_r(sp1.get_center(), sp1.get_target());
    double r1_fm = fm.find_r(sp_c.get_center(), sp_c.get_target());
    double r2_fm = fm.find_r(sp_t.get_center(), sp_t.get_target());

    double r_spm = spm.find_r(sp1.get_center(), sp1.get_target());
    double r1_spm = spm.find_r(sp_c.get_center(), sp_c.get_target());
    double r2_spm = spm.find_r(sp_t.get_center(), sp_t.get_target());

    double r_efrm = efrm.find_r(sp1.get_center(), sp1.get_target());
    double r1_efrm = efrm.find_r(sp_c.get_center(), sp_c.get_target());
    double r2_efrm = efrm.find_r(sp_t.get_center(), sp_t.get_target());

    Point2d target2d(target1.get_x(), target1.get_y());

    double h1_fm = FindTargetHeight(&map, &fm, center1, target2d, r_fm, r1_fm, r2_fm, r1_fm + r2_fm + r_fm);
    double h1_spm = FindTargetHeight(&map, &spm, center1, target2d, r_spm, r1_spm, r2_spm, r1_spm + r2_spm + r_spm);
    double h1_efrm = FindTargetHeight(&map, &efrm, center1, target2d, r_efrm, r1_efrm, r2_efrm, r1_efrm + r2_efrm + r_efrm);

    BOOST_TEST(h1_fm == 40, tt::tolerance(1e-4));
    BOOST_TEST(h1_spm == 40, tt::tolerance(1e-4));
    BOOST_TEST(h1_efrm == 40, tt::tolerance(1e-4));
}



BOOST_AUTO_TEST_SUITE_END()
