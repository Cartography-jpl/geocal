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

BOOST_AUTO_TEST_CASE(linear_gradient_bad_pixel_detection_test)
{
  int sz = 512;
  int number_task = 1;
  //int sz = 8192;
  //int number_task = 5;
  blitz::Array<double, 2> img(sz,sz);
  blitz::Array<double, 1> lnspace(sz);
  for(int i = 0; i < img.cols(); ++i)
    lnspace(i) = 10.0/sz * i;
  for(int i = 0; i < img.rows(); ++i)
    img(i,blitz::Range::all()) = lnspace + lnspace(i);

  // Add a couple of bad pixels.
  blitz::Array<bool, 2> bad_expect(img.shape());
  bad_expect = false;
  img(100,200) = 40.0;
  img(300,400) = -40.0;
  img(0,100) = -80.0;
  img(sz-1,sz-1) = 80.0;
  bad_expect(100,200) = true;
  bad_expect(300,400) = true;
  bad_expect(0,100) = true;
  bad_expect(sz-1,sz-1) = true;
  blitz::Array<bool, 2> bad_pixel;
  if(false) {
    // For better timing
    for(int i = 0; i < 9; ++i)
      bad_pixel.reference(linear_gradient_bad_pixel_detection(img, 7, 90.0, 2,
			      75, ARRAY_LOCAL_MEDIAN_TRUNCATE, number_task));
  } else {
    bad_pixel.reference(linear_gradient_bad_pixel_detection(img, 7, 90.0, 2,
			    75, ARRAY_LOCAL_MEDIAN_TRUNCATE, number_task));
  }
  BOOST_CHECK(blitz::all(bad_pixel == bad_expect));
}

// We've moved away from openmp, instead use C++ threads.
//#include <omp.h>

// Sample omp test. Leave in place as an example now, but we don't
// really want to run this regularly.
// BOOST_AUTO_TEST_CASE(timing_test)
// {
//   return;
//   blitz::Array<int, 3> data(900,1000,1024);
//   blitz::Array<double, 2> coeff(1000,1024);
//   data = 1;
//   coeff = 2;
//   blitz::Array<double, 3> res(900,1000,1024);
//   std::cerr << "Hi, starting.\n";
//   std::cerr << omp_get_max_threads() << " threads\n";
//   omp_set_num_threads(10);
//   std::cerr << omp_get_max_threads() << " threads\n";
// #pragma omp parallel for  
//   for(int i1 = 0; i1 < data.rows(); ++i1) {
//     std::cerr << "Hi from " << omp_get_thread_num() << "\n";
//     for(int i2 = 0; i2 < data.cols(); ++i2)
//       for(int i3 = 0; i3 < data.depth(); ++i3) {
// 	res(i1,i2,i3) = coeff(i2,i3) * data(i1, i2, i3);
//       }
//   }
//   std::cerr << "Hi, done.\n";
// }

BOOST_AUTO_TEST_SUITE_END()
