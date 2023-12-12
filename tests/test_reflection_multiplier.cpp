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

VG::VegetationType grass = VG::VegetationType::Grass;
VG::VegetationType shrub = VG::VegetationType::Shrub;
VG::VegetationType forest = VG::VegetationType::Forest;


BOOST_AUTO_TEST_CASE(test_ElevationReflectionMultiplier) {
    // Initialization
    RC::ElevationReflectionMultiplier erm;
    double incidence_angle = M_PI / 4;
    double wave_l = 5 * std::pow(10, -5);
    double sko = wave_l / (8 * std::sin(incidence_angle));

    // We are checking the Rayleigh criterion:
    double frenel_coeff = erm.frenel_coefficient(incidence_angle, wave_l, sko / 2);
    double theory_coeff = std::exp(-std::pow(M_PI, 2) / 32);

    BOOST_TEST(frenel_coeff == theory_coeff, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_VegetationReflectionMultiplier) {
    RC::VegetationReflectionMultiplier vrm;
    double incidence_angle = M_PI / 5;

    // X - range:
    double wave_l1 = 0.03;

    // S - range:
    double wave_l2 = 0.1;

    // VHF - range:
    double wave_l3 = 1;

    // For grass:
    double a1 = 3.2;
    double b1 = 1;

    // For the shrub
    double a2 = 0.32;
    double b2 = 3;

    // For the forest:
    double a3 = 0.032;
    double b3 = 5;

    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, grass, wave_l1) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, grass, wave_l2) <= 1, tt::tolerance(1e-6));

    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, shrub, wave_l1) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, shrub, wave_l2) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, shrub, wave_l3) <= 1, tt::tolerance(1e-6));

    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, shrub, wave_l1) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, shrub, wave_l2) <= 1, tt::tolerance(1e-6));
    BOOST_TEST(vrm.vegetation_coeff(incidence_angle, shrub, wave_l3) <= 1, tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(test_FrenelReflectionMultiplier) {
    // Map initialization
    EL::Plain plain;
    VG::None veg;

    DP::Constant dp1(65); // For fresh water
    CD::Constant c1(15); // For fresh water
    Map map(&plain, &veg, &dp1, &c1);

    // Frenel coefficient initialization
    RC::FrenelReflectionMultiplier frm;

    // Reflection dot receiving
    PointCartesian rls(0, 0, 2);
    PointCartesian target(25, 0, 2);
    PointSpheric sp(rls, target);
    PointCartesian rp = RP::FindReflectionPoint(sp, &map, &fm);
    PointSpheric rp_t(rp, target);
    double incidence_angle = rp_t.get_phi();

    // Setting the signal parameters
    double wave_l = 1; // VHF
    double cond1 = 15; // For fresh water
    double cond2 = 0.005; // For poor soil

    // Horicontal polarization:
    double theory1 = 1; // Maximum value of frenel reflection multiplier
    BOOST_TEST(std::abs(frm.horizontal_polarization(&map, rp_t, incidence_angle, wave_l, cond1)) <= theory1, tt::tolerance(1e-6));
    BOOST_TEST(std::abs(frm.horizontal_polarization(&map, rp_t, incidence_angle, wave_l, cond2)) <= theory1, tt::tolerance(1e-6));

    // Vertical polarization
    BOOST_TEST(std::abs(frm.vertical_polarization(&map, rp_t, incidence_angle, wave_l, cond1)) <= theory1, tt::tolerance(1e-6));
    BOOST_TEST(std::abs(frm.vertical_polarization(&map, rp_t, incidence_angle, wave_l, cond2)) <= theory1, tt::tolerance(1e-6));

    // Circular polarization
    BOOST_TEST(std::abs(frm.circular_polarization(&map, rp_t, incidence_angle, wave_l, cond1)) <= theory1, tt::tolerance(1e-6));
    BOOST_TEST(std::abs(frm.circular_polarization(&map, rp_t, incidence_angle, wave_l, cond1)) <= theory1, tt::tolerance(1e-6));

    // Cross polarization
    BOOST_TEST(std::abs(frm.cross_polarization(&map, rp_t, incidence_angle, wave_l, cond1)) <= theory1, tt::tolerance(1e-6));
    BOOST_TEST(std::abs(frm.cross_polarization(&map, rp_t, incidence_angle, wave_l, cond1)) <= theory1, tt::tolerance(1e-6));
} // TODO

BOOST_AUTO_TEST_CASE(test_ReflectionMultiplier) {
    // Map initialization
    EL::Plain plain;
    VG::None veg;
    DP::Constant dp(2);
    CD::Constant c(0);

    Map map(&plain, &veg, &dp, &c);

    PointCartesian rls(0, 0, 2);
    PointCartesian target(4, 0, 2);
    PointSpheric sp(rls, target);

    RC::ElevationReflectionMultiplier erm;
    RC::VegetationReflectionMultiplier vrm;
    RC::FrenelReflectionMultiplier frm;
    RC::ReflectionMultiplier rm(&erm, &frm, &vrm);

    ReflectionCoefficients::Polarization horizontal = ReflectionCoefficients::HorizontalPolarization;
    ReflectionCoefficients::Polarization vertical = ReflectionCoefficients::VerticalPolarization;
    ReflectionCoefficients::Polarization circular = ReflectionCoefficients::CircularPolarization;
    ReflectionCoefficients::Polarization cross = ReflectionCoefficients::CrossPolarization;

    double a = 0.32;
    double b = 3;
    double incidence_angle = M_PI / 4;
    double wave_l = 1;
    double cond1 = 15;
    double sko = wave_l / (8 * std::sin(incidence_angle));
    double frenel_coeff = erm.frenel_coefficient(incidence_angle, wave_l, sko / 2); // The Rayleigh criterion is used:
    double vegetation_coeff = vrm.vegetation_coeff(incidence_angle, shrub, wave_l); // Shrub vegetation is used
    std::complex<double> polarization_coeff = frm.horizontal_polarization(&map, sp, incidence_angle, wave_l, cond1); // Horizontal polarization is used

    BOOST_TEST(std::abs(rm.reflection_multiplier(&map, sp, horizontal, incidence_angle, wave_l, cond1, sko / 2, shrub)) == std::abs(frenel_coeff * vegetation_coeff * polarization_coeff), tt::tolerance(1e-6));
}

BOOST_AUTO_TEST_SUITE_END()
