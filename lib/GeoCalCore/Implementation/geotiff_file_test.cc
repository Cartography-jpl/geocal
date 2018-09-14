#include "unit_test_support.h"
#include "geotiff_file.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geotiff_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  const std::string fname = test_data_dir() + "usgs_dem/n44w069_10m.tif";
  GeotiffFile f(fname, "r");
  BOOST_CHECK_EQUAL(f.file_name(), fname);
  BOOST_CHECK_EQUAL(f.mode(), "r");
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  const std::string fname = test_data_dir() + "usgs_dem/n44w069_10m.tif";
  boost::shared_ptr<GeotiffFile> f(new GeotiffFile(fname, "r"));
  std::string d = serialize_write_string(f);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<GeotiffFile> fr = 
    serialize_read_string<GeotiffFile>(d);
  BOOST_CHECK_EQUAL(fr->file_name(), fname);
  BOOST_CHECK_EQUAL(fr->mode(), "r");
}

BOOST_AUTO_TEST_SUITE_END()


