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
