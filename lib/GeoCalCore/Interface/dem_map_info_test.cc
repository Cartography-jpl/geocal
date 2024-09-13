#include "unit_test_support.h"
#include "dem_map_info.h"
#include "gdal_dem.h"
#include "srtm_dem.h"
#include <boost/filesystem.hpp>
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(dem_map_info, GlobalFixture)

class DemMapInfoTest : public DemMapInfo {
public:
  DemMapInfoTest() : DemMapInfo(boost::shared_ptr<Datum>(new SimpleDatum(10)),
    MapInfo(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter),
	    50, 60, 50 + 100, 60 + 25, 100, 25)) {}
  virtual ~DemMapInfoTest() {}
  virtual void print(std::ostream& Os) const { Os << "DemMapInfoTest\n";}
  virtual double elevation(int Y_index, int X_index) const
  {
    return 2 * Y_index + X_index; }
};

BOOST_AUTO_TEST_CASE(dem_map_info_test)
{
  DemMapInfoTest d;
  BOOST_CHECK(fabs(d.height_reference_surface(Geodetic(30, 40, 0)) - 
		   0.0) < 1e-6);
  double height1 = 100;
  Geodetic g1(81.5, 56.5, height1);
  double height_expect = 2*21 + 6;
  BOOST_CHECK_CLOSE(d.height_datum(g1), height_expect, 1e-4);
  BOOST_CHECK_CLOSE(d.height_reference_surface(g1), 
		    height_expect + 10, 1e-4);
  BOOST_CHECK_CLOSE(d.distance_to_surface(g1), height1 - (height_expect + 10), 
		    1e-4);
  Geodetic g1_expect(g1.latitude(), g1.longitude(), height_expect + 10);
  BOOST_CHECK(distance(*d.surface_point(g1), g1_expect) < 1e-4);
  double min_h, max_h;
  double pad = 5;
  d.height_range(Geodetic(60.6,50.6,0), Geodetic(80.9, 55.9, 0),
		 min_h, max_h, pad);
  double min_h_expect = 0 + 10 - pad;
  double max_h_expect = height_expect + 10 + pad;
  BOOST_CHECK_CLOSE(min_h, min_h_expect, 1e-4);
  BOOST_CHECK_CLOSE(max_h, max_h_expect, 1e-4);
}

BOOST_AUTO_TEST_CASE(slope_aspect)
{
  // We calculated these values using the python library richdem to
  // compare. Note there are slight differences, richdem uses float32
  // while we use double, we also convert everything to meters (with
  // slight round off) and richdem leaves in the native length (feet
  // in this case). But we should mostly agree.
  //
  // Note I also compared to gdaldem
  // (see https://gdal.org/programs/gdaldem.html)
  // gdaldem aspect dem_foot.tif aspect.tif -alg Horn
  // gdaldem slope dem_foot.tif slope.tif -alg Horn
  // This gives the same values as richdem, and matches what we have here.
  GdalDem d(test_data_dir() + "dem_foot.tif");
  BOOST_CHECK_CLOSE(d.slope_riserun(20,19), 0.6217460352558923, 1e-2);
  BOOST_CHECK_CLOSE(d.slope_percentage(20,19), 0.6217460352558923 * 100, 1e-2);
  BOOST_CHECK_CLOSE(d.slope_radian(20,19), atan(0.6217460352558923), 1e-2);
  BOOST_CHECK_CLOSE(d.slope_degree(20,19),
		    atan(0.6217460352558923) * Constant::rad_to_deg, 1e-2);
  // Check each quadrant, to make sure we handle correctly
  BOOST_CHECK_CLOSE(d.aspect(20,19), 30.17352, 1e-2);
  BOOST_CHECK_CLOSE(d.aspect(2,59), 138.81407, 1e-2);
  BOOST_CHECK_CLOSE(d.aspect(3,33), 213.36636, 1e-2);
  BOOST_CHECK_CLOSE(d.aspect(2,87), 326.30994, 1e-2);
  double slope, aspect;
  d.slope_and_aspect(*d.map_info().ground_coordinate(19,20), slope, aspect);
  BOOST_CHECK_CLOSE(slope, atan(0.6217460352558923) * Constant::rad_to_deg,
		    1e-2);
  BOOST_CHECK_CLOSE(aspect, 30.17352, 1e-2);
  
  try {
    std::string dbase = SrtmDem().directory_base();
    GdalDem d2(dbase + "/n34e119_L2.hlf");
    // Check handling of flat areas
    BOOST_CHECK_EQUAL(d2.slope_riserun(3555,3560),0.0);
    BOOST_CHECK_EQUAL(d2.aspect(3555,3560),270.0);
    // Check handling where aspect cross 0/360 line
    BOOST_CHECK(d2.aspect(2448,826) > 350);
    BOOST_CHECK(d2.aspect(2448+1,826) < 10);
    d2.slope_and_aspect(*d2.map_info().ground_coordinate(826+0.5,2448+0.5),
			slope, aspect);
    BOOST_CHECK_CLOSE(slope, 48.81526238991841, 1e-4);
    BOOST_CHECK_CLOSE(aspect, 358.99614600730774, 1e-4);
  } catch(const boost::filesystem::filesystem_error&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
  }  
}

BOOST_AUTO_TEST_SUITE_END()
