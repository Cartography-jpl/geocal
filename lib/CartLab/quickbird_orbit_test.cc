#include "unit_test_support.h"
#include "quickbird_orbit.h"
#include "quickbird_camera.h"
#include "ipi.h"
#include "geodetic.h"
#include <iostream>
#include <cmath>
using namespace GeoCal;
using namespace blitz;

class QuickbirdOrbitFixture : public GlobalFixture {
public:
  QuickbirdOrbitFixture()
  {
    std::string fname = test_data_dir() + 
      "06JAN06033849-P1BS-005545519120_01_P001.EPH";
    orb.reset(new QuickBirdOrbit(fname));
    t = Time::parse_time("2006-01-06T03:38:49.805725Z") + 2.0;
  }
  boost::shared_ptr<Orbit> orb;
  Time t;
};
BOOST_FIXTURE_TEST_SUITE(quickbird_orbit, QuickbirdOrbitFixture)

BOOST_AUTO_TEST_CASE(ephemeris)
{
  std::string fname = test_data_dir() + 
    "06JAN06033849-P1BS-005545519120_01_P001.EPH";
  QuickBirdEphemeris e(fname);
  BOOST_CHECK_EQUAL(e.file_name(), fname);

  // All these expected results come from directly looking at the
  // input file. This is in plain text, so it is easy to read by eye.

  BOOST_CHECK_EQUAL((int) e.ephemeris().size(), 2522);
  BOOST_CHECK_CLOSE(e.time_spacing(), 0.02, 1e-4);
  BOOST_CHECK(fabs(e.min_time() - 
		   Time::parse_time("2006-01-06T03:38:26.790529Z")) < 1e-3);
  BOOST_CHECK(fabs(e.max_time() - 
		   Time::parse_time("2006-01-06T03:39:17.230529Z")) < 1e-3);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][0], -2073444.7387687771000000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][1], 5217942.5204200782000000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][2], 3873585.5859117978000000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][3], -117.3210519302560000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][4], 4580.2789893791914000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][5], -6212.9292350958312000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][6], 0.0013682569363000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][7], -0.0001362282447500, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][8], 0.0003135203847000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][9], 0.0027674336307000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][10], -0.0026094620688000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[0][11], 0.0049859896540000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][0], -2075235.8727155479000000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][1], 5440484.1031867517000000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][2], 3554302.1328906710000000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][3], 45.8803320989507600, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][4], 4244.6941571491679000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][5], -6448.6476850815798000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][6], 0.0013602768244000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][7], -0.0000796491958240, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][8], 0.0002480386974200, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][9], 0.0025714731031000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][10], -0.0025299288638000, 1e-4);
  BOOST_CHECK_CLOSE(e.ephemeris()[2521][11], 0.0052244878478000, 1e-4);

}

BOOST_AUTO_TEST_CASE(attitude)
{
  std::string fname = test_data_dir() + 
    "06JAN06033849-P1BS-005545519120_01_P001.ATT";
  QuickBirdAttitude e(fname);
  BOOST_CHECK_EQUAL(e.file_name(), fname);

  // All these expected results come from directly looking at the
  // input file. This is in plain text, so it is easy to read by eye.

  BOOST_CHECK_EQUAL((int) e.attitude().size(), 2522);
  BOOST_CHECK_CLOSE(e.time_spacing(), 0.02, 1e-4);
  BOOST_CHECK(fabs(e.min_time() - 
		   Time::parse_time("2006-01-06T03:38:26.790529Z")) < 1e-3);
  BOOST_CHECK(fabs(e.max_time() - 
		   Time::parse_time("2006-01-06T03:39:17.230529Z")) < 1e-3);
  BOOST_CHECK_CLOSE(e.attitude()[0][0], -0.3087939999536268, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][1], -0.8376664361461180, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][2], 0.3189198092995629, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][3], -0.3182001926197278, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][4], 0.0000000001320000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][5], -0.0000000000379000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][6], 0.0000000000161000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][7], -0.0000000000146000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][8], 0.0000000000471000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][9], 0.0000000000422000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][10], -0.0000000000384000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][11], 0.0000000001290000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][12], 0.0000000000163000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[0][13], 0.0000000001330000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][0], -0.3423342205519756, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][1], -0.7019587136127043, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][2],  0.5494114237300394, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][3], -0.2969988776025926, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][4],  0.0000000001300000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][5], -0.0000000000359000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][6],  0.0000000000262000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][7], -0.0000000000150000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][8],  0.0000000000710000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][9],  0.0000000000556000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][10],-0.0000000000319000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][11], 0.0000000001060000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][12], 0.0000000000233000, 1e-4);
  BOOST_CHECK_CLOSE(e.attitude()[2521][13], 0.0000000001340000, 1e-4);
}

BOOST_AUTO_TEST_CASE(orbit)
{
  BOOST_CHECK(fabs(orb->min_time() - 
		   Time::parse_time("2006-01-06T03:38:26.790529Z")) < 1e-3);
  boost::shared_ptr<Camera> cam(new QuickBirdCamera);
  Time tstart = Time::parse_time("2006-01-06T03:38:49.805725Z");
  Time tend = tstart + 4.235362;
  double tspace = 4.235362 / 29224;
  boost::shared_ptr<TimeTable> 
    tt(new ConstantSpacingTimeTable(tstart, tend, tspace));
  Ipi ipi(orb, cam, 0, orb->min_time(), orb->max_time(), tt);
  ImageCoordinate ic;
  bool success;
  ipi.image_coordinate(Geodetic(33.8007639, 111.9295833), ic, success);
  BOOST_CHECK(success);
}

BOOST_AUTO_TEST_CASE(derivative_ci)
{
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  boost::shared_ptr<CartesianInertial> p0 = orb->position_ci(t2.value());
  boost::shared_ptr<CartesianInertial> p1 = orb->position_ci(t2.value() + eps);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1->position[0] - p0->position[0]) / eps;
  jac_fd(1,0) = (p1->position[1] - p0->position[1]) / eps;
  jac_fd(2,0) = (p1->position[2] - p0->position[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  boost::array<AutoDerivative<double>, 3> p2 = 
    orb->position_ci_with_derivative(t2);
  jac_calc(0,Range::all()) = p2[0].gradient();
  jac_calc(1,Range::all()) = p2[1].gradient();
  jac_calc(2,Range::all()) = p2[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(derivative_cf)
{
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  boost::shared_ptr<CartesianFixed> p0 = orb->position_cf(t2.value());
  boost::shared_ptr<CartesianFixed> p1 = orb->position_cf(t2.value() + eps);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1->position[0] - p0->position[0]) / eps;
  jac_fd(1,0) = (p1->position[1] - p0->position[1]) / eps;
  jac_fd(2,0) = (p1->position[2] - p0->position[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  boost::array<AutoDerivative<double>, 3> p2 = 
    orb->position_cf_with_derivative(t2);
  jac_calc(0,Range::all()) = p2[0].gradient();
  jac_calc(1,Range::all()) = p2[1].gradient();
  jac_calc(2,Range::all()) = p2[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(derivative_ci_look)
{
  // Skip for now
  return;
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  SimpleCamera cam;
  ScLookVector sl = cam.sc_look_vector(FrameCoordinate(0,0), 0);
  ScLookVectorWithDerivative sl_wd = 
    cam.sc_look_vector_with_derivative(FrameCoordinateWithDerivative(0,0), 0);
  CartesianInertialLookVector p0 = orb->ci_look_vector(t2.value(), sl);
  CartesianInertialLookVector p1 = orb->ci_look_vector(t2.value() + eps, sl);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1.look_vector[0] - p0.look_vector[0]) / eps;
  jac_fd(1,0) = (p1.look_vector[1] - p0.look_vector[1]) / eps;
  jac_fd(2,0) = (p1.look_vector[2] - p0.look_vector[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  CartesianInertialLookVectorWithDerivative p2 = 
    orb->ci_look_vector(t2, sl_wd);
  jac_calc(0,Range::all()) = p2.look_vector[0].gradient();
  jac_calc(1,Range::all()) = p2.look_vector[1].gradient();
  jac_calc(2,Range::all()) = p2.look_vector[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(derivative_cf_look)
{
  // Skip for now
  return;
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  SimpleCamera cam;
  ScLookVector sl = cam.sc_look_vector(FrameCoordinate(0,0), 0);
  ScLookVectorWithDerivative sl_wd = 
    cam.sc_look_vector_with_derivative(FrameCoordinateWithDerivative(0,0), 0);
  CartesianFixedLookVector p0 = orb->cf_look_vector(t2.value(), sl);
  CartesianFixedLookVector p1 = orb->cf_look_vector(t2.value() + eps, sl);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1.look_vector[0] - p0.look_vector[0]) / eps;
  jac_fd(1,0) = (p1.look_vector[1] - p0.look_vector[1]) / eps;
  jac_fd(2,0) = (p1.look_vector[2] - p0.look_vector[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  CartesianFixedLookVectorWithDerivative p2 = 
    orb->cf_look_vector(t2, sl_wd);
  jac_calc(0,Range::all()) = p2.look_vector[0].gradient();
  jac_calc(1,Range::all()) = p2.look_vector[1].gradient();
  jac_calc(2,Range::all()) = p2.look_vector[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(derivative_sc_look1)
{
  // Skip for now
  return;
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  SimpleCamera cam;
  ScLookVector sl = cam.sc_look_vector(FrameCoordinate(0,0), 0);
  CartesianInertialLookVector clv = orb->ci_look_vector(t2.value(), sl);
  CartesianInertialLookVectorWithDerivative 
    clv_wd(clv.look_vector[0],clv.look_vector[1],clv.look_vector[2]);
  ScLookVector p0 = orb->sc_look_vector(t2.value(), clv);
  ScLookVector p1 = orb->sc_look_vector(t2.value() + eps, clv);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1.look_vector[0] - p0.look_vector[0]) / eps;
  jac_fd(1,0) = (p1.look_vector[1] - p0.look_vector[1]) / eps;
  jac_fd(2,0) = (p1.look_vector[2] - p0.look_vector[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  ScLookVectorWithDerivative p2 = 
    orb->sc_look_vector(t2, clv_wd);
  jac_calc(0,Range::all()) = p2.look_vector[0].gradient();
  jac_calc(1,Range::all()) = p2.look_vector[1].gradient();
  jac_calc(2,Range::all()) = p2.look_vector[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(derivative_sc_look2)
{
  // Skip for now
  return;
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  SimpleCamera cam;
  ScLookVector sl = cam.sc_look_vector(FrameCoordinate(0,0), 0);
  CartesianFixedLookVector clv = orb->cf_look_vector(t2.value(), sl);
  CartesianFixedLookVectorWithDerivative 
    clv_wd(clv.look_vector[0],clv.look_vector[1],clv.look_vector[2]);
  ScLookVector p0 = orb->sc_look_vector(t2.value(), clv);
  ScLookVector p1 = orb->sc_look_vector(t2.value() + eps, clv);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1.look_vector[0] - p0.look_vector[0]) / eps;
  jac_fd(1,0) = (p1.look_vector[1] - p0.look_vector[1]) / eps;
  jac_fd(2,0) = (p1.look_vector[2] - p0.look_vector[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  ScLookVectorWithDerivative p2 = 
    orb->sc_look_vector(t2, clv_wd);
  jac_calc(0,Range::all()) = p2.look_vector[0].gradient();
  jac_calc(1,Range::all()) = p2.look_vector[1].gradient();
  jac_calc(2,Range::all()) = p2.look_vector[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
