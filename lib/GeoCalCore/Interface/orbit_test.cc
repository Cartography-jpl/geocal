#include "unit_test_support.h"
#include "orbit.h"
#include "camera.h"
#include "geodetic.h"

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(orbit, GlobalFixture)

BOOST_AUTO_TEST_CASE(kepler_orbit_data)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  BOOST_CHECK_CLOSE(orb.min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(orb.max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_CLOSE(orb.position_ci(t)->position[0], -1788501.0, 1e-4);
  BOOST_CHECK_CLOSE(orb.position_ci(t)->position[1], -6854177.0, 1e-4);
  BOOST_CHECK_CLOSE(orb.position_ci(t)->position[2], -16811.0, 1e-3);
  try {
    orb.position_ci(t - 10);
    BOOST_FAIL("Should have thrown exception");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }
  try {
    orb.position_ci(t + 110);
    BOOST_FAIL("Should have thrown exception");
  } catch(const Exception& e) {
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(kepler_orbit_data_derivative_ci)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  boost::shared_ptr<CartesianInertial> p0 = orb.position_ci(t2.value());
  boost::shared_ptr<CartesianInertial> p1 = orb.position_ci(t2.value() + eps);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1->position[0] - p0->position[0]) / eps;
  jac_fd(1,0) = (p1->position[1] - p0->position[1]) / eps;
  jac_fd(2,0) = (p1->position[2] - p0->position[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  boost::array<AutoDerivative<double>, 3> p2 = 
    orb.position_ci_with_derivative(t2);
  jac_calc(0,Range::all()) = p2[0].gradient();
  jac_calc(1,Range::all()) = p2[1].gradient();
  jac_calc(2,Range::all()) = p2[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(kepler_orbit_data_derivative_ci_look)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  SimpleCamera cam;
  ScLookVector sl = cam.sc_look_vector(FrameCoordinate(0,0), 0);
  ScLookVectorWithDerivative sl_wd = 
    cam.sc_look_vector_with_derivative(FrameCoordinateWithDerivative(0,0), 0);
  CartesianInertialLookVector p0 = orb.ci_look_vector(t2.value(), sl);
  CartesianInertialLookVector p1 = orb.ci_look_vector(t2.value() + eps, sl);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1.look_vector[0] - p0.look_vector[0]) / eps;
  jac_fd(1,0) = (p1.look_vector[1] - p0.look_vector[1]) / eps;
  jac_fd(2,0) = (p1.look_vector[2] - p0.look_vector[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  CartesianInertialLookVectorWithDerivative p2 = 
    orb.ci_look_vector(t2, sl_wd);
  jac_calc(0,Range::all()) = p2.look_vector[0].gradient();
  jac_calc(1,Range::all()) = p2.look_vector[1].gradient();
  jac_calc(2,Range::all()) = p2.look_vector[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(kepler_orbit_data_derivative_sc_look1)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  SimpleCamera cam;
  ScLookVector sl = cam.sc_look_vector(FrameCoordinate(0,0), 0);
  CartesianInertialLookVector clv = orb.ci_look_vector(t2.value(), sl);
  CartesianInertialLookVectorWithDerivative 
    clv_wd(clv.look_vector[0],clv.look_vector[1],clv.look_vector[2]);
  ScLookVector p0 = orb.sc_look_vector(t2.value(), clv);
  ScLookVector p1 = orb.sc_look_vector(t2.value() + eps, clv);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1.look_vector[0] - p0.look_vector[0]) / eps;
  jac_fd(1,0) = (p1.look_vector[1] - p0.look_vector[1]) / eps;
  jac_fd(2,0) = (p1.look_vector[2] - p0.look_vector[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  ScLookVectorWithDerivative p2 = 
    orb.sc_look_vector(t2, clv_wd);
  jac_calc(0,Range::all()) = p2.look_vector[0].gradient();
  jac_calc(1,Range::all()) = p2.look_vector[1].gradient();
  jac_calc(2,Range::all()) = p2.look_vector[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(kepler_orbit_data_derivative_cf_look)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  SimpleCamera cam;
  ScLookVector sl = cam.sc_look_vector(FrameCoordinate(0,0), 0);
  ScLookVectorWithDerivative sl_wd = 
    cam.sc_look_vector_with_derivative(FrameCoordinateWithDerivative(0,0), 0);
  CartesianFixedLookVector p0 = orb.cf_look_vector(t2.value(), sl);
  CartesianFixedLookVector p1 = orb.cf_look_vector(t2.value() + eps, sl);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1.look_vector[0] - p0.look_vector[0]) / eps;
  jac_fd(1,0) = (p1.look_vector[1] - p0.look_vector[1]) / eps;
  jac_fd(2,0) = (p1.look_vector[2] - p0.look_vector[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  CartesianFixedLookVectorWithDerivative p2 = 
    orb.cf_look_vector(t2, sl_wd);
  jac_calc(0,Range::all()) = p2.look_vector[0].gradient();
  jac_calc(1,Range::all()) = p2.look_vector[1].gradient();
  jac_calc(2,Range::all()) = p2.look_vector[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(kepler_orbit_data_derivative_sc_look2)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  SimpleCamera cam;
  ScLookVector sl = cam.sc_look_vector(FrameCoordinate(0,0), 0);
  CartesianFixedLookVector clv = orb.cf_look_vector(t2.value(), sl);
  CartesianFixedLookVectorWithDerivative 
    clv_wd(clv.look_vector[0],clv.look_vector[1],clv.look_vector[2]);
  ScLookVector p0 = orb.sc_look_vector(t2.value(), clv);
  ScLookVector p1 = orb.sc_look_vector(t2.value() + eps, clv);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1.look_vector[0] - p0.look_vector[0]) / eps;
  jac_fd(1,0) = (p1.look_vector[1] - p0.look_vector[1]) / eps;
  jac_fd(2,0) = (p1.look_vector[2] - p0.look_vector[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  ScLookVectorWithDerivative p2 = 
    orb.sc_look_vector(t2, clv_wd);
  jac_calc(0,Range::all()) = p2.look_vector[0].gradient();
  jac_calc(1,Range::all()) = p2.look_vector[1].gradient();
  jac_calc(2,Range::all()) = p2.look_vector[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

BOOST_AUTO_TEST_CASE(kepler_orbit_data_derivative_cf)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  TimeWithDerivative t2 = 
    TimeWithDerivative::time_pgs(AutoDerivative<double>(t.pgs(), 0, 1));
  double eps = 1e-3;
  boost::shared_ptr<CartesianFixed> p0 = orb.position_cf(t2.value());
  boost::shared_ptr<CartesianFixed> p1 = orb.position_cf(t2.value() + eps);
  Array<double, 2> jac_fd(3,1);
  jac_fd(0,0) = (p1->position[0] - p0->position[0]) / eps;
  jac_fd(1,0) = (p1->position[1] - p0->position[1]) / eps;
  jac_fd(2,0) = (p1->position[2] - p0->position[2]) / eps;
  Array<double, 2> jac_calc(3, 1);
  boost::array<AutoDerivative<double>, 3> p2 = 
    orb.position_cf_with_derivative(t2);
  jac_calc(0,Range::all()) = p2[0].gradient();
  jac_calc(1,Range::all()) = p2[1].gradient();
  jac_calc(2,Range::all()) = p2[2].gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_fd, jac_calc, 0.1);
}

class OrbitTest : public KeplerOrbit {
public:
  void interpolate_t(const boost::array<double, 3>& P1,
		     const boost::array<double, 3>& V1,
		     const boost::array<double, 3>& P2,
		     const boost::array<double, 3>& V2,
		     double toffset, double tspace,
		     boost::array<double, 3>& Pres,
		     boost::array<double, 3>& Vres) const
  { interpolate(P1, V1, P2, V2, toffset, tspace, Pres, Vres); }
  boost::math::quaternion<double> interpolate_t(
              const boost::math::quaternion<double>& Q1, 
              const boost::math::quaternion<double>& Q2,
	      double toffset, double tspace) const
  { return interpolate(Q1, Q2, toffset, tspace); }
};

BOOST_AUTO_TEST_CASE(basic_test)
{
  OrbitTest ot;
  boost::array<double, 3> p1 = {{1, 2, 3}};
  boost::array<double, 3> v1 = {{4, 5, 6}};
  boost::array<double, 3> p2 = {{7, 8, 9}};
  boost::array<double, 3> v2 = {{10, 11, 12}};
  boost::array<double, 3> pres, vres;
  ot.interpolate_t(p1, v1, p2, v2, 0, 2, pres, vres);
  BOOST_CHECK_CLOSE(pres[0], 1.0, 1e-4);
  BOOST_CHECK_CLOSE(pres[1], 2.0, 1e-4);
  BOOST_CHECK_CLOSE(pres[2], 3.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[0], 4.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[1], 5.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[2], 6.0, 1e-4);
  ot.interpolate_t(p1, v1, p2, v2, 2, 2, pres, vres);
  BOOST_CHECK_CLOSE(pres[0], 7.0, 1e-4);
  BOOST_CHECK_CLOSE(pres[1], 8.0, 1e-4);
  BOOST_CHECK_CLOSE(pres[2], 9.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[0], 10.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[1], 11.0, 1e-4);
  BOOST_CHECK_CLOSE(vres[2], 12.0, 1e-4);
  boost::math::quaternion<double> q1(-0.3087939999536268,    
				     -0.8376664361461180,     
				     0.3189198092995629,    
				     -0.3182001926197278);
  boost::math::quaternion<double> q2(-0.3423342205519756,    
				     -0.7019587136127043,     
				     0.5494114237300394,    
				     -0.2969988776025926);
  boost::math::quaternion<double> qres = ot.interpolate_t(q1, q2, 0, 2);
  BOOST_CHECK(l1(qres - q1) < 1e-8);
  qres = ot.interpolate_t(q1, q2, 2, 2);
  BOOST_CHECK(l1(qres - q2) < 1e-8);
}

BOOST_AUTO_TEST_CASE(orbit_data_offset)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  boost::shared_ptr<QuaternionOrbitData> od = 
    boost::dynamic_pointer_cast<QuaternionOrbitData>(orb.orbit_data(t + 50));
  boost::array<AutoDerivative<double>, 3> poff =
    {{AutoDerivative<double>(10, 0, 6),
      AutoDerivative<double>(20, 1, 6),
      AutoDerivative<double>(30, 2, 6)}};
  boost::math::quaternion<AutoDerivative<double> > sc_corr =
    quat_rot("xyz", AutoDerivative<double>(0.01, 3, 6),
	     AutoDerivative<double>(0.02, 4, 6),
	     AutoDerivative<double>(0.03, 5, 6));
  QuaternionOrbitData od2(*od, poff, sc_corr);
  BOOST_CHECK_CLOSE(od->position_ci()->position[0] + 10,
		    od2.position_ci()->position[0], 1e-4);
  BOOST_CHECK_CLOSE(od->position_ci()->position[1] + 20,
		    od2.position_ci()->position[1], 1e-4);
  BOOST_CHECK_CLOSE(od->position_ci()->position[2] + 30,
		    od2.position_ci()->position[2], 1e-4);
  BOOST_CHECK_CLOSE(od->position_ci_with_derivative()[0].value() + 10,
		    od2.position_ci_with_derivative()[0], 1e-4);
  BOOST_CHECK_CLOSE(od->position_ci_with_derivative()[1].value() + 20,
		    od2.position_ci_with_derivative()[1], 1e-4);
  BOOST_CHECK_CLOSE(od->position_ci_with_derivative()[2].value() + 30,
		    od2.position_ci_with_derivative()[2], 1e-4);
  BOOST_CHECK_CLOSE(od2.position_cf()->position[0] - 
		    od->position_cf()->position[0],
		    21.272, 1e-2);
  BOOST_CHECK_CLOSE(od2.position_cf()->position[1] - 
		    od->position_cf()->position[1],
		    -6.90276, 1e-2);
  BOOST_CHECK_CLOSE(od2.position_cf()->position[2] - 
		    od->position_cf()->position[2],
		    29.9975, 1e-2);
}

BOOST_AUTO_TEST_CASE(serialization_quaternion_orbit_data)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  boost::shared_ptr<QuaternionOrbitData> od = 
    boost::dynamic_pointer_cast<QuaternionOrbitData>(orb.orbit_data(t + 50));
  std::string d = serialize_write_string(od);
  if(true)
    std::cerr << d;
  boost::shared_ptr<QuaternionOrbitData> odr = 
    serialize_read_string<QuaternionOrbitData>(d);
}


BOOST_AUTO_TEST_SUITE_END()
