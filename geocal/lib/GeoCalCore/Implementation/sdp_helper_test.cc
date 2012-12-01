#include "unit_test_support.h"
#include <cstdlib>
#include "sdp_helper.h"
#include "geocal_matrix.h"    
#include "eci.h"
#include "ecr.h"
#include "geodetic.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(sdp_helper, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!SdpHelper::sdp_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SDP Toolkit, so skipping SDP tests.");
    return;
  }
  ToolkitCoordinateInterface* keep = 
    CartesianFixed::toolkit_coordinate_interface;
  SdpToolkitCoordinateInterface tk;
  CartesianFixed::toolkit_coordinate_interface = &tk;
  ToolkitTimeInterface* keep2 = 
    Time::toolkit_time_interface;
  SdpToolkitTimeInterface tk2;
  Time::toolkit_time_interface = &tk2;
  Time r = SdpHelper::parse_time("1996-07-03T04:13:57.987654Z");
  BOOST_CHECK_CLOSE(r.pgs(), 110520840.987654, 1e-6);
  r += 2.0 * 365 * 24 * 60 * 60; // This goes through a leapsecond
  BOOST_CHECK_EQUAL(SdpHelper::to_string(r), 
		    std::string("1998-07-03T04:13:56.987654Z"));
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  boost::shared_ptr<CartesianFixed> e_ecr_start = 
    Geodetic(10, 20, 30).convert_to_cf();
  boost::shared_ptr<CartesianInertial> e_eci = e_ecr_start->convert_to_ci(t);
  BOOST_CHECK_CLOSE(e_eci->position[0], 888551.27707691176, 1e-4);
  BOOST_CHECK_CLOSE(e_eci->position[1], -6218769.7591331499, 1e-4);
  BOOST_CHECK_CLOSE(e_eci->position[2], 1100106.8473076127, 1e-4);
  boost::shared_ptr<CartesianFixed> e_ecr = e_eci->convert_to_cf(t);
  BOOST_CHECK_CLOSE(e_ecr->position[0], e_ecr_start->position[0], 1e-4);
  BOOST_CHECK_CLOSE(e_ecr->position[1], e_ecr_start->position[1], 1e-4);
  BOOST_CHECK_CLOSE(e_ecr->position[2], e_ecr_start->position[2], 1e-4);
  double ci_to_cf[3][3];
  e_eci->ci_to_cf(t, ci_to_cf);
  boost::array<double, 3> eci_arr;
  mul_t(ci_to_cf, e_ecr->position, eci_arr);
  BOOST_CHECK_CLOSE(eci_arr[0], e_eci->position[0], 1e-4);
  BOOST_CHECK_CLOSE(eci_arr[1], e_eci->position[1], 1e-4);
  BOOST_CHECK_CLOSE(eci_arr[2], e_eci->position[2], 1e-4);
  boost::array<double, 3> ecr_arr;
  mul(ci_to_cf, e_eci->position, ecr_arr);
  BOOST_CHECK_CLOSE(ecr_arr[0], e_ecr->position[0], 1e-4);
  BOOST_CHECK_CLOSE(ecr_arr[1], e_ecr->position[1], 1e-4);
  BOOST_CHECK_CLOSE(ecr_arr[2], e_ecr->position[2], 1e-4);
  CartesianFixed::toolkit_coordinate_interface = keep;
  Time::toolkit_time_interface = keep2;
}

BOOST_AUTO_TEST_SUITE_END()
