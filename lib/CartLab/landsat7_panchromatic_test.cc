#include "unit_test_support.h"
#include "landsat7_panchromatic.h"
#include <boost/filesystem.hpp>
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(landsat7_panchromatic, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!boost::filesystem::is_directory("/data/bank/anc/LANDSAT/LANDSAT7_PANCHROMATIC_15M/database"))
    return;
  Landsat7Panchromatic f("/data/bank/anc/LANDSAT/LANDSAT7_PANCHROMATIC_15M/database");
  // We just found this value empirical, and check that we get the
  // same results back.
  BOOST_CHECK_CLOSE(f.interpolate(f.coordinate(Geodetic(42, -77))), 70.0,
		    1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  if(!boost::filesystem::is_directory("/data/bank/anc/LANDSAT/LANDSAT7_PANCHROMATIC_15M/database"))
    return;
  boost::shared_ptr<RasterImage> img;
  img.reset(new Landsat7Panchromatic("/data/bank/anc/LANDSAT/LANDSAT7_PANCHROMATIC_15M/database"));
  std::string d = serialize_write_string(img);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RasterImage> imgr = 
    serialize_read_string<RasterImage>(d);
  BOOST_CHECK_CLOSE(imgr->interpolate(imgr->coordinate(Geodetic(42, -77))), 
		    70.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
