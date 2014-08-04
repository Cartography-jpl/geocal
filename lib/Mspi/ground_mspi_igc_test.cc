#include "ground_mspi_igc.h"
#include "unit_test_support.h"
#include <iostream>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(ground_mspi_igc, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Time tstart = Time::parse_time("1996-07-03T04:13:47.987654Z") + 10;
  boost::shared_ptr<GroundCoordinate> p(new Geodetic(50, 60, 1));
  std::vector<Time> time_tag;
  time_tag.push_back(tstart + 0 * 5.1);
  time_tag.push_back(tstart + 1 * 5.1);
  time_tag.push_back(tstart + 2 * 5.1);
  GroundMspiIgc igc(tstart, p, 45.0, -10.0, 1.7, time_tag,
		    test_data_dir() + "ground_mspi_camera_test.config", 2);
  CartesianFixedLookVector lv;
  boost::shared_ptr<CartesianFixed> p2;
  igc.cf_look_vector(ImageCoordinate(0, 0), lv, p2);
  LnLookVector ln(lv, *p2);
  BOOST_CHECK_CLOSE(ln.view_zenith(), 1.75848329e+00 * Constant::rad_to_deg, 
		    1e-4);
  BOOST_CHECK_CLOSE(ln.view_azimuth(), 3.66389728e+00 * Constant::rad_to_deg,
		    1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
