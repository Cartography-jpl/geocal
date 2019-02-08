#include "unit_test_support.h"
#include "misc_array_function.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(misc_array_function, GlobalFixture)

BOOST_AUTO_TEST_CASE(array_local_median_test)
{
  blitz::Array<double, 2> data(100,100);
  double s = 10.0 / (data.rows() - 1);
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j)
      data(i,j) = i * s + j * s;
  data(10,10) = 100.0;
  blitz::Array<double, 2> res = array_local_median(data, 1, 7);
  BOOST_CHECK_CLOSE(res(10,10), 2.12121, 1e-2);
  BOOST_CHECK_CLOSE(res(0,0), (data(0,1) + data(0,2)) / 2.0, 1e-2);
}

BOOST_AUTO_TEST_SUITE_END()
