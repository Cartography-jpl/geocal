#include "unit_test_support.h"
#include "srtm_90m_dem.h"
#include "gdal_dem.h"
#include "gdal_raster_image.h"
#include <boost/filesystem.hpp>
#include <iostream>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(srtm_90m_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(srtm_90m_dem_vs_gdaldem)
{
  // make sure Srtm90mDem and GdalDem gives the same height_reference_surface
  if(!boost::filesystem::is_directory("/data/bank/anc/DEM/srtm_v3_dem_L1"))
      return;
  // DatumGeoid96 might not be available, so for the test just use a
  // SimpleDatum.
  boost::shared_ptr<Datum> datum(new SimpleDatum(0.0));
  Srtm90mDem d("/data/bank/anc/DEM/srtm_v3_dem_L1", false, datum);
  GdalDem dGdal(test_data_dir() + "srtm_90m_dem/n34w120_L2.tif");

  double latitude = 34.904444;
  double longitude = -119.263411;
  double lat = (latitude + (1.0 / 10800.0));
  double lon = (longitude + (.5 / 10800.0));
  double h = d.height_reference_surface(Geodetic(lat, lon));
  double hGdal = dGdal.height_reference_surface(Geodetic(lat, lon));
  BOOST_CHECK_CLOSE(h, hGdal, 1e-4);
}

BOOST_AUTO_TEST_CASE(srtm_90m_dem_data)
{
  Srtm90mData srtm_90m_data(test_data_dir() + "srtm_90m_dem", false);
  double val = srtm_90m_data.interpolate(srtm_90m_data.coordinate(Geodetic(43.5, -68.5)));
  BOOST_CHECK_CLOSE(val, 0.0, 1e-4);
}

BOOST_AUTO_TEST_CASE(srtm_90m_dem_data_real_database)
{
  // Old unit test, requires that we have actual database. If we don't
  // then just skip this test.
  if(boost::filesystem::is_directory("/data/bank/anc/DEM/srtm_v3_dem_L1")) {
	Srtm90mData srtm_90m_data("/data/bank/anc/DEM/srtm_v3_dem_L1", false);
    double latitude = 34.904444;
    double longitude = -119.263411;
    double lat = (latitude + (1.0 / 10800.0));
    double lon = (longitude + (.5 / 10800.0));
    boost::shared_ptr<GroundCoordinate> gc
      (srtm_90m_data.map_info().coordinate_converter().
       convert_from_coordinate(lon, lat));
    
    // The test with 10m USGS DEM gives value 1450.25
    // Test with srtm 90m DEM gives value 1466.406.
    // BOOST_CHECK_CLOSE(srtm_90m_data.interpolate(srtm_90m_data.coordinate(*gc)), 1450.25, 1e-2);
    BOOST_CHECK_CLOSE(srtm_90m_data.interpolate(srtm_90m_data.coordinate(*gc)), 1466.406, 1e-2);
  }
}

BOOST_AUTO_TEST_CASE(srtm_90m_dem)
{
  // DatumGeoid96 might not be available, so for the test just use a
  // SimpleDatum.
  boost::shared_ptr<Datum> datum(new SimpleDatum(10.0));
  Srtm90mDem d(test_data_dir() + "srtm_90m_dem", false, datum);
  BOOST_CHECK_CLOSE(d.height_reference_surface(Geodetic(43.5, -68.5)),
		    10, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization_srtm_90m_dem_data)
{
  if(!have_serialize_supported())
    return;

  boost::shared_ptr<Srtm90mData> srtm_90m_data
    (new Srtm90mData(test_data_dir() + "srtm_90m_dem", false));
  std::string d = serialize_write_string(srtm_90m_data);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Srtm90mData> srtm_90m_datar =
    serialize_read_string<Srtm90mData>(d);
  double val = srtm_90m_datar->interpolate(srtm_90m_datar->coordinate(Geodetic(43.5, -68.5)));
  BOOST_CHECK_CLOSE(val, 0.0, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{

  if(!have_serialize_supported())
    return;

  boost::shared_ptr<Datum> datum(new SimpleDatum(10.0));
  boost::shared_ptr<Dem> dem(new Srtm90mDem(test_data_dir() + "srtm_90m_dem", false,
				       datum));
  std::string d = serialize_write_string(dem);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Dem> demr =
    serialize_read_string<Dem>(d);
  BOOST_CHECK_CLOSE(demr->height_reference_surface(Geodetic(43.5, -68.5)),
		    10, 1e-4);
}


BOOST_AUTO_TEST_CASE(srtm_90m_dem_real_database)
{
  // Old unit test, requires that we have actual database. If we don't
  // then just skip this test.
  if(!boost::filesystem::is_directory("/data/bank/anc/DEM/srtm_v3_dem_L1"))
    return;
  // DatumGeoid96 might not be available, so for the test just use a
  // SimpleDatum.
  boost::shared_ptr<Datum> datum(new SimpleDatum(10.0));
  Srtm90mDem d("/data/bank/anc/DEM/srtm_v3_dem_L1", false, datum);
  double latitude = 34.904444;
  double longitude = -119.263411;
  double lat = (latitude + (1.0 / 10800.0));
  double lon = (longitude + (.5 / 10800.0));
  // The test with 10m USGS DEM gives value 1460.255
  // Test with srtm 90m DEM  gives value 1476.406.
  BOOST_CHECK_CLOSE(d.height_reference_surface(Geodetic(lat, lon)),
		  1476.406, 1e-2);
}

BOOST_AUTO_TEST_CASE(create_subset_file)
{
  // Don't normally run this test, it takes a bit of time to run
  //return;
  if(!boost::filesystem::is_directory("/data/bank/anc/DEM/srtm_v3_dem_L1"))
    return;
  Srtm90mData d("/data/bank/anc/DEM/srtm_v3_dem_L1");
  std::vector<boost::shared_ptr<GroundCoordinate> > pt;
  pt.push_back(boost::shared_ptr<GroundCoordinate>(new Geodetic(40.5, -110.5)));
  pt.push_back(boost::shared_ptr<GroundCoordinate>(new Geodetic(37.5, -106.7)));
  d.create_subset_file("test_srtm_90m_dem.tif", "gtiff", pt);

  boost::shared_ptr<Datum> datum(new SimpleDatum(10.0));
  Srtm90mDem dtrue("/data/bank/anc/DEM/srtm_v3_dem_L1", false, datum);
  GdalRasterImage d2("test_srtm_90m_dem.tif");
  GdalDem dcreated("test_srtm_90m_dem.tif", datum);
  Geodetic g(38, -108);
  BOOST_CHECK_CLOSE(dtrue.height_reference_surface(g),
		    dcreated.height_reference_surface(g), 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
