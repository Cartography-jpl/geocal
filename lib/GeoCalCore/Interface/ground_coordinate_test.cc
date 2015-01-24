#include "unit_test_support.h"
#include "ecr.h"
#include "eci.h"
#include "geocal_time.h"
#include "geodetic.h"
#include "wgs84_constant.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ground_coordinate, GlobalFixture)

BOOST_AUTO_TEST_CASE(eci)
{
  boost::array<double, 3> p = {{10, 20, 30}};
  Eci e1(p);
  Eci e2(10, 20, 30);
  BOOST_CHECK(e1.position == p);
  BOOST_CHECK(e2.position == p);

  Geodetic g(10, 20, 10000);
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  boost::shared_ptr<CartesianInertial> e3 = 
    g.convert_to_cf()->convert_to_ci(t);
  boost::array<double, 3> d = {{10.0, 11.0, 12.0}};
  CartesianInertialLookVector lv(d);
  boost::shared_ptr<CartesianInertial> e4 =
    e3->reference_surface_intersect_approximate(lv, 100);

// Check that surface point is in the direction of the lv.

  d[0] = e4->position[0] - e3->position[0];
  d[1] = e4->position[1] - e3->position[1];
  d[2] = e4->position[2] - e3->position[2];
  CartesianInertialLookVector lv2(d);
  BOOST_CHECK_CLOSE(lv2.direction()[0], lv.direction()[0], 1e-2);
  BOOST_CHECK_CLOSE(lv2.direction()[1], lv.direction()[1], 1e-2);
  BOOST_CHECK_CLOSE(lv2.direction()[2], lv.direction()[2], 1e-2);

// Check that height is near desired height.

  BOOST_CHECK(fabs(e4->convert_to_cf(t)->height_reference_surface() - 100) < 1);
}

BOOST_AUTO_TEST_CASE(ecr)
{
  boost::array<double, 3> p = {{10, 20, 30}};
  Ecr e1(p);
  Ecr e2(10, 20, 30);
  BOOST_CHECK(e1.position == p);
  BOOST_CHECK(e2.position == p);
  Geodetic g(10, 20, 50);
  Ecr e3(g);
  BOOST_CHECK_CLOSE(e3.latitude(), 10.0, 1e-4);
  BOOST_CHECK_CLOSE(e3.longitude(), 20.0, 1e-4);
  BOOST_CHECK_CLOSE(e3.height_reference_surface(), 50.0, 1e-4);
  BOOST_CHECK_CLOSE(e3.min_radius_reference_surface(), 
		    Constant::wgs84_a, 1e-4);
  boost::shared_ptr<CartesianFixed> cf = e3.convert_to_cf();
  BOOST_CHECK(dynamic_cast<Ecr*>(cf.get()) != 0);
  BOOST_CHECK(e3.position == cf->position);
  BOOST_CHECK_EQUAL(e1.print_to_string(), "Ecr: (10 m, 20 m, 30 m)");

  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  boost::shared_ptr<CartesianFixed> e_ecr_start = 
    Geodetic(10, 20, 30).convert_to_cf();
  boost::shared_ptr<CartesianInertial> e_eci = e_ecr_start->convert_to_ci(t);
  BOOST_CHECK_CLOSE(e_eci->position[0], 888551.27707691176, 1e-4);
  BOOST_CHECK_CLOSE(e_eci->position[1], -6218769.7591331499, 1e-4);
  BOOST_CHECK_CLOSE(e_eci->position[2], 1100106.8473076127, 1e-4);
  boost::shared_ptr<CartesianFixed> e_ecr = e_eci->convert_to_cf(t);
  BOOST_CHECK_CLOSE(e_ecr->position[0], e_ecr_start->position[0], 1e-4);
  BOOST_CHECK_CLOSE(e_ecr->position[1], e_ecr_start->position[1], 1e-4);
  BOOST_CHECK_CLOSE(e_ecr->position[2], e_ecr_start->position[2], 1e-4);
}

BOOST_AUTO_TEST_CASE(solar_angle)
{
  Time t = Time::parse_time("1996-07-03T04:13:57.987654Z") + 10;
  CartesianFixedLookVector lv(Ecr::sub_solar_point(t).position);
  Geodetic p(50, 60, 1);
  LnLookVector ln(lv, p);
  // The results are from an old unit test in MSPI code. We don't
  // expect this to exactly match because the MSPI code used the SDP
  // toolkit. But should be close.
  BOOST_CHECK_CLOSE(ln.view_zenith(), 9.06966388e-01 * Constant::rad_to_deg, 
		    0.1);
  BOOST_CHECK_CLOSE(ln.view_azimuth(), 4.87556601e+00 * Constant::rad_to_deg, 
		    0.1);
}

BOOST_AUTO_TEST_CASE(solar_distance)
{
  Time t = Time::parse_time("2010-10-07T19:11:24.500000Z");
  BOOST_CHECK_CLOSE(Ecr::solar_distance(t), 0.99938745098726067, 1e-4);
}

BOOST_AUTO_TEST_CASE(geodetic)
{
  Geodetic g(30, 40, 50);
  BOOST_CHECK_CLOSE(g.latitude(), 30.0, 1e-4);
  BOOST_CHECK_CLOSE(g.longitude(), 40.0, 1e-4);
  BOOST_CHECK_CLOSE(g.height_reference_surface(), 50.0, 1e-4);
  Ecr g_ecr(g);
  Geodetic g2(g_ecr);
  BOOST_CHECK(g_ecr.position == g.convert_to_cf()->position);
  BOOST_CHECK_CLOSE(g2.latitude(), 30.0, 1e-4);
  BOOST_CHECK_CLOSE(g2.longitude(), 40.0, 1e-4);
  BOOST_CHECK_CLOSE(g2.height_reference_surface(), 50.0, 1e-4);
  BOOST_CHECK(distance(g, g2) < 1e-4);
  BOOST_CHECK_EQUAL(g.print_to_string(), "Geodetic: (30 deg, 40 deg, 50 m)");
}

BOOST_AUTO_TEST_CASE(geocentric)
{
  Geocentric g(30, 40, 50);
  BOOST_CHECK_CLOSE(g.latitude(), 30.0, 1e-4);
  BOOST_CHECK_CLOSE(g.longitude(), 40.0, 1e-4);
  BOOST_CHECK_CLOSE(g.height_reference_surface(), 50.0, 1e-4);
  Ecr g_ecr(g);
  Geocentric g2(g_ecr);
  BOOST_CHECK(g_ecr.position == g.convert_to_cf()->position);
  BOOST_CHECK_CLOSE(g2.latitude(), 30.0, 1e-4);
  BOOST_CHECK_CLOSE(g2.longitude(), 40.0, 1e-4);
  BOOST_CHECK_CLOSE(g2.height_reference_surface(), 50.0, 1e-4);
  BOOST_CHECK(distance(g, g2) < 1e-4);
  BOOST_CHECK_EQUAL(g.print_to_string(), "Geocentric: (30 deg, 40 deg, 50 m)");
}

BOOST_AUTO_TEST_CASE(pos_and_vel)
{
  Geodetic g(10, 20, 30);
  Ecr e(g);
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  boost::array<double, 3> vel_cf = {{100, 200, 300}};
  boost::array<double, 3> vel_ci;
  boost::shared_ptr<CartesianInertial> ci;
  convert_position_and_velocity(t, e, vel_cf, ci, vel_ci);
  BOOST_CHECK_CLOSE(ci->position[0], 888551.27707691176, 1e-4);
  BOOST_CHECK_CLOSE(ci->position[1], -6218769.7591331499, 1e-4);
  BOOST_CHECK_CLOSE(ci->position[2], 1100106.8473076127, 1e-4);
  BOOST_CHECK_CLOSE(vel_ci[0], 628.71130860798417, 1e-4);
  BOOST_CHECK_CLOSE(vel_ci[1], -74.22614700738886, 1e-4);
  BOOST_CHECK_CLOSE(vel_ci[2], 299.83611083221604, 1e-4);
  boost::shared_ptr<CartesianFixed> cf;
  convert_position_and_velocity(t, *ci, vel_ci, cf, vel_cf);
  BOOST_CHECK(distance(e, *cf) < 1e-4);
  BOOST_CHECK_CLOSE(vel_cf[0], 100, 1e-4);
  BOOST_CHECK_CLOSE(vel_cf[1], 200, 1e-4);
  BOOST_CHECK_CLOSE(vel_cf[2], 300, 1e-4);
  // Sanity check, the earth equator rotation speed according to
  // wikipedia 465.1 m/s.
  Ecr e2(Geodetic(0, 0));
  vel_cf[0] = 0;
  vel_cf[1] = 0;
  vel_cf[2] = 0;
  convert_position_and_velocity(t, e, vel_cf, ci, vel_ci);
  double speed = sqrt(vel_ci[0] * vel_ci[0] + vel_ci[1] * vel_ci[1] + 
		      vel_ci[2] * vel_ci[2]);
  BOOST_CHECK(fabs(speed - 465.1) < 10.0);
}

BOOST_AUTO_TEST_CASE(pos_and_vel_with_der)
{
  Geodetic g(10, 20, 30);
  Ecr cf(g);
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  TimeWithDerivative tder = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 7));
  boost::array<AutoDerivative<double>, 3> cf_der = 
    {{AutoDerivative<double>(cf.position[0], 1, 7),
     AutoDerivative<double>(cf.position[1], 2, 7),
      AutoDerivative<double>(cf.position[2], 3, 7)}};
  boost::array<AutoDerivative<double>, 3> vel_cf = 
    {{AutoDerivative<double>(100.0, 4, 7), 
      AutoDerivative<double>(200.0, 5, 7),
      AutoDerivative<double>(300, 6, 7)}};
  boost::array<AutoDerivative<double>, 3> vel_ci, ci_der;
  boost::shared_ptr<CartesianInertial> ci;
  convert_position_and_velocity(tder, cf, cf_der, vel_cf, ci, ci_der, vel_ci);
  BOOST_CHECK_CLOSE(ci->position[0], 888551.27707691176, 1e-4);
  BOOST_CHECK_CLOSE(ci->position[1], -6218769.7591331499, 1e-4);
  BOOST_CHECK_CLOSE(ci->position[2], 1100106.8473076127, 1e-4);
  BOOST_CHECK_CLOSE(vel_ci[0].value(), 628.71130860798417, 1e-4);
  BOOST_CHECK_CLOSE(vel_ci[1].value(), -74.22614700738886, 1e-4);
  BOOST_CHECK_CLOSE(vel_ci[2].value(), 299.83611083221604, 1e-4);
  boost::shared_ptr<CartesianFixed> cf2;
  convert_position_and_velocity(tder, *ci, ci_der, vel_ci, cf2, cf_der, vel_cf);
  BOOST_CHECK(distance(g, *cf2) < 1e-4);
  BOOST_CHECK_CLOSE(vel_cf[0].value(), 100, 1e-4);
  BOOST_CHECK_CLOSE(vel_cf[1].value(), 200, 1e-4);
  BOOST_CHECK_CLOSE(vel_cf[2].value(), 300, 1e-4);
}

BOOST_AUTO_TEST_CASE(pos_and_vel_with_der_check_grad)
{
  Geodetic g(10, 20, 30);
  Ecr cf(g);
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  TimeWithDerivative tder = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 7));
  boost::array<AutoDerivative<double>, 3> cf_der = 
    {{AutoDerivative<double>(cf.position[0], 1, 7),
     AutoDerivative<double>(cf.position[1], 2, 7),
      AutoDerivative<double>(cf.position[2], 3, 7)}};
  boost::array<AutoDerivative<double>, 3> vel_cf = 
    {{AutoDerivative<double>(100.0, 4, 7), 
      AutoDerivative<double>(200.0, 5, 7),
      AutoDerivative<double>(300, 6, 7)}};
  boost::array<AutoDerivative<double>, 3> vel_ci, ci_der;
  boost::shared_ptr<CartesianInertial> ci;
  convert_position_and_velocity(tder, cf, cf_der, vel_cf, ci, ci_der, vel_ci);

  boost::array<double, 3> vel_cf2 = {{100, 200, 300}};
  boost::array<double, 3> vel_ci2;
  boost::shared_ptr<CartesianInertial> cieps;
  double eps = 1e-3;
  blitz::Array<double, 2> jac_fd(3, 7);
  blitz::Array<double, 2> jac2_fd(3, 7);
  blitz::Array<double, 2> jac_calc(3, 7);
  blitz::Array<double, 2> jac2_calc(3, 7);
  blitz::Range rall(blitz::Range::all());
  for(int i = 0; i < 3; ++i) {
    jac_calc(i, rall) = ci_der[i].gradient();
    jac2_calc(i, rall) = vel_ci[i].gradient();
  }
  convert_position_and_velocity(t + eps, cf, vel_cf2, cieps, vel_ci2);
  for(int i = 0; i < 3; ++i) {
    jac_fd(i, 0) = (cieps->position[i] - ci->position[i]) / eps;
    jac2_fd(i, 0) = (vel_ci2[i] - vel_ci[i].value()) /eps;
  }
  eps = 10.0;
  for(int j = 0; j < 3; ++j) {
    double old = cf.position[j];
    cf.position[j] += eps;
    convert_position_and_velocity(t, cf, vel_cf2, cieps, vel_ci2);
    cf.position[j] = old;
    for(int i = 0; i < 3; ++i) {
      jac_fd(i, j + 1) = (cieps->position[i] - ci->position[i]) / eps;
      jac2_fd(i, j + 1) = (vel_ci2[i] - vel_ci[i].value()) /eps;
    }
  }
  for(int j = 0; j < 3; ++j) {
    double old = vel_cf2[j];
    vel_cf2[j] += eps;
    convert_position_and_velocity(t, cf, vel_cf2, cieps, vel_ci2);
    vel_cf2[j] = old;
    for(int i = 0; i < 3; ++i) {
      jac_fd(i, j + 4) = (cieps->position[i] - ci->position[i]) / eps;
      jac2_fd(i, j + 4) = (vel_ci2[i] - vel_ci[i].value()) /eps;
    }
  }
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac2_fd, jac2_calc, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
