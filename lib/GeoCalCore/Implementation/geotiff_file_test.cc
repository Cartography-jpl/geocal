#include "unit_test_support.h"
#include "geotiff_file.h"
#include <iostream>
#include <boost/make_shared.hpp>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geotiff_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  std::string fname = test_data_dir() + "usgs_dem/n44w069_10m.tif";
  GeotiffFile f(fname, "r");
  BOOST_CHECK_EQUAL(f.file_name(), fname);
  BOOST_CHECK_EQUAL(f.mode(), "r");
  blitz::Array<double, 1> t = f.get_tiftag<blitz::Array<double, 1> >(GeotiffFile::TIFFTAG_GEOPIXELSCALE);
  blitz::Array<double, 1> texpect(3);
  texpect = 9.25926e-05, 9.25926e-05, 0.0;
  BOOST_CHECK_MATRIX_CLOSE_TOL(t, texpect, 1e-4);
}

BOOST_AUTO_TEST_CASE(write_file)
{
  std::string fname = "geotiff_write_file_test.tif";
  boost::shared_ptr<GeotiffFile> f =
    boost::make_shared<GeotiffFile>(fname, "w");
  BOOST_CHECK_EQUAL(f->file_name(), fname);
  BOOST_CHECK_EQUAL(f->mode(), "w");
  f->write_1x1_file();
  f.reset();
  f = boost::make_shared<GeotiffFile>(fname, "r");
  BOOST_CHECK_EQUAL(f->file_name(), fname);
  BOOST_CHECK_EQUAL(f->mode(), "r");
  BOOST_CHECK(f->has_tiftag(GeotiffFile::TIFFTAG_IMAGEWIDTH));
  BOOST_CHECK_EQUAL(f->get_tiftag<int>(GeotiffFile::TIFFTAG_IMAGEWIDTH), 1);
  BOOST_CHECK(f->has_tiftag(GeotiffFile::TIFFTAG_IMAGELENGTH));
  BOOST_CHECK_EQUAL(f->get_tiftag<int>(GeotiffFile::TIFFTAG_IMAGELENGTH), 1);
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


