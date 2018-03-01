#include "unit_test_support.h"
#include "landsat7_global.h"
#include "vicar_file.h"
#include <boost/filesystem.hpp>
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(landsat7_global, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!VicarFile::vicar_available())
    return;
  if(!boost::filesystem::is_directory("/raid22/band62_VICAR"))
    return;
  Landsat7Global f("/raid22/band62_VICAR", Landsat7Global::BAND62);
  // We just found this value empirical, and check that we get the
  // same results back.
  BOOST_CHECK_CLOSE(f.interpolate(f.coordinate(Geodetic(42, -77))), 136.0,
		    1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!VicarFile::vicar_available())
    return;
  if(!have_serialize_supported())
    return;
  if(!boost::filesystem::is_directory("/raid22/band62_VICAR"))
    return;
  boost::shared_ptr<RasterImage> img;
  img.reset(new Landsat7Global("/raid22/band62_VICAR", Landsat7Global::BAND62));
  std::string d = serialize_write_string(img);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RasterImage> imgr = 
    serialize_read_string<RasterImage>(d);
  BOOST_CHECK_CLOSE(imgr->interpolate(imgr->coordinate(Geodetic(42, -77))), 
		    136, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
