#include "unit_test_support.h"
#include "context_sqrt_decode_image.h"
#include "memory_raster_image.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(context_sqrt_decode_image, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<RasterImage> rawimg =
    boost::make_shared<MemoryRasterImage>(10, 20);
  int val = 0;
  for(int i = 0; i < rawimg->number_line(); ++i)
    for(int j = 0; j < rawimg->number_sample(); ++j)
      rawimg->write(i, j, val++);
  ContextSqrtDecodeImage img(rawimg);
  BOOST_CHECK_EQUAL(img.number_line(), rawimg->number_line());
  BOOST_CHECK_EQUAL(img.number_sample(), rawimg->number_sample());
  BOOST_CHECK_EQUAL(rawimg->read(5,6), 106);
  BOOST_CHECK_EQUAL(img.read(5,6), 778);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<RasterImage> rawimg =
    boost::make_shared<MemoryRasterImage>(10, 20);
  int val = 0;
  for(int i = 0; i < rawimg->number_line(); ++i)
    for(int j = 0; j < rawimg->number_sample(); ++j)
      rawimg->write(i, j, val++);
  boost::shared_ptr<ContextSqrtDecodeImage> img =
    boost::make_shared<ContextSqrtDecodeImage>(rawimg);
  std::string d = serialize_write_string(img);
  if(false)
    std::cerr << d;
  boost::shared_ptr<ContextSqrtDecodeImage> imgr = 
    serialize_read_string<ContextSqrtDecodeImage>(d);
  BOOST_CHECK_EQUAL(imgr->number_line(), rawimg->number_line());
  BOOST_CHECK_EQUAL(imgr->number_sample(), rawimg->number_sample());
  BOOST_CHECK_EQUAL(rawimg->read(5,6), 106);
  BOOST_CHECK_EQUAL(imgr->read(5,6), 778);
}

BOOST_AUTO_TEST_SUITE_END()

