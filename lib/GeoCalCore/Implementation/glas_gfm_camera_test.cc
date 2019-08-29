#include "unit_test_support.h"
#include "glas_gfm_camera.h"
#include "quaternion_camera.h"
#include <iostream>
#include <boost/make_shared.hpp>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(glas_gfm_camera, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  GlasGfmCamera cam;
  // Have two FA pairs, even though they are redundant. This tests the
  // handling of this in the code.
  cam.delta_sample_pair(128);
  blitz::Array<double, 2> fa(2, 4);
  fa = -0.00765, 0, 0, 0,
    0, 0, 0.00765, 0;
  cam.field_alignment(fa);
  // GlasGfmCamera should have the same values as this QuaternionCamera.
  QuaternionCamera qcam(boost::math::quaternion<double>(1,0,0,0),
			1, 256, 0.00765 / 128, 0.00765 / 128,
			1.41009182,
			FrameCoordinate(0, 128),
			QuaternionCamera::LINE_IS_Y,
			QuaternionCamera::INCREASE_IS_NEGATIVE,
			QuaternionCamera::INCREASE_IS_POSITIVE);  
  BOOST_CHECK_EQUAL(cam.number_band(), 1);
  BOOST_CHECK_EQUAL(cam.number_line(0), 1);
  BOOST_CHECK_EQUAL(cam.number_sample(0), 256);
  BOOST_CHECK_CLOSE(cam.focal_length(), 1.41009182, 1e-4);
  BOOST_CHECK_EQUAL(cam.sensor_type(), "S");
  if(false) {
    FrameCoordinate fc(-0.5, 0);
    std::cerr << qcam.frame_coordinate(cam.sc_look_vector(fc, 0),0) << "\n"
	      << cam.frame_coordinate(qcam.sc_look_vector(fc, 0),0) << "\n";
    return;
  }
  for(int i = 0; i < 256; ++i) {
    FrameCoordinate fc1(-0.5, i);
    BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fc1, 0),0).line
		     - fc1.line) < 1e-6);
    BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fc1, 0),0).sample
		     - fc1.sample) < 1e-6);
    BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fc1, 0),0).line
		     - fc1.line) < 1e-6);
    BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fc1, 0),0).sample
		     - fc1.sample) < 1e-6);
    FrameCoordinate fc2(0.5, i);
    BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fc2, 0),0).line
		     - fc2.line) < 1e-6);
    BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fc2, 0),0).sample
		     - fc2.sample) < 1e-6);
    BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fc2, 0),0).line
		     - fc2.line) < 1e-6);
    BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fc2, 0),0).sample
		     - fc2.sample) < 1e-6);
    FrameCoordinateWithDerivative fc3(-0.5, i);
    BOOST_CHECK(fabs(qcam.frame_coordinate_with_derivative
		     (cam.sc_look_vector_with_derivative(fc3, 0),0).line.value()
		     - fc3.line.value()) < 1e-6);
    BOOST_CHECK(fabs(qcam.frame_coordinate_with_derivative
       (cam.sc_look_vector_with_derivative(fc3, 0),0).sample.value()
		     - fc3.sample.value()) < 1e-6);
    BOOST_CHECK(fabs(cam.frame_coordinate_with_derivative
        (qcam.sc_look_vector_with_derivative(fc3, 0),0).line.value()
		     - fc3.line.value()) < 1e-6);
    BOOST_CHECK(fabs(cam.frame_coordinate_with_derivative
        (qcam.sc_look_vector_with_derivative(fc3, 0),0).sample.value()
		     - fc3.sample.value()) < 1e-6);
    FrameCoordinateWithDerivative fc4(0.5, i);
    BOOST_CHECK(fabs(qcam.frame_coordinate_with_derivative
		     (cam.sc_look_vector_with_derivative(fc4, 0),0).line.value()
		     - fc4.line.value()) < 1e-6);
    BOOST_CHECK(fabs(qcam.frame_coordinate_with_derivative
       (cam.sc_look_vector_with_derivative(fc4, 0),0).sample.value()
		     - fc4.sample.value()) < 1e-6);
    BOOST_CHECK(fabs(cam.frame_coordinate_with_derivative
        (qcam.sc_look_vector_with_derivative(fc4, 0),0).line.value()
		     - fc4.line.value()) < 1e-6);
    BOOST_CHECK(fabs(cam.frame_coordinate_with_derivative
        (qcam.sc_look_vector_with_derivative(fc4, 0),0).sample.value()
		     - fc4.sample.value()) < 1e-6);
  }

  // Test handling of extended camera for image coordinates outside of
  // the actual camera range.
  FrameCoordinate fext(-5, 128);
  BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fext, 0),0).line
		   - fext.line) < 1e-6);
  BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fext, 0),0).sample
		   - fext.sample) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fext, 0),0).line
		   - fext.line) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fext, 0),0).sample
		   - fext.sample) < 1e-6);
  fext = FrameCoordinate(5, 128);
  BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fext, 0),0).line
		   - fext.line) < 1e-6);
  BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fext, 0),0).sample
		   - fext.sample) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fext, 0),0).line
		   - fext.line) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fext, 0),0).sample
		   - fext.sample) < 1e-6);
  fext = FrameCoordinate(0, -10);
  BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fext, 0),0).line
		   - fext.line) < 1e-6);
  BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fext, 0),0).sample
		   - fext.sample) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fext, 0),0).line
		   - fext.line) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fext, 0),0).sample
		   - fext.sample) < 1e-6);
  fext = FrameCoordinate(0, 256 + 10);
  BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fext, 0),0).line
		   - fext.line) < 1e-6);
  BOOST_CHECK(fabs(qcam.frame_coordinate(cam.sc_look_vector(fext, 0),0).sample
		   - fext.sample) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fext, 0),0).line
		   - fext.line) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(qcam.sc_look_vector(fext, 0),0).sample
		   - fext.sample) < 1e-6);
  
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Camera> cam(new GlasGfmCamera());
  std::string d = serialize_write_string(cam);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Camera> camr = serialize_read_string<Camera>(d);
}

BOOST_AUTO_TEST_SUITE_END()
