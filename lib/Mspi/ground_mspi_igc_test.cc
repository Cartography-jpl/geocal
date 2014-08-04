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
}

BOOST_AUTO_TEST_SUITE_END()
