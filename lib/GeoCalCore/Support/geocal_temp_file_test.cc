#include "unit_test_support.h"
#include "geocal_temp_file.h"
#include <fstream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_temp_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  std::string fname;
  {
    GeoCalTempFile tf;
    fname = tf.temp_fname();
    std::ofstream f(fname);
    f << "Hi there\n";
    BOOST_CHECK(boost::filesystem::exists(fname));
  }
  BOOST_CHECK(!boost::filesystem::exists(fname));
}

BOOST_AUTO_TEST_SUITE_END()

