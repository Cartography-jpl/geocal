#include "unit_test_support.h"
#include "gdal_dem.h"    
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(gdal_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(gdal_dem_test)
{
  std::string fname = test_data_dir() + "cib_sample.img";
  GdalDem d(fname, boost::shared_ptr<Datum>(new SimpleDatum(10)));
  Geodetic g1(35.895, 44.800, 100);
  BOOST_CHECK_CLOSE(d.height_datum(g1), 69.6, 1e-4);
  BOOST_CHECK_CLOSE(d.height_reference_surface(g1), 69.6 + 10.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
