#include "unit_test_support.h"
#include "quadratic_geometric_model.h"
using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(quadratic_geometric_model, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  Array<double, 1> trans(12);
  trans = 0.974026, 0, 0.025974, 0, 0, 0, 0, 0.974026, -0.948052, 0, 0, 0;
  QuadraticGeometricModel m(trans);
  ImageCoordinate ic(150, 150);
  ImageCoordinate icres = m.image_coordinate(ic);
  ImageCoordinate icres_exp(146.13, 145.156);
  BOOST_CHECK(icres == icres_exp);
}

BOOST_AUTO_TEST_SUITE_END()
