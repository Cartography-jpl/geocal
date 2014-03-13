#include "unit_test_support.h"
#include "ipi.h"
#include "eci.h"
#include "geocal_time.h"
#include "geodetic.h"
#include "wgs84_constant.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ipi, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + 10000 * 40.8e-3;
  boost::shared_ptr<Orbit> orb(new KeplerOrbit);
  boost::shared_ptr<Camera> cam(new SimpleCamera);
  boost::shared_ptr<TimeTable> tt(new ConstantSpacingTimeTable(tmin, tmax));
  int band = 0;
  Ipi ipi(orb, cam, band, tmin, tmax, tt);

  Time texpect = tmin + 1000 * 40.8e-3;
  FrameCoordinate fc(0, 30);
  CartesianInertialLookVector lv = 
    orb->ci_look_vector(texpect, cam->sc_look_vector(fc, band));
  boost::shared_ptr<CartesianFixed> pt = 
    orb->position_ci(texpect)->reference_surface_intersect_approximate(lv)->
    convert_to_cf(texpect);
  Time tres;
  FrameCoordinate fres;
  bool success;
  ipi.time(*pt, tres, fres, success);
  BOOST_CHECK(success);
  BOOST_CHECK(fabs(tres - texpect) < 1.0 / 16 * 40.8e-3);
  BOOST_CHECK(fabs(fres.line - fc.line) < 1.0 / 16);
  BOOST_CHECK(fabs(fres.sample - fc.sample) < 1.0 / 16);
  ImageCoordinate ic;
  ImageCoordinate ic_expect = tt->image_coordinate(texpect, fc);
  ipi.image_coordinate(*pt, ic, success);
  BOOST_CHECK(success);
  BOOST_CHECK_EQUAL(ic, ic_expect);

  fc.sample = -30;
  lv = orb->ci_look_vector(texpect, cam->sc_look_vector(fc, band));
  pt = orb->position_ci(texpect)->reference_surface_intersect_approximate(lv)->
    convert_to_cf(texpect);
  ipi.time(*pt, tres, fres, success);
  BOOST_CHECK(success);
  BOOST_CHECK(fabs(tres - texpect) < 1.0 / 16 * 40.8e-3);
  BOOST_CHECK(fabs(fres.line - fc.line) < 1.0 / 16);
  BOOST_CHECK(fabs(fres.sample - fc.sample) < 1.0 / 16);
  ipi.image_coordinate(*pt, ic, success);
  BOOST_CHECK(!success);
  ipi.image_coordinate_extended(*pt, ic, success);
  ic_expect = tt->image_coordinate(texpect, fc);
  BOOST_CHECK(success);
  BOOST_CHECK_EQUAL(ic, ic_expect);
}

BOOST_AUTO_TEST_SUITE_END()
