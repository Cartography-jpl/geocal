#include "unit_test_support.h"
#include "gdal_dem.h"    
#include "dem_map_info_offset.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(dem_map_info_offset, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  std::string fname = test_data_dir() + "cib_sample.img";
  boost::shared_ptr<DemMapInfo> 
    d(new GdalDem(fname, boost::shared_ptr<Datum>(new SimpleDatum(10))));
  Geodetic g1(35.895, 44.800, 100);
  BOOST_CHECK_CLOSE(d->height_datum(g1), 69.6, 1e-4);
  BOOST_CHECK_CLOSE(d->height_reference_surface(g1), 69.6 + 10.0, 1e-4);
  DemMapInfoOffset doff(d, 100.0);
  BOOST_CHECK_CLOSE(doff.height_datum(g1), 69.6 + 100, 1e-4);
  BOOST_CHECK_CLOSE(doff.height_reference_surface(g1), 69.6 + 10.0 + 100, 1e-4);
}
BOOST_AUTO_TEST_SUITE_END()