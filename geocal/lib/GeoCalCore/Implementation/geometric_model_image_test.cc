#include "unit_test_support.h"
#include "geometric_model_image.h"
#include "quadratic_geometric_model.h"
#include "vicar_lite_file.h"
using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(geometric_model_image, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  // This matches some data that appears in the old picmtch4 tests.
  boost::shared_ptr<RasterImage> img(new VicarLiteRasterImage(test_data_dir() + "phase_correlation/xxim2"));
  boost::shared_ptr<QuadraticGeometricModel> geom_model(new QuadraticGeometricModel());
  GeometricModelImage gimg(img, geom_model, 300, 300);
  GeometricTiePoints tp;
  tp.add_point(ImageCoordinate(1,2), ImageCoordinate(1,1));
  tp.add_point(ImageCoordinate(1,310), ImageCoordinate(1,301));
  tp.add_point(ImageCoordinate(309,2), ImageCoordinate(301, 1));
  geom_model->fit_transformation(tp);
  Array<int, 2> data = gimg.read(150 - 96 / 2, 150 - 96 / 2, 96, 96);
  BOOST_CHECK(data(0, 0) == 114);
  BOOST_CHECK(data(0, 1) == 154);
}

BOOST_AUTO_TEST_SUITE_END()
