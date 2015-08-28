#include "unit_test_support.h"
#include "quaternion_camera.h"
#include "ipi.h"
#include "eci.h"
#include "geocal_time.h"
#include "geodetic.h"
#include "wgs84_constant.h"
#include <cmath>

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(ipi, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + 2000 * 40.8e-3;
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

BOOST_AUTO_TEST_CASE(jac_test)
{
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + 2000 * 40.8e-3;
  boost::shared_ptr<Orbit> orb(new KeplerOrbit);
  SimpleCamera cam1;
  boost::shared_ptr<QuaternionCamera> cam
    ( new QuaternionCamera(quat_rot_z(cam1.epsilon()) *
			   quat_rot_y(cam1.beta()) *
			   quat_rot_x(cam1.delta()),
			   cam1.number_line(0), cam1.number_sample(0), 
			   cam1.line_pitch() / 1e-3, 
			   cam1.sample_pitch() / 1e-3,
			   cam1.focal_length() / 1e-3,
			   FrameCoordinate(cam1.number_line(0) / 2.0, 
					   cam1.number_sample(0) / 2.0)));
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

  // Now test time_with_derivative.
  cam->fit_epsilon(true);
  cam->fit_beta(true);
  cam->fit_delta(true);
  cam->fit_line_pitch(false);
  cam->fit_sample_pitch(false);
  cam->fit_focal_length(false);
  cam->fit_principal_point_line(false);
  cam->fit_principal_point_sample(false);
  cam->add_identity_gradient();

  TimeWithDerivative tres_wd;
  FrameCoordinateWithDerivative fres_wd;
  ipi.time_with_derivative(*pt, tres_wd, fres_wd, success);
  BOOST_CHECK(success);
  BOOST_CHECK(fabs(tres_wd.value() - texpect) < 1.0 / 16 * 40.8e-3);
  BOOST_CHECK(fabs(fres_wd.line.value() - fc.line) < 1.0 / 16);
  BOOST_CHECK(fabs(fres_wd.sample.value() - fc.sample) < 1.0 / 16);
  ImageCoordinateWithDerivative ic_wd;
  ipi.image_coordinate_with_derivative(*pt, ic_wd, success);
  BOOST_CHECK(success);
  BOOST_CHECK_EQUAL(ic_wd.value(), ic_expect);
  Array<double, 2> jac_calc(3, 3);
  Array<double, 2> jac_calc2(2, 3);
  jac_calc(0, Range::all()) = tres_wd.gradient();
  jac_calc(1, Range::all()) = fres_wd.line.gradient();
  jac_calc(2, Range::all()) = fres_wd.sample.gradient();
  jac_calc2(0, Range::all()) = ic_wd.line.gradient();
  jac_calc2(1, Range::all()) = ic_wd.sample.gradient();
  Array<double, 2> jac_fd(3, 3);
  Array<double, 2> jac_fd2(2, 3);
  Array<double, 1> eps(3);
  eps = 1e-3;
  Array<double, 1> p0 = cam->parameter();
  for(int i = 0; i < 3; ++i) {
    Array<double, 1> p(p0.copy());
    p(i) += eps(i);
    cam->parameter(p);
    Time tres2;
    FrameCoordinate fres2;
    ipi.time(*pt, tres2, fres2, success);
    jac_fd(0, i) = (tres2 - tres) / eps(i);
    jac_fd(1, i) = (fres2.line - fres.line) / eps(i);
    jac_fd(2, i) = (fres2.sample - fres.sample) / eps(i);
    ImageCoordinate ic2;
    ipi.image_coordinate(*pt, ic2, success);
    jac_fd2(0, i) = (ic2.line - ic.line) / eps(i);
    jac_fd2(1, i) = (ic2.sample - ic.sample) / eps(i);
  }
  // Fairly large disagreement, but results seem to be about 1% of
  // finite difference, which is acceptable.
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_calc(0,Range::all()), 
			       jac_fd(0,Range::all()), 3);
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_calc(2,Range::all()), 
			       jac_fd(2,Range::all()), 80);
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_calc2, jac_fd2, 100);
}

BOOST_AUTO_TEST_SUITE_END()
