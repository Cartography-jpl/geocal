#include "unit_test_support.h"
#include "quickbird_camera.h"
using namespace GeoCal;

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
  if(!have_serialize_supported())
    return;

  boost::shared_ptr<Camera> cam(new QuickBirdCamera());
  FrameCoordinate f1(1, 2);
  ScLookVector sl = cam->sc_look_vector(f1, 0);
  std::string d = serialize_write_string(cam);
  if(false)
    std::cerr << d;
  boost::shared_ptr<QuickBirdCamera> camr =
    serialize_read_string<QuickBirdCamera>(d);
  FrameCoordinate f2 = camr->frame_coordinate(sl, 0);
  FrameCoordinate f3 = cam->frame_coordinate(sl, 0);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
  BOOST_CHECK_EQUAL(camr->number_line(0), 1);
  BOOST_CHECK_EQUAL(camr->number_sample(0), 27552);
  BOOST_CHECK_EQUAL(camr->number_band(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
