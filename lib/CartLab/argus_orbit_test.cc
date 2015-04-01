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

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  std::string fname = test_data_dir() + "argus.csv";
  boost::shared_ptr<ArgusOrbit> orb(new ArgusOrbit(fname));
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<ArgusOrbit> orbr =
    serialize_read_string<ArgusOrbit>(d);
  BOOST_CHECK_EQUAL(orbr->number_row(), 1138);
  BOOST_CHECK_EQUAL(orbr->nav(100, 1)->file_name(), "00010062.JPG");
}

BOOST_AUTO_TEST_CASE(serialization_od)
{
  if(!have_serialize_supported())
    return;
  std::string fname = test_data_dir() + "argus.csv";
  ArgusOrbit orb(fname);
  boost::shared_ptr<ArgusOrbitData> od  = orb.nav(100,1);
  std::string d = serialize_write_string(od);
  if(false)
    std::cerr << d;
  boost::shared_ptr<ArgusOrbitData> odr =
    serialize_read_string<ArgusOrbitData>(d);
  BOOST_CHECK_EQUAL(odr->file_name(), "00010062.JPG");
}

BOOST_AUTO_TEST_SUITE_END()
