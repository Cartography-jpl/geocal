#include "unit_test_support.h"
#include "glas_gfm_camera.h"
#include "camera_radial_distortion.h"
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

BOOST_AUTO_TEST_CASE(gfm_test)
{
  blitz::Array<double, 1> k_radial(3);
  k_radial = -0.00734339259200545000000, 0.00002837587863624170000,
    0.00000001284198912402710;
  // SPICE OD_K uses opposite sign convention
  k_radial *= -1;
  double focal_length = 352.9271664;
  double pitch = 7e-3;
  int left_masked = 38;
  CameraRadialDistortion rcam(boost::math::quaternion<double>(1,0,0,0),
			     k_radial,
			     2048, 2048, pitch, pitch, focal_length,
			     FrameCoordinate(0.430442527 - 0.5,
					     2543.46099 - 0.5 +left_masked));
  GlasGfmCamera cam(rcam, 0, 10, 10);
  BOOST_CHECK_EQUAL(cam.number_band(), 1);
  BOOST_CHECK_EQUAL(cam.number_line(0), 2048);
  BOOST_CHECK_EQUAL(cam.number_sample(0), 2048);
  BOOST_CHECK_CLOSE(cam.focal_length(), focal_length * 1e-3, 1e-4);
  BOOST_CHECK_EQUAL(cam.sensor_type(), "F");
  //int step = 1;
  int test_step = 20;
  for(int i = 0; i < cam.number_line(0); i += test_step) 
    for(int j = 0; j < cam.number_sample(0); j += test_step) {
      if(false)
	if(j == 0 && i % 10 == 0)
	  std::cerr << "Starting line " << i << "\n";
      FrameCoordinate fc(i,j);
      BOOST_CHECK(fabs(rcam.frame_coordinate(cam.sc_look_vector(fc, 0),0).line
		     - fc.line) < 1e-3);
      BOOST_CHECK(fabs(rcam.frame_coordinate(cam.sc_look_vector(fc, 0),0).sample
		     - fc.sample) < 1e-3);
      BOOST_CHECK(fabs(cam.frame_coordinate(rcam.sc_look_vector(fc, 0),0).line
		     - fc.line) < 1e-3);
      BOOST_CHECK(fabs(cam.frame_coordinate(rcam.sc_look_vector(fc, 0),0).sample
		     - fc.sample) < 1e-3);
      FrameCoordinateWithDerivative fc2(AutoDerivative<double>(i, 0, 2),
					AutoDerivative<double>(j, 1, 2));
      BOOST_CHECK(fabs(rcam.frame_coordinate_with_derivative(cam.sc_look_vector_with_derivative(fc2, 0),0).line.value()
		       - fc2.line.value()) < 1e-3);
      BOOST_CHECK(fabs(rcam.frame_coordinate_with_derivative(cam.sc_look_vector_with_derivative(fc2, 0),0).sample.value()
		       - fc2.sample.value()) < 1e-3);
      BOOST_CHECK(fabs(cam.frame_coordinate_with_derivative(rcam.sc_look_vector_with_derivative(fc2, 0),0).line.value()
		       - fc2.line.value()) < 1e-3);
      BOOST_CHECK(fabs(cam.frame_coordinate_with_derivative(rcam.sc_look_vector_with_derivative(fc2, 0),0).sample.value()
		       - fc2.sample.value()) < 1e-3);
    }
}

BOOST_AUTO_TEST_CASE(glas_test)
{
  blitz::Array<double, 1> k_radial(3);
  k_radial = -0.00734339259200545000000, 0.00002837587863624170000,
    0.00000001284198912402710;
  // SPICE OD_K uses opposite sign convention
  k_radial *= -1;
  double focal_length = 352.9271664;
  double pitch = 7e-3;
  int left_masked = 38;
  CameraRadialDistortion rcam(boost::math::quaternion<double>(1,0,0,0),
			     k_radial,
			     1, 2048, pitch, pitch, focal_length,
			     FrameCoordinate(0.430442527 - 0.5,
					     2543.46099 - 0.5 +left_masked));
  GlasGfmCamera cam(rcam, 0, 10);
  BOOST_CHECK_EQUAL(cam.number_band(), 1);
  BOOST_CHECK_EQUAL(cam.number_line(0), 1);
  BOOST_CHECK_EQUAL(cam.number_sample(0), 2048);
  BOOST_CHECK_CLOSE(cam.focal_length(), focal_length * 1e-3, 1e-4);
  BOOST_CHECK_EQUAL(cam.sensor_type(), "S");
  //int step = 1;
  int test_step = 20;
  for(int i = 0; i < cam.number_line(0); i += test_step) 
    for(int j = 0; j < cam.number_sample(0); j += test_step) {
      if(false)
	if(j == 0 && i % 10 == 0)
	  std::cerr << "Starting line " << i << "\n";
      FrameCoordinate fc(i,j);
      BOOST_CHECK(fabs(rcam.frame_coordinate(cam.sc_look_vector(fc, 0),0).line
		     - fc.line) < 1e-3);
      BOOST_CHECK(fabs(rcam.frame_coordinate(cam.sc_look_vector(fc, 0),0).sample
		     - fc.sample) < 1e-3);
      BOOST_CHECK(fabs(cam.frame_coordinate(rcam.sc_look_vector(fc, 0),0).line
		     - fc.line) < 1e-3);
      BOOST_CHECK(fabs(cam.frame_coordinate(rcam.sc_look_vector(fc, 0),0).sample
		     - fc.sample) < 1e-3);
      FrameCoordinateWithDerivative fc2(AutoDerivative<double>(i, 0, 2),
					AutoDerivative<double>(j, 1, 2));
      BOOST_CHECK(fabs(rcam.frame_coordinate_with_derivative(cam.sc_look_vector_with_derivative(fc2, 0),0).line.value()
		       - fc2.line.value()) < 1e-3);
      BOOST_CHECK(fabs(rcam.frame_coordinate_with_derivative(cam.sc_look_vector_with_derivative(fc2, 0),0).sample.value()
		       - fc2.sample.value()) < 1e-3);
      BOOST_CHECK(fabs(cam.frame_coordinate_with_derivative(rcam.sc_look_vector_with_derivative(fc2, 0),0).line.value()
		       - fc2.line.value()) < 1e-3);
      BOOST_CHECK(fabs(cam.frame_coordinate_with_derivative(rcam.sc_look_vector_with_derivative(fc2, 0),0).sample.value()
		       - fc2.sample.value()) < 1e-3);
    }
  double max_line_diff, max_sample_diff;
  cam.compare_camera(rcam, max_line_diff, max_sample_diff);
  BOOST_CHECK(max_line_diff < 1e-2);
  BOOST_CHECK(max_sample_diff < 1e-2);
}

BOOST_AUTO_TEST_CASE(glas_test2)
{
  // This was a problem case we investigated. This has been fixed (we
  // were assuming line_is_y, which isn't the case this camera), but
  // leave test stubbed out here.
  return;
  boost::shared_ptr<CameraRadialDistortion> cam =
    serialize_read<CameraRadialDistortion>("/bigdata/smyth/MiplMarsTest/test_cam.xml");
  GlasGfmCamera gcam(*cam, 0, 100);
  double max_line_diff, max_sample_diff;
  gcam.compare_camera(*cam, max_line_diff, max_sample_diff);
  std::cerr << "max_line_diff:   " << max_line_diff << "\n"
	    << "max_sample_diff: " << max_sample_diff << "\n";
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
