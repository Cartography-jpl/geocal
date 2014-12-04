#ifndef UNIT_TEST_SUPPORT_H
#define UNIT_TEST_SUPPORT_H
#include "global_fixture.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

// Stuff needed for testing serialization.
// Have this in place in case we want to make this conditional
#define HAVE_BOOST_SERIALIZATON 
#ifdef HAVE_BOOST_SERIALIZATON
#define USE_BOOST_SERIALIZATON

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>

#endif

//-----------------------------------------------------------------------
/// Check for two matrixes being equal
//-----------------------------------------------------------------------

#define BOOST_CHECK_MATRIX_CLOSE_TOL( A, B, tol ) BOOST_CHECK_LT(max(abs(A - (B))), tol)
#define BOOST_CHECK_MATRIX_CLOSE( A, B ) BOOST_CHECK_MATRIX_CLOSE_TOL(A, B, 1e-8)

#endif
