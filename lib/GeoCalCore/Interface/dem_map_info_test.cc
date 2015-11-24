#include "unit_test_support.h"
#include <boost/test/floating_point_comparison.hpp>
#include "dem_map_info.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(dem_map_info, GlobalFixture)

class DemMapInfoTest : public DemMapInfo {
public:
  DemMapInfoTest() : DemMapInfo(boost::shared_ptr<Datum>(new SimpleDatum(10)),
    MapInfo(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter),
	    50, 60, 50 + 100, 60 + 50, 100, 50)) {}
  virtual ~DemMapInfoTest() {}
  virtual void print(std::ostream& Os) const { Os << "DemMapInfoTest\n";}
  virtual double elevation(int Y_index, int X_index) const
  { return 2 * Y_index + X_index; }
};

BOOST_AUTO_TEST_CASE(dem_map_info_test)
{
  DemMapInfoTest d;
  BOOST_CHECK(fabs(d.height_reference_surface(Geodetic(30, 40, 0)) - 
		   0.0) < 1e-6);
  double height1 = 100;
  Geodetic g1(81, 56, height1);
  double height_expect = 46.5;
  BOOST_CHECK_CLOSE(d.height_datum(g1), height_expect, 1e-4);
  BOOST_CHECK_CLOSE(d.height_reference_surface(g1), 
		    height_expect + 10, 1e-4);
  BOOST_CHECK_CLOSE(d.distance_to_surface(g1), height1 - (height_expect + 10), 
		    1e-4);
  Geodetic g1_expect(g1.latitude(), g1.longitude(), height_expect + 10);
  BOOST_CHECK(distance(*d.surface_point(g1), g1_expect) < 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
