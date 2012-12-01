#ifndef UNIT_TEST_SUPPORT_H
#define UNIT_TEST_SUPPORT_H
#include "global_fixture.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

//-----------------------------------------------------------------------
/// Check for two matrixes being equal
//-----------------------------------------------------------------------

#define BOOST_CHECK_MATRIX_CLOSE_TOL( A, B, tol ) BOOST_CHECK_LT(max(abs(A - (B))), tol)
#define BOOST_CHECK_MATRIX_CLOSE( A, B ) BOOST_CHECK_MATRIX_CLOSE_TOL(A, B, 1e-8)

#endif
