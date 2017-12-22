#include "unit_test_support.h"
#include "quadratic_geometric_model.h"
using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(quadratic_geometric_model, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<GeometricTiePoints> tp =
    boost::make_shared<GeometricTiePoints>();
  tp->add_point(ImageCoordinate(1,2), ImageCoordinate(1,1));
  tp->add_point(ImageCoordinate(1,310), ImageCoordinate(1,301));
  tp->add_point(ImageCoordinate(309,2), ImageCoordinate(301, 1));
  ImageCoordinate ic(150, 150);
  ImageCoordinate icres_exp(146.13, 145.156);
  QuadraticGeometricModel m(tp, 9);
  BOOST_CHECK(m.original_image_coordinate(ic) == icres_exp);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<GeometricTiePoints> tp =
    boost::make_shared<GeometricTiePoints>();
  tp->add_point(ImageCoordinate(1,2), ImageCoordinate(1,1));
  tp->add_point(ImageCoordinate(1,310), ImageCoordinate(1,301));
  tp->add_point(ImageCoordinate(309,2), ImageCoordinate(301, 1));
  boost::shared_ptr<QuadraticGeometricModel> m =
    boost::make_shared<QuadraticGeometricModel>(tp, 9);
  std::string d = serialize_write_string(m);
  if(false)
    std::cerr << d;
  boost::shared_ptr<QuadraticGeometricModel> mr =
    serialize_read_string<QuadraticGeometricModel>(d);
  ImageCoordinate ic(150, 150);
  ImageCoordinate icres_exp(146.13, 145.156);
  BOOST_CHECK(mr->original_image_coordinate(ic) == icres_exp);
}
BOOST_AUTO_TEST_SUITE_END()
