#include "unit_test_support.h"
#include "coordinate_converter.h"
#include "geodetic.h"
#include "ecr.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geodetic_converter, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  GeodeticConverter gc;
  boost::shared_ptr<GroundCoordinate> gp = 
    gc.convert_from_coordinate(40, 30, 50);
  Geodetic g(*gp);
  BOOST_CHECK_CLOSE(g.latitude(), 30.0, 1e-4);
  BOOST_CHECK_CLOSE(g.longitude(), 40.0, 1e-4);
  BOOST_CHECK_CLOSE(g.height_reference_surface(), 50.0, 1e-4);
  Ecr g_ecr(g);
  double x, y, height;
  gc.convert_to_coordinate(g_ecr, x, y, height);
  BOOST_CHECK_CLOSE(y, 30.0, 1e-4);
  BOOST_CHECK_CLOSE(x, 40.0, 1e-4);
  BOOST_CHECK_CLOSE(height, 50.0, 1e-4);
  GeodeticConverter gc2;
  BOOST_CHECK(gc.is_same(gc2));
}

BOOST_AUTO_TEST_SUITE_END()
