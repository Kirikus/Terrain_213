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
BOOST_AUTO_TEST_SUITE_END()
