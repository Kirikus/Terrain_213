#include "test_reflection_multiplier.h"
#include <QApplication>
#include "../lib/reflection_multiplier.h"
#include "../lib/reflection_point.h"

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;
namespace RC = ReflectionCoefficients;
namespace RP = ReflectionPoint;
namespace EM = EarthModels;

BOOST_AUTO_TEST_SUITE(test_reflection_multipliers)

EM::ModelFlat fm;
EM::ModelSpheric spm;
EM::ModelEffectiveRadius efrm;

BOOST_AUTO_TEST_CASE(test_ElevationReflectionMultiplier) {
    RC::ElevationReflectionMultiplier erm;
    double incidence_angle = M_PI / 4;
    double wave_l = 5 * std::pow(10, -5);
    double sko = wave_l / (8 * std::sin(incidence_angle));

    //We check the Rayleigh criterion:
    double frenel_coeff = erm.frenel_coefficient(incidence_angle, wave_l, sko / 2);
    double theory_coeff = std::exp(-std::pow(M_PI, 2) / 32);
    BOOST_TEST(frenel_coeff == theory_coeff, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_VegetationReflectionMultiplier) {
    RC::VegetationReflectionMultiplier vrm;
    double incidence_angle = M_PI / 5;

    //X - range:
    double wave_l1 = 0.03;

    //S - range:
    double wave_l2 = 0.1;

    //VHF - range:
    double wave_l3 = 1;

    //for grass:
    double a1 = 3.2;
    double b1 = 1;

    //for the shrub
    double a2 = 0.32;
    double b2 = 3;

    //for the forest:
    double a3 = 0.032;
    double b3 = 5;

    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, a1, b1, wave_l1) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, a1, b1, wave_l2) <= 1, tt::tolerance(1e-6));

    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, a2, b2, wave_l1) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, a2, b2, wave_l2) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, a2, b2, wave_l3) <= 1, tt::tolerance(1e-6));

    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, a3, b3, wave_l1) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, a3, b3, wave_l2) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, a3, b3, wave_l3) <= 1, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_FrenelReflectionMultiplier) {
    RC::FrenelReflectionMultiplier frm;

    EL::Plain plain;
    VG::None veg;
    DP::Constant dp(2);
    Map map(&plain, &veg, &dp);

    PointCartesian rls(0, 0, 2);
    PointCartesian target(4, 0, 2);
    PointSpheric sp(rls, target);

    PointCartesian rp = RP::FindReflectionPoint(sp, &map, &fm);
    PointSpheric rp_t(rp, target);
    double incidence_angle = rp_t.get_phi();

    //horicontal polarization:
    double theory1 = -0.267949;
    BOOST_TEST(frm.horizontal_polarization(&map, sp, incidence_angle).real() == theory1, tt::tolerance(1e-6));
    BOOST_TEST(frm.horizontal_polarization(&map, sp, incidence_angle).imag() == 0, tt::tolerance(1e-6));

    //vertical polarization
    double theory2 = 0.0717967;
    BOOST_TEST(frm.vertical_polarization(&map, sp, incidence_angle).real() == theory2, tt::tolerance(1e-6));
    BOOST_TEST(frm.vertical_polarization(&map, sp, incidence_angle).imag() == 0, tt::tolerance(1e-6));
} //for imag todo

BOOST_AUTO_TEST_CASE(test_ReflectionMultiplier) {
    EL::Plain plain;
    VG::None veg;
    DP::Constant dp(2);
    Map map(&plain, &veg, &dp);

    PointCartesian rls(0, 0, 2);
    PointCartesian target(4, 0, 2);
    PointSpheric sp(rls, target);

    RC::ElevationReflectionMultiplier erm;
    RC::VegetationReflectionMultiplier vrm;
    RC::FrenelReflectionMultiplier frm;
    RC::ReflectionMultiplier rm(&erm, &frm, &vrm);

    double a = 0.32;
    double b = 3;
    double incidence_angle = M_PI / 4;
    double wave_l = 5 * std::pow(10, -5);
    double sko = wave_l / (8 * std::sin(incidence_angle));
    double frenel_coeff = erm.frenel_coefficient(incidence_angle, wave_l, sko / 2);// The Rayleigh criterion is used:
    double vegetation_coeff = vrm.vegetation_coeff(incidence_angle, a, b, wave_l);
    std::complex<double> polarization_coeff = frm.horizontal_polarization(&map, sp, incidence_angle);

    BOOST_TEST(rm.reflection_multiplier(&map, sp, incidence_angle, wave_l, sko / 2, a, b) == frenel_coeff * vegetation_coeff * polarization_coeff, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_SUITE_END()
