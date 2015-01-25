#include "unit_test_support.h"
#include "hdf_orbit.h"
#include "eci_tod.h"
#include "quaternion_camera.h"
#include "geodetic.h"
using namespace GeoCal;
using namespace blitz;

class HdfOrbitFixture : public GlobalFixture {
public:
  HdfOrbitFixture() 
  {
    std::string fname = test_data_dir() + "sample_orbit.h5";
    orb.reset(new HdfOrbit<EciTod, TimeAcsCreator>(fname));
    t = Time::time_acs(215077459.472);
  }
  boost::shared_ptr<HdfOrbit<EciTod, TimeAcsCreator> > orb;
  Time t;
};

BOOST_FIXTURE_TEST_SUITE(hdf_orbit, HdfOrbitFixture)

// This attempts to match the results of the sc2rpc unit tests.
// **NOTE** We get results that are about 7 meters different than what
// sc2rpc gets. This was tracked down to what I believe is a more
// accurate calculation the spice toolkit for J2000 to ECEF.  We allow
// a 10 meter error in comparing to sc2rpc.

BOOST_AUTO_TEST_CASE(sc2rpc)
{
  QuaternionCamera cam(boost::math::quaternion<double>(1,0,0,0),
		       3375, 3648,
		       1.0 / 2500000,
		       1.0 / 2500000,
		       1.0,
		       FrameCoordinate(1688.0, 1824.5),
		       QuaternionCamera::LINE_IS_Y);
  boost::shared_ptr<CartesianFixed> pt =
    orb->reference_surface_intersect_approximate(t, cam, FrameCoordinate(3375, 3648));
  Geodetic pt_geod(-60.3162137, 47.2465154);
  BOOST_CHECK(distance(pt_geod, *pt) < 10.0);
}

BOOST_AUTO_TEST_CASE(basic)
{
  Geodetic gexpect(-59.6722, 45.2541, 661831);
  BOOST_CHECK(distance(*orb->position_cf(t), gexpect) < 10);
  boost::array<double, 3> p1 = orb->position_cf(t)->position;
  double tdelta = 1.0;
  boost::array<double, 3> p2 = orb->position_cf(t + tdelta)->position;
  boost::shared_ptr<QuaternionOrbitData> od = 
    boost::dynamic_pointer_cast<QuaternionOrbitData>(orb->orbit_data(t));
  boost::array<double, 3> v = od->velocity_cf();
  for(int i = 0; i < 3; ++i)
    BOOST_CHECK(fabs((p2[i] - p1[i]) / tdelta - v[i]) < 1.0);
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


BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Orbit> orbr = 
    serialize_read_string<Orbit>(d);
  BOOST_CHECK(fabs(orb->min_time() - orbr->min_time()) < 1e-3);
  BOOST_CHECK(fabs(orb->max_time() - orbr->max_time()) < 1e-3);
}

BOOST_AUTO_TEST_SUITE_END()

