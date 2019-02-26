#include "unit_test_support.h"
#include "sensrb_camera.h"
#include <iostream>
#include <boost/make_shared.hpp>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(sensrb_camera, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  // Dummy Test
  SensrbCamera cam(boost::math::quaternion<double>(1,0,0,0),
		   1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0,
		   2048, 1024, 18e-6, 21e-6,
		   123.8e-3, FrameCoordinate(2048/2, 1024/2));
  //std::cerr << cam << "\n";
  
  // blitz::Array<double, 2> kappa(3,3);
  // kappa = 1, 0, 0,
  //         0, 1, 0,
  //         0, 0, 1;
  // blitz::Array<double, 2> kappa_inv(kappa);
  
  // CameraRationalPolyomial cam(2048,1024,2e-06,(2048-1)/2.0,(1024-1)/2.0,
  // 			      boost::math::quaternion<double>(1,0,0,0),
  // 			      kappa, kappa_inv);
  // // These are values we got from the matlab code (see
  // // compare_burl_matlab_test.py in the python code)
  // ScLookVector slv = cam.sc_look_vector(FrameCoordinate(10, 20), 0);
  // ScLookVector slv2 = cam.sc_look_vector(FrameCoordinate(1000, 50), 0);
  // BOOST_CHECK_CLOSE(slv.direction()[0], -0.00200699, 1e-3);
  // BOOST_CHECK_CLOSE(slv.direction()[1], -0.001003, 1e-3);
  // BOOST_CHECK_CLOSE(slv.direction()[2], 0.999997, 1e-3);
  // BOOST_CHECK_CLOSE(slv2.direction()[0], -0.001947, 1e-3);
  // BOOST_CHECK_CLOSE(slv2.direction()[1], 0.000976998, 1e-3);
  // BOOST_CHECK_CLOSE(slv2.direction()[2], 0.999998, 1e-3);
  // FrameCoordinate fc = cam.frame_coordinate(slv, 0);
  // FrameCoordinate fc2 = cam.frame_coordinate(slv2, 0);
  // BOOST_CHECK_CLOSE(fc.line, 10.0, 1e-3);
  // BOOST_CHECK_CLOSE(fc.sample, 20.0, 1e-3);
  // BOOST_CHECK_CLOSE(fc2.line, 1000.0, 1e-3);
  // BOOST_CHECK_CLOSE(fc2.sample, 50.0, 1e-3);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Camera> cam(new SensrbCamera
		(boost::math::quaternion<double>(1,0,0,0),
		 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0,
		 2048, 1024, 18e-6, 21e-6,
		 123.8e-3, FrameCoordinate(2048/2, 1024/2)));
  std::string d = serialize_write_string(cam);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Camera> camr = serialize_read_string<Camera>(d);
  // BOOST_CHECK_EQUAL(cam->number_band(), camr->number_band());
  // for(int b = 0; b < cam->number_band(); ++b) {
  //   BOOST_CHECK_EQUAL(cam->number_sample(b), camr->number_sample(b));
  //   for(double ln = 0; ln < cam->number_line(b); ln += 10) {
  //     for(int smp = 0; smp < cam->number_sample(b); smp += 10) {
  // 	FrameCoordinate fc(ln, smp);
  // 	FrameCoordinate fc2 = 
  // 	  camr->frame_coordinate(cam->sc_look_vector(fc, b), b);
  // 	BOOST_CHECK(fabs(fc.line - fc2.line) < 0.1);
  // 	BOOST_CHECK(fabs(fc.sample - fc2.sample) < 0.1);
  // 	FrameCoordinate fc3 = 
  // 	  cam->frame_coordinate(camr->sc_look_vector(fc, b), b);
  // 	BOOST_CHECK(fabs(fc.line - fc3.line) < 0.1);
  // 	BOOST_CHECK(fabs(fc.sample - fc3.sample) < 0.1);
  //     }
  //   }
  // }
}

BOOST_AUTO_TEST_SUITE_END()
