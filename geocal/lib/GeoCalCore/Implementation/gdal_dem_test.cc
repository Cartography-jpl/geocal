#include "unit_test_support.h"
#include "gdal_dem.h"    
#include "gdal_datum.h"
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

BOOST_AUTO_TEST_CASE(gdal_dem_scale)
{
  // This DEM has the data in US survey foot instead of meter. This
  // test checks to make sure we handle this conversion correctly.
  //
  // This DEM is actually relative to North American Datum 1983, which
  // has a datum of NADV 88. GDAL web site supplies this as the file
  // g2012a_conus.gtx. However this is a bit big for including as a
  // unit test input. So we fake this as having no datum. Since we are
  // just testing the handling of the scaling, this doesn't affect the
  // actual test.
  
  std::string fname = test_data_dir() + "dem_foot.tif";
  GdalDem d(fname, boost::shared_ptr<Datum>(new SimpleDatum(10)));
  Geodetic g1(34, -117.9);
  BOOST_CHECK_CLOSE(d.height_datum(g1), 133.502667, 1e-4);
  BOOST_CHECK_CLOSE(d.height_reference_surface(g1), 133.502667 + 10.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
