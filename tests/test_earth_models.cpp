#include "test_earth_models.h"

#include <QApplication>

#include "../lib/earth_models.h"

#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(test_earth_models)



BOOST_AUTO_TEST_SUITE_END()
