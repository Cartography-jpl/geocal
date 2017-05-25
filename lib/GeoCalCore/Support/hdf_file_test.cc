#include "unit_test_support.h"
#include "hdf_file.h"

using namespace GeoCal;
using namespace blitz;
BOOST_FIXTURE_TEST_SUITE(hdf_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  std::string fname = test_data_dir() + "sample_orbit.h5";
  HdfFile f(fname);
  std::string desc = 
    f.read_attribute<std::string>("/Orbit/Ephemeris/Position/Description");
  BOOST_CHECK_EQUAL(desc, "This is ECI True of Date Position, in meters");
  Array<double, 2> pos = f.read_field<double, 2>("/Orbit/Ephemeris/Position");
  BOOST_CHECK_EQUAL(pos.shape()[0], 100);
  BOOST_CHECK_EQUAL(pos.shape()[1], 3);
  BOOST_CHECK_CLOSE(pos(0,0), 3435100.496, 1e-6);
  BOOST_CHECK_CLOSE(pos(0,1), 945571.538, 1e-6);
  BOOST_CHECK_CLOSE(pos(0,2), -6053387.573, 1e-6);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  std::string fname = test_data_dir() + "sample_orbit.h5";
  boost::shared_ptr<HdfFile> f(new HdfFile(fname));
  std::string d = serialize_write_string(f);
  if(false)
    std::cerr << d;
  boost::shared_ptr<HdfFile> fr = 
    serialize_read_string<HdfFile>(d);
  std::string desc = 
    fr->read_attribute<std::string>("/Orbit/Ephemeris/Position/Description");
  BOOST_CHECK_EQUAL(desc, "This is ECI True of Date Position, in meters");
  Array<double, 2> pos = fr->read_field<double, 2>("/Orbit/Ephemeris/Position");
  BOOST_CHECK_EQUAL(pos.shape()[0], 100);
  BOOST_CHECK_EQUAL(pos.shape()[1], 3);
  BOOST_CHECK_CLOSE(pos(0,0), 3435100.496, 1e-6);
  BOOST_CHECK_CLOSE(pos(0,1), 945571.538, 1e-6);
  BOOST_CHECK_CLOSE(pos(0,2), -6053387.573, 1e-6);
}  

BOOST_AUTO_TEST_SUITE_END()
