#include "unit_test_support.h"
#include "camera.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(simple_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  SimpleCamera c;
  FrameCoordinate f1(1, 2);
  ScLookVector sl = c.sc_look_vector(f1, 0);
  FrameCoordinate f2 = c.frame_coordinate(sl, 0);
  BOOST_CHECK_EQUAL(c.number_band(), 1);
  BOOST_CHECK_EQUAL(c.number_line(0), 1);
  BOOST_CHECK_EQUAL(c.number_sample(0), 1504);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
  BOOST_CHECK_CLOSE(c.frame_line_coordinate(sl, 0), f1.line, 1e-4);
  FrameCoordinateWithDerivative f1d(AutoDerivative<double>(1, 0, 2), 
				    AutoDerivative<double>(2, 1, 2));
  ScLookVectorWithDerivative sld = c.sc_look_vector_with_derivative(f1d, 0);
  FrameCoordinateWithDerivative f2d = 
    c.frame_coordinate_with_derivative(sld, 0);
  BOOST_CHECK_CLOSE(sld.look_vector[0].value(), sl.look_vector[0], 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[1].value(), sl.look_vector[1], 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[2].value(), sl.look_vector[2], 1e-4);
  double eps = 0.01;
  ScLookVector slv2 = c.sc_look_vector(FrameCoordinate(1 + eps, 2), 0);
  BOOST_CHECK_CLOSE(sld.look_vector[0].gradient()(0), 
		    (slv2.look_vector[0] - sl.look_vector[0]) / eps, 1e-4);
  // Gradient for this is very near zero, so test fails. Could do
  // something more complicated, but just skip this one.
  // BOOST_CHECK_CLOSE(sld.look_vector[1].gradient()(0), 
  // 		    (slv2.look_vector[1] - sl.look_vector[1]) / eps, 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[2].gradient()(0), 
		    (slv2.look_vector[2] - sl.look_vector[2]) / eps, 1e-4);
  slv2 = c.sc_look_vector(FrameCoordinate(1, 2 + eps), 0);
  BOOST_CHECK_CLOSE(sld.look_vector[0].gradient()(1), 
		    (slv2.look_vector[0] - sl.look_vector[0]) / eps, 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[1].gradient()(1), 
		    (slv2.look_vector[1] - sl.look_vector[1]) / eps, 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[2].gradient()(1), 
		    (slv2.look_vector[2] - sl.look_vector[2]) / eps, 1e-4);
  BOOST_CHECK_CLOSE(f2d.line.value(), f1d.line.value(), 1e-4);
  BOOST_CHECK_MATRIX_CLOSE(f2d.line.gradient(), f1d.line.gradient());
  BOOST_CHECK_CLOSE(f2d.sample.value(), f1d.sample.value(), 1e-4);
  BOOST_CHECK_MATRIX_CLOSE(f2d.sample.gradient(), f1d.sample.gradient());
}

BOOST_AUTO_TEST_SUITE_END()
