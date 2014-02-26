#include "unit_test_support.h"
#include "argus_orbit.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(argus_orbit, GlobalFixture)
BOOST_AUTO_TEST_CASE(argus_orbit)
{
  std::string fname = test_data_dir() + "argus.csv";
  ArgusOrbit f(fname);
  BOOST_CHECK_EQUAL(f.number_row(), 1138);
  BOOST_CHECK_EQUAL(f.nav(100, 1)->file_name(), "00010062.JPG");
}

BOOST_AUTO_TEST_CASE(argus_orbit_focal_length)
{
  // This depends on real test data, so we don't normally run this.
  if(false) {
    ArgusOrbit orb("/raid1/smyth/mali/20090531_084101/20090531_084101.csv");
    for(int i = 1; i <= 13; ++i)
      std::cerr << i << ": " << orb.focal_length(i) << "\n";
  }
}
BOOST_AUTO_TEST_SUITE_END()
