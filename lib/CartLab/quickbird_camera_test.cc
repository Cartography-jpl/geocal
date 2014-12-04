#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "geocal_serialize_common.h"
#include "unit_test_support.h"
#include "quickbird_camera.h"
using namespace GeoCal;
BOOST_CLASS_EXPORT(GeoCal::Camera);
BOOST_CLASS_EXPORT(GeoCal::QuaternionCamera);
BOOST_CLASS_EXPORT(GeoCal::QuickBirdCamera);

BOOST_FIXTURE_TEST_SUITE(quickbird_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  QuickBirdCamera c;
  FrameCoordinate f1(1, 2);
  ScLookVector sl = c.sc_look_vector(f1, 0);
  FrameCoordinate f2 = c.frame_coordinate(sl, 0);
  BOOST_CHECK_EQUAL(c.number_band(), 1);
  BOOST_CHECK_EQUAL(c.number_line(0), 1);
  BOOST_CHECK_EQUAL(c.number_sample(0), 27552);
  BOOST_CHECK_CLOSE(sl.direction()[0], -0.00831307, 1e-4);
  BOOST_CHECK_CLOSE(sl.direction()[1], 0.0260168, 1e-4);
  BOOST_CHECK_CLOSE(sl.direction()[2], 0.999627, 1e-4);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
  BOOST_CHECK_CLOSE(c.frame_line_coordinate(sl, 0), f1.line, 1e-4);
  sl = c.sc_look_vector(FrameCoordinate(-0.5, c.number_sample(0)), 0);
  BOOST_CHECK_CLOSE(sl.direction()[0], -0.00818884, 1e-4);
  BOOST_CHECK_CLOSE(sl.direction()[1], -0.01112154, 1e-4);
  BOOST_CHECK_CLOSE(sl.direction()[2], 0.999905, 1e-4);
  sl = c.sc_look_vector(FrameCoordinate(+0.5, c.number_sample(0)), 0);
  BOOST_CHECK_CLOSE(sl.direction()[0], -0.00818749, 1e-4);
  BOOST_CHECK_CLOSE(sl.direction()[1], -0.01112154, 1e-4);
  BOOST_CHECK_CLOSE(sl.direction()[2], 0.999905, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  boost::shared_ptr<Camera> cam(new QuickBirdCamera());
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
  boost::shared_ptr<QuickBirdCamera> camr2 = 
    boost::dynamic_pointer_cast<QuickBirdCamera>(camr);
  
  BOOST_CHECK_EQUAL(camr2->number_line(0), 1);
  BOOST_CHECK_EQUAL(camr2->number_sample(0), 27552);
  BOOST_CHECK_EQUAL(camr2->number_band(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
