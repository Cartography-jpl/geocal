#include "unit_test_support.h"
#include "smooth_image.h"
#include "memory_raster_image.h"
#include "sub_raster_image.h"

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(smooth_image, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<RasterImage> data(new MemoryRasterImage(11, 10));
  int val = 0;
  for(int i = 0; i < data->number_line(); ++i)
    for(int j = 0; j < data->number_sample(); ++j, ++val)
      data->write(i, j, val);
  boost::shared_ptr<RasterImage> ra(new SmoothImage(data));
  BOOST_CHECK_EQUAL(ra->number_line(), 11 - 6);
  BOOST_CHECK_EQUAL(ra->number_sample(), 10 - 6);
  for(int i = 0; i < ra->number_line(); ++i)
    for(int j = 0; j < ra->number_sample(); ++j) {
      double res = 0;
      for(int i1 = 0; i1 < 7; ++i1)
	for(int i2 = 0; i2 < 7; ++i2)
	  res += (*data)(i1 + i, i2 + j);
      res /= 49;
      BOOST_CHECK_EQUAL((*ra)(i, j), (int) res);
    }
  Array<double, 2> rav = ra->read_double(0,0, ra->number_line(), 
					 ra->number_sample());
  for(int i = 0; i < ra->number_line(); ++i)
    for(int j = 0; j < ra->number_sample(); ++j) {
      double res = 0;
      for(int i1 = 0; i1 < 7; ++i1)
	for(int i2 = 0; i2 < 7; ++i2)
	  res += (*data)(i1 + i, i2 + j);
      res /= 49;
      BOOST_CHECK_CLOSE(rav(i, j), res, 1e-8);
    }
  for(int i = 0; i < ra->number_line(); ++i)
    for(int j = 0; j < ra->number_sample(); ++j) {
      double res = 0;
      for(int i1 = 0; i1 < 7; ++i1)
	for(int i2 = 0; i2 < 7; ++i2)
	  res += (*data)(i1 + i, i2 + j);
      res /= 49;
      Array<double, 2> t = ra->read_double(i, j, 1, 1);
      BOOST_CHECK_CLOSE(t(0, 0), res, 1e-8);
    }
}		    

BOOST_AUTO_TEST_SUITE_END()
