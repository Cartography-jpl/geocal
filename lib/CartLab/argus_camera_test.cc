#include "unit_test_support.h"
#include "argus_camera.h"
#include "ecr.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(argus_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
// Not actually used anymore, we hardwire the line and sample pitch.
//   std::string fname = std::string(getenv("srcdir")) + 
//     "/test_data/argus_camera6.jpg";
  ArgusCamera cam(10);
  BOOST_CHECK_CLOSE(cam.focal_length(), 400.0, 1e-4);
  BOOST_CHECK_CLOSE(cam.line_pitch(),   0.00641026, 1e-4);
  BOOST_CHECK_CLOSE(cam.sample_pitch(), 0.00641026, 1e-4);
  FrameCoordinate fc1(cam.number_line(0) / 2.0, cam.number_sample(0) / 2.0);
  BOOST_CHECK_CLOSE(fc1.line, cam.frame_coordinate(cam.sc_look_vector(fc1, 0), 
						   0).line, 1e-4);
  BOOST_CHECK_CLOSE(fc1.sample, 
		    cam.frame_coordinate(cam.sc_look_vector(fc1, 0), 
					 0).sample, 1e-4);
  ScLookVector slv = cam.sc_look_vector(fc1, 0);
  BOOST_CHECK_CLOSE(slv.direction()[0], 0.0,1e-4);
  BOOST_CHECK_CLOSE(slv.direction()[1], 0.0,1e-4);
  BOOST_CHECK_CLOSE(slv.direction()[2], 1.0,1e-4);
  FrameCoordinate fc2(1, 1);
  BOOST_CHECK_CLOSE(fc2.line, cam.frame_coordinate(cam.sc_look_vector(fc2, 0), 
						   0).line, 1e-4);
  BOOST_CHECK_CLOSE(fc2.sample, 
		    cam.frame_coordinate(cam.sc_look_vector(fc2, 0), 
					 0).sample, 1e-4);
  slv = cam.sc_look_vector(fc2, 0);
  BOOST_CHECK_CLOSE(slv.direction()[0], 0.0372854, 1e-4);
  BOOST_CHECK_CLOSE(slv.direction()[1], -0.0390369, 1e-4);
  BOOST_CHECK_CLOSE(slv.direction()[2], 0.998542, 1e-4);
}


BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_BOOST_SERIALIZATON
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  boost::shared_ptr<Camera> cam(new ArgusCamera(10));
  FrameCoordinate f1(1, 2);
  ScLookVector sl = cam->sc_look_vector(f1, 0);
  oa << GEOCAL_NVP(cam);
  if(false)
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<Camera> camr;
  ia >> GEOCAL_NVP(camr);
  FrameCoordinate f2 = camr->frame_coordinate(sl, 0);
  FrameCoordinate f3 = cam->frame_coordinate(sl, 0);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
  boost::shared_ptr<ArgusCamera> camr2 = 
    boost::dynamic_pointer_cast<ArgusCamera>(camr);
  
  BOOST_CHECK_CLOSE(camr2->focal_length(), 400.0, 1e-4);
  BOOST_CHECK_CLOSE(camr2->line_pitch(),   0.00641026, 1e-4);
  BOOST_CHECK_CLOSE(camr2->sample_pitch(), 0.00641026, 1e-4);
  BOOST_CHECK_EQUAL(camr2->number_band(), 1);
#endif
}

BOOST_AUTO_TEST_SUITE_END()

