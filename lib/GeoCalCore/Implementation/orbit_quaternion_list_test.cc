#include "unit_test_support.h"
#include "orbit_quaternion_list.h"
#include "aircraft_orbit_data.h"

using namespace GeoCal;
using namespace blitz;
class OrbitQuaternionListFixture : public GlobalFixture {
public:
  OrbitQuaternionListFixture()  {
    Time t1 = Time::time_pgs(100.0);
    Time t2 = Time::time_pgs(102.0);
    Geodetic p1(10, 20, 200);
    Geodetic p2(12, 22, 400);
    od1.reset(new AircraftOrbitData(t1, p1, t2, p2, 10, 20, 30));
    od2.reset(new AircraftOrbitData(t2, p2, t1, p1, 20, 30, 20));
    std::vector<boost::shared_ptr<QuaternionOrbitData> > v;
    v.push_back(od2);
    v.push_back(od1);
    orb.reset(new OrbitQuaternionList(v));
    t = Time::time_pgs(101);
  }
  boost::shared_ptr<OrbitQuaternionList> orb;
  boost::shared_ptr<AircraftOrbitData> od1, od2;
  Time t;
};

BOOST_FIXTURE_TEST_SUITE(orbit_quaternion_list, OrbitQuaternionListFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  BOOST_CHECK_CLOSE(orb->min_time().pgs(), 100.0, 1e-6);
  BOOST_CHECK_CLOSE(orb->max_time().pgs(), 102.0, 1e-6);
  BOOST_CHECK(distance(*orb->orbit_data(Time::time_pgs(100.0))->position_cf(),
		       od1->position_geodetic()) < 10);
  Geodetic g1(*orb->orbit_data(Time::time_pgs(101.999))->position_cf());
  BOOST_CHECK(distance(*orb->orbit_data(Time::time_pgs(101.99999))->position_cf(),
		       od2->position_geodetic()) < 10);
  Geodetic g2(*orb->orbit_data(Time::time_pgs(101))->position_cf());
  BOOST_CHECK_CLOSE(orb->orbit_data(Time::time_pgs(101))->position_cf()->
		    latitude(), 11.0, 1.0);
  BOOST_CHECK_CLOSE(orb->orbit_data(Time::time_pgs(101))->position_cf()->
		    longitude(), 21.0, 1.0);
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
