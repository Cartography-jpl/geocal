#include "unit_test_support.h"
#include "geocal_static_sort.h"
#include <random>
#include <algorithm>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(geocal_static_sort, GlobalFixture)

bool is_sorted(const std::vector<double>& d)
{
  for(int i = 0; i < (int) d.size()-1; ++i)
    if(d[i] > d[i+1])
      return false;
  return true;
}

BOOST_AUTO_TEST_CASE(basic)
{
  std::mt19937 rnd;
  for(int i = 2; i < 31; ++i) {
    //std::cerr << "Checking " << i << "\n";
    std::vector<double> v;
    for(int j = 0; j < i; ++j)
      v.push_back(j);
    std::shuffle(v.begin(), v.end(), rnd);
    // Might just be sorted by luck - force to be out of order
    if(is_sorted(v))
      std::swap(v[0],v[1]);
    BOOST_CHECK(!is_sorted(v));
    static_sort(v);
    BOOST_CHECK(is_sorted(v));
  }
}

BOOST_AUTO_TEST_SUITE_END()

