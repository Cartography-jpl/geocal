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
  ImageCoordinate icres_exp(146.13, 145.156);
  BOOST_CHECK(m.image_coordinate(ic) == icres_exp);
  QuadraticGeometricModel m2;
  GeometricTiePoints tp;
  tp.add_point(ImageCoordinate(1,2), ImageCoordinate(1,1));
  tp.add_point(ImageCoordinate(1,310), ImageCoordinate(1,301));
  tp.add_point(ImageCoordinate(309,2), ImageCoordinate(301, 1));
  BOOST_CHECK(m2.image_coordinate(ic) != icres_exp);
  m2.fit_transformation(tp);
  BOOST_CHECK(m2.image_coordinate(ic) == icres_exp);
}

BOOST_AUTO_TEST_SUITE_END()
