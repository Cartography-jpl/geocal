#include "unit_test_support.h"
#include "gdal_datum.h"
#include "geodetic.h"

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(gdal_datum, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  std::string fname = test_data_dir() + "egm96.img";
  GdalDatum datum(fname);
  BOOST_CHECK_CLOSE(datum.undulation(Geodetic(40, -100, 0)), -25.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
