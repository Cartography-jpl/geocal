#include "unit_test_support.h"
#include "magnify_bilinear.h"
#include "memory_raster_image.h"
#include "sub_raster_image.h"

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(magnify_bilinear, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<RasterImage> data(new MemoryRasterImage(11, 10));
  int val = 0;
  for(int i = 0; i < data->number_line(); ++i)
    for(int j = 0; j < data->number_sample(); ++j, ++val)
      data->write(i, j, val);
  boost::shared_ptr<RasterImage> ra(new MagnifyBilinear(data, 2.0));
  BOOST_CHECK_EQUAL(ra->number_line(), 20);
  BOOST_CHECK_EQUAL(ra->number_sample(), 18);
  for(int i = 0; i < ra->number_line(); ++i)
    for(int j = 0; j < ra->number_sample(); ++j)
      BOOST_CHECK_EQUAL((*ra)(i, j), (int) data->interpolate(i / 2.0, j /2.0));
  Array<double, 2> rav = ra->read_double(0,0, ra->number_line(), 
					 ra->number_sample());
  for(int i = 0; i < ra->number_line(); ++i)
    for(int j = 0; j < ra->number_sample(); ++j)
      BOOST_CHECK_CLOSE(rav(i, j), data->interpolate(i / 2.0, j /2.0), 1e-8);
  for(int i = 0; i < ra->number_line(); ++i)
    for(int j = 0; j < ra->number_sample(); ++j) {
      Array<double, 2> t = ra->read_double(i, j, 1, 1);
      BOOST_CHECK_CLOSE(t(0, 0), data->interpolate(i / 2.0, j /2.0), 1e-8);
    }

  SubRasterImage rasub(ra, 5, 6, 10, 8);
  BOOST_CHECK_EQUAL(rasub.number_line(), 10);
  BOOST_CHECK_EQUAL(rasub.number_sample(), 8);
  for(int i = 0; i < rasub.number_line(); ++i)
    for(int j = 0; j < rasub.number_sample(); ++j)
      BOOST_CHECK_EQUAL(rasub(i, j), (int) data->interpolate((i + 5) / 2.0, 
							     (j + 6) /2.0));
  Array<double, 2> rasubv = rasub.read_double(0,0, rasub.number_line(), 
					      rasub.number_sample());
  for(int i = 0; i < rasub.number_line(); ++i)
    for(int j = 0; j < rasub.number_sample(); ++j)
      BOOST_CHECK_CLOSE(rasubv(i, j), data->interpolate((i + 5) / 2.0, 
							(j + 6) /2.0), 1e-8);
  for(int i = 0; i < rasub.number_line(); ++i)
    for(int j = 0; j < rasub.number_sample(); ++j) {
      Array<double, 2> t = rasub.read_double(i, j, 1, 1);
      BOOST_CHECK_CLOSE(t(0, 0), data->interpolate((i + 5) / 2.0, 
						   (j + 6) /2.0), 1e-8);
    }
  
}

BOOST_AUTO_TEST_SUITE_END()
