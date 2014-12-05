#include "unit_test_support.h"
#include "usgs_dem.h"
#include <boost/filesystem.hpp>
#include <iostream>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(usgs_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(usgs_dem_data)
{
  UsgsDemData usgs_data(test_data_dir() + "usgs_dem", false);
  double val = usgs_data.interpolate(usgs_data.coordinate(Geodetic(43.5, -68.5)));
  BOOST_CHECK_CLOSE(val, 0.0, 1e-4);
}

BOOST_AUTO_TEST_CASE(usgs_dem_data_real_database)
{
  // Old unit test, requires that we have actual database. If we don't
  // then just skip this test.
  if(boost::filesystem::is_directory("/data/bank/anc/DEM/USA10M/database")) {
    UsgsDemData usgs_data("/data/bank/anc/DEM/USA10M/database", false);
    double latitude = 34.904444;
    double longitude = -119.263411;
    double lat = (latitude + (1.0 / 10800.0));
    double lon = (longitude + (.5 / 10800.0));
    boost::shared_ptr<GroundCoordinate> gc
      (usgs_data.map_info().coordinate_converter().
       convert_from_coordinate(lon, lat));
    
    // The old unit test value is 1454, which I verified using
    //gdallocationinfo is *wrong*. Not sure where this value came
    //from, but the following value appears to be correct.
    // BOOST_CHECK_CLOSE(usgs_data.interpolate(usgs_data.coordinate(*gc)), 1454)
    BOOST_CHECK_CLOSE(usgs_data.interpolate(usgs_data.coordinate(*gc)), 1450.25, 1e-2);
  }
}

BOOST_AUTO_TEST_CASE(usgs_dem)
{
  // DatumGeoid96 might not be available, so for the test just use a
  // SimpleDatum.
  boost::shared_ptr<Datum> datum(new SimpleDatum(10.0));
  UsgsDem d(test_data_dir() + "usgs_dem", false, datum);
  BOOST_CHECK_CLOSE(d.height_reference_surface(Geodetic(43.5, -68.5)),
		    10, 1e-4);
}
BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_BOOST_SERIALIZATON
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  boost::shared_ptr<Datum> datum(new SimpleDatum(10.0));
  boost::shared_ptr<Dem> d(new UsgsDem(test_data_dir() + "usgs_dem", false, 
				       datum));
  oa << GEOCAL_NVP(d);
  if(false)
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<Dem> dr;
  ia >> GEOCAL_NVP(dr);

  BOOST_CHECK_CLOSE(dr->height_reference_surface(Geodetic(43.5, -68.5)),
		    10, 1e-4);
#endif
}


BOOST_AUTO_TEST_CASE(usgs_dem_real_database)
{
  // Old unit test, requires that we have actual database. If we don't
  // then just skip this test.
  if(boost::filesystem::is_directory("/data/bank/anc/DEM/USA10M/database")) {
    // DatumGeoid96 might not be available, so for the test just use a
    // SimpleDatum.
    boost::shared_ptr<Datum> datum(new SimpleDatum(10.0));
    UsgsDem d("/data/bank/anc/DEM/USA10M/database", false, datum);
    double latitude = 34.904444;
    double longitude = -119.263411;
    double lat = (latitude + (1.0 / 10800.0));
    double lon = (longitude + (.5 / 10800.0));
    BOOST_CHECK_CLOSE(d.height_reference_surface(Geodetic(lat, lon)),
		      1460.255, 1e-2);
  }
}

BOOST_AUTO_TEST_SUITE_END()
