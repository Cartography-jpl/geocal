#include "unit_test_support.h"
#include "camera_radial_distortion.h"
#include "constant.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(camera_radial_distortion, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  // This is MRO CTX camera, just as a reasonable camera to use.
  blitz::Array<double, 1> k_radial(3);
  k_radial = -0.00734339259200545000000, 0.00002837587863624170000,
    0.00000001284198912402710;
  // SPICE OD_K uses opposite sign convention
  k_radial *= -1;
  double focal_length = 352.9271664;
  double pitch = 7e-3;
  int left_masked = 38;
  CameraRadialDistortion cam(boost::math::quaternion<double>(1,0,0,0),
			     k_radial,
			     1, 5000, pitch, pitch, focal_length,
			     FrameCoordinate(0.430442527 - 0.5,
					     2543.46099 - 0.5 +left_masked));
  QuaternionCamera cam_no_distort(boost::math::quaternion<double>(1,0,0,0),
				  1, 5000, pitch, pitch, focal_length,
				  FrameCoordinate(0.430442527 - 0.5,
					     2543.46099 - 0.5 +left_masked));
  // Location of undistorted pixel (0,0) in distorted camera
  //std::cerr << cam.frame_coordinate(cam_no_distort.sc_look_vector(FrameCoordinate(0,0),0),0) << "\n";
  // Location of undistorted pixel (0,5000) in distorted camera
  //std::cerr << cam.frame_coordinate(cam_no_distort.sc_look_vector(FrameCoordinate(0,5000),0),0) << "\n";
  ScLookVector slv = cam.sc_look_vector(FrameCoordinate(0.5, 1), 0);
  ScLookVector slv2 = cam.sc_look_vector(FrameCoordinate(0.5, 5000), 0);
  FrameCoordinate fc = cam.frame_coordinate(slv, 0);
  FrameCoordinate fc2 = cam.frame_coordinate(slv2, 0);
  BOOST_CHECK_CLOSE(fc.line, 0.5, 1e-2);
  BOOST_CHECK(fabs(fc.sample - 1.0) < 0.01);
  BOOST_CHECK_CLOSE(fc2.line, 0.5, 1e-2);
  BOOST_CHECK_CLOSE(fc2.sample, 5000.0, 1e-2);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  blitz::Array<double, 1> k_radial(3);
  k_radial = -0.00734339259200545000000, 0.00002837587863624170000,
    0.00000001284198912402710;
  double focal_length = 352.9271664;
  double pitch = 7e-3;
  int left_masked = 38;
  boost::shared_ptr<Camera> cam =
    boost::make_shared<CameraRadialDistortion>
    (boost::math::quaternion<double>(1,0,0,0),
     k_radial,
     1, 5000, pitch, pitch, focal_length,
     FrameCoordinate(0.430442527 - 0.5,
		     2543.46099 - 0.5 +left_masked));
  std::string d = serialize_write_string(cam);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Camera> camr = serialize_read_string<Camera>(d);
  BOOST_CHECK_EQUAL(cam->number_band(), camr->number_band());
  for(int b = 0; b < cam->number_band(); ++b) {
    BOOST_CHECK_EQUAL(cam->number_sample(b), camr->number_sample(b));
    for(double ln = 0; ln < cam->number_line(b); ln += 10) {
      for(int smp = 0; smp < cam->number_sample(b); smp += 10) {
	FrameCoordinate fc(ln, smp);
	FrameCoordinate fc2 = 
	  camr->frame_coordinate(cam->sc_look_vector(fc, b), b);
	BOOST_CHECK(fabs(fc.line - fc2.line) < 0.1);
	BOOST_CHECK(fabs(fc.sample - fc2.sample) < 0.1);
	FrameCoordinate fc3 = 
	  cam->frame_coordinate(camr->sc_look_vector(fc, b), b);
	BOOST_CHECK(fabs(fc.line - fc3.line) < 0.1);
	BOOST_CHECK(fabs(fc.sample - fc3.sample) < 0.1);
      }
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
