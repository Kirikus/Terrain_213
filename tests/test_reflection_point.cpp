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
namespace EM = EarthModels;

BOOST_AUTO_TEST_SUITE(test_reflection_point)

EM::ModelFlat fm;
EM::ModelSpheric spm;
EM::ModelEffectiveRadius efrm;

BOOST_AUTO_TEST_CASE(test_null_derivative) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::Plain plain;
    CD::Constant c(0);

    Map map(&plain, &non_veg, &const_dp, &c);

    PointCartesian rls(3, 3, 0);
    PointCartesian reflection_point(4, 2, 0);
    PointSpheric rp(rls, reflection_point);
    double derivative = RP::FindDerivative(rp, &map);

    BOOST_TEST(derivative == 0, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_GeoData_simple_derivative) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::GeoData geo_data;
    CD::Constant c(0);

    Map map(&geo_data, &non_veg, &const_dp, &c);

    PointCartesian rls(0, 0, 0);
    PointCartesian reflection_point(16, 0, 11);
    PointSpheric rp(rls, reflection_point);
    double derivative = RP::FindDerivative(rp, &map);

    BOOST_TEST(derivative == 1, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_GeoData_derivative) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::GeoData geo_data;
    CD::Constant c(0);

    Map map(&geo_data, &non_veg, &const_dp, &c);

    double delta_d = 0.001;
    PointCartesian rls(0, 0, 0);
    PointCartesian reflection_point(17, 4, 10);
    PointSpheric rp(rls, reflection_point);
    rp.change_d(rp.get_d() + delta_d);

    Point2d rp2d(17, 4);
    Point2d rp2d2(rp.get_target().get_x(), rp.get_target().get_y());

    double theory_derivative = (map.h(&rp2d2) - map.h(&rp2d))/delta_d;
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

BOOST_AUTO_TEST_CASE(test_simple_FindReflectionPoint) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::Plain plain;
    CD::Constant c(0);

    Map map(&plain, &non_veg, &const_dp, &c);

    PointCartesian rls1(0, 0, 2);
    PointCartesian target1(4, 0, 2);
    PointSpheric sp1(rls1, target1);
    PointCartesian reflection_point1_fm = RP::FindReflectionPoint(sp1, &map, &fm);
    PointCartesian reflection_point1_spm = RP::FindReflectionPoint(sp1, &map, &spm);
    PointCartesian reflection_point1_efrm = RP::FindReflectionPoint(sp1, &map, &efrm);

    PointCartesian rls2(2*sqrt(2), 2*sqrt(2), 3);
    PointCartesian target2(4*sqrt(2), 4*sqrt(2), 3);
    PointSpheric sp2(rls2, target2);
    PointCartesian reflection_point2_fm = RP::FindReflectionPoint(sp2, &map, &fm);
    PointCartesian reflection_point2_spm = RP::FindReflectionPoint(sp2, &map, &spm);
    PointCartesian reflection_point2_efrm = RP::FindReflectionPoint(sp2, &map, &efrm);

    PointCartesian rls3(5, 5, 3);
    PointCartesian target3(5, 1, 3);
    PointSpheric sp3(rls3, target3);
    PointCartesian reflection_point3_fm = RP::FindReflectionPoint(sp3, &map, &fm);
    PointCartesian reflection_point3_spm = RP::FindReflectionPoint(sp3, &map, &spm);
    PointCartesian reflection_point3_efrm = RP::FindReflectionPoint(sp3, &map, &efrm);

    BOOST_TEST(reflection_point1_fm.get_x() == 2, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_fm.get_y() == 0, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_fm.get_h() == 0, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point1_spm.get_x() == 2, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_spm.get_y() == 0, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_spm.get_h() == 0, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point1_efrm.get_x() == 2, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_efrm.get_y() == 0, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_efrm.get_h() == 0, tt::tolerance(1e-4));



    BOOST_TEST(reflection_point2_fm.get_x() == 3*sqrt(2), tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_fm.get_y() == 3*sqrt(2), tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_fm.get_h() == 0, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point2_spm.get_x() == 3*sqrt(2), tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_spm.get_y() == 3*sqrt(2), tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_spm.get_h() == 0, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point2_efrm.get_x() == 3*sqrt(2), tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_efrm.get_y() == 3*sqrt(2), tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_efrm.get_h() == 0, tt::tolerance(1e-4));



    BOOST_TEST(reflection_point3_fm.get_x() == 5, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_fm.get_y() == 3, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_fm.get_h() == 0, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point3_spm.get_x() == 5, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_spm.get_y() == 3, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_spm.get_h() == 0, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point3_efrm.get_x() == 5, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_efrm.get_y() == 3, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_efrm.get_h() == 0, tt::tolerance(1e-4));
}

BOOST_AUTO_TEST_CASE(test_GeoData_FindReflectionPoint) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::GeoData geo_data;
    CD::Constant c(0);

    Map map(&geo_data, &non_veg, &const_dp, &c);

    PointCartesian rls1(0, 0, 0);
    PointCartesian rls2(20, 20, 0);
    PointCartesian rls3(20, 0, 40);
    PointCartesian target1(20, 0, 40);
    PointCartesian target2(0, 0, 0);
    PointSpheric sp1(rls1, target1);
    PointSpheric sp2(rls2, target1);
    PointSpheric sp3(rls3, target2);

    PointCartesian reflection_point1_fm = RP::FindReflectionPoint(sp1, &map, &fm);
    PointCartesian reflection_point2_fm = RP::FindReflectionPoint(sp2, &map, &fm);
    PointCartesian reflection_point3_fm = RP::FindReflectionPoint(sp3, &map, &fm);

    PointCartesian reflection_point1_spm = RP::FindReflectionPoint(sp1, &map, &spm);
    PointCartesian reflection_point2_spm = RP::FindReflectionPoint(sp2, &map, &spm);
    PointCartesian reflection_point3_spm = RP::FindReflectionPoint(sp3, &map, &spm);

    PointCartesian reflection_point1_efrm = RP::FindReflectionPoint(sp1, &map, &efrm);
    PointCartesian reflection_point2_efrm = RP::FindReflectionPoint(sp2, &map, &efrm);
    PointCartesian reflection_point3_efrm = RP::FindReflectionPoint(sp3, &map, &efrm);

    double reflection_x1 = 7.5;
    double reflection_y1 = 0;
    double reflection_h1 = 2.5;

    double reflection_x2 = 20;
    double reflection_y2 = 12.5;
    double reflection_h2 = 2.5;

    double reflection_x3 = reflection_x1;
    double reflection_y3 = reflection_y1;
    double reflection_h3 = reflection_h1;

    BOOST_TEST(reflection_point1_fm.get_x() == reflection_x1, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_fm.get_y() == reflection_y1, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_fm.get_h() == reflection_h1, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point1_spm.get_x() == reflection_x1, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_spm.get_y() == reflection_y1, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_spm.get_h() == reflection_h1, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point1_efrm.get_x() == reflection_x1, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_efrm.get_y() == reflection_y1, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point1_efrm.get_h() == reflection_h1, tt::tolerance(1e-4));



    BOOST_TEST(reflection_point2_fm.get_x() == reflection_x2, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_fm.get_y() == reflection_y2, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_fm.get_h() == reflection_h2, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point2_spm.get_x() == reflection_x2, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_spm.get_y() == reflection_y2, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_spm.get_h() == reflection_h2, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point2_efrm.get_x() == reflection_x2, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_efrm.get_y() == reflection_y2, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point2_efrm.get_h() == reflection_h2, tt::tolerance(1e-4));



    BOOST_TEST(reflection_point3_fm.get_x() == reflection_x3, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_fm.get_y() == reflection_y3, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_fm.get_h() == reflection_h3, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point3_spm.get_x() == reflection_x3, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_spm.get_y() == reflection_y3, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_spm.get_h() == reflection_h3, tt::tolerance(1e-4));

    BOOST_TEST(reflection_point3_efrm.get_x() == reflection_x3, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_efrm.get_y() == reflection_y3, tt::tolerance(1e-4));
    BOOST_TEST(reflection_point3_efrm.get_h() == reflection_h3, tt::tolerance(1e-4));
}

BOOST_AUTO_TEST_CASE(test_plain_FindIncidenceAngle) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::Plain plain;
    CD::Constant c(0);

    Map map(&plain, &non_veg, &const_dp, &c);

    PointCartesian rls(0, 0, 2);
    PointCartesian target(4, 0, 2);
    PointSpheric sp1(rls, target);

    PointCartesian reflection_point_fm = RP::FindReflectionPoint(sp1, &map, &fm);
    PointCartesian reflection_point_spm = RP::FindReflectionPoint(sp1, &map, &spm);
    PointCartesian reflection_point_efrm = RP::FindReflectionPoint(sp1, &map, &efrm);

    PointSpheric sp2_fm(rls, reflection_point_fm);
    double incidence_angle_fm = ReflectionPoint::FindIncidenceAngle(sp2_fm, &map, &fm);
    double incidence_angle_spm = ReflectionPoint::FindIncidenceAngle(sp2_fm, &map, &spm);
    double incidence_angle_efrm = ReflectionPoint::FindIncidenceAngle(sp2_fm, &map, &efrm);

    double theory_angle = M_PI / 4;
    BOOST_TEST(incidence_angle_fm == theory_angle, tt::tolerance(1e-4));
    BOOST_TEST(incidence_angle_spm == theory_angle, tt::tolerance(1e-4));
    BOOST_TEST(incidence_angle_efrm == theory_angle, tt::tolerance(1e-4));
}

BOOST_AUTO_TEST_CASE(test_GeoData_FindIncidenceAngle) {
    DP::Constant const_dp(4);
    VG::None non_veg;
    EL::GeoData geo_data;
    CD::Constant c(0);

    Map map(&geo_data, &non_veg, &const_dp, &c);

    PointCartesian rls(0, 0, 0);
    PointCartesian target(20, 0, 40);
    PointSpheric sp1(rls, target);

    PointCartesian reflection_point_fm = RP::FindReflectionPoint(sp1, &map, &fm);
    PointCartesian reflection_point_spm = RP::FindReflectionPoint(sp1, &map, &spm);
    PointCartesian reflection_point_efrm = RP::FindReflectionPoint(sp1, &map, &efrm);

    PointSpheric sp2_fm(rls, reflection_point_fm);
    PointSpheric sp3_fm(target, reflection_point_fm);

    PointSpheric sp2_spm(rls, reflection_point_fm);
    PointSpheric sp3_spm(target, reflection_point_fm);

    PointSpheric sp2_efrm(rls, reflection_point_fm);
    PointSpheric sp3_efrm(target, reflection_point_fm);

    double incidence_angle1_fm = ReflectionPoint::FindIncidenceAngle(sp2_fm, &map, &fm);
    double incidence_angle2_fm = ReflectionPoint::FindIncidenceAngle(sp3_fm, &map, &fm);

    double incidence_angle1_spm = ReflectionPoint::FindIncidenceAngle(sp2_fm, &map, &fm);
    double incidence_angle2_spm = ReflectionPoint::FindIncidenceAngle(sp3_fm, &map, &fm);

    double incidence_angle1_efrm = ReflectionPoint::FindIncidenceAngle(sp2_fm, &map, &fm);
    double incidence_angle2_efrm = ReflectionPoint::FindIncidenceAngle(sp3_fm, &map, &fm);

    BOOST_TEST(incidence_angle1_fm == incidence_angle2_fm, tt::tolerance(1e-4));
    BOOST_TEST(incidence_angle1_spm == incidence_angle2_spm, tt::tolerance(1e-4));
    BOOST_TEST(incidence_angle1_efrm == incidence_angle2_efrm, tt::tolerance(1e-4));
}

BOOST_AUTO_TEST_SUITE_END()
