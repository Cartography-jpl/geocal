#include "air_mspi_orbit.h"
#include "unit_test_support.h"
#include "eci.h"
#include <iostream>

using namespace GeoCal;
using namespace blitz;

class AirMspiOrbitFixture : public GlobalFixture {
public:
  AirMspiOrbitFixture() {
    epoch = Time::parse_time("1997-08-25T19:33:52.905714Z");
    // This corresponds to what the DidDem returns. We hardcode this, so
    // the unit test isn't dependent on having the DidDem available.
    boost::shared_ptr<Datum> datum(new SimpleDatum(-26));
    boost::shared_ptr<MspiGimbal> gimbal(new MspiGimbal(0,0,0));
    try {
      // This might fail if GDAL doesn't support hdf
      orb.reset(new AirMspiOrbit(test_data_dir() + "airmspi_orbit_file_test.hdf", 
				 gimbal, datum));
    } catch(Exception& E) {
      return;
    }
  }
  boost::shared_ptr<AirMspiOrbit> orb;
  Time epoch;
};

BOOST_FIXTURE_TEST_SUITE(air_mspi_orbit, AirMspiOrbitFixture)

BOOST_AUTO_TEST_CASE(nav_data_test)
{
  blitz::Array<double, 1> d(14);
  d = 0.3, 0.1, 0.2, 0.6, 0.5, 0.4, 0.8, 0.9, 1.1, 1.2, 1.0, 0.7, 1.3, 1.4;
  SimpleDatum datum(10);
  AirMspiNavData n1(d, datum, false);
  AirMspiNavData n2(d, datum, true);
  BOOST_CHECK_CLOSE(n1.position.latitude() * Constant::deg_to_rad,
		    0.1, 1e-4);
  BOOST_CHECK_CLOSE(n1.position.longitude() * Constant::deg_to_rad,
		    0.2, 1e-4);
  BOOST_CHECK_CLOSE(n1.position.height_reference_surface(),
		    10 + 0.3, 1e-4);
  BOOST_CHECK_CLOSE(n1.velocity[0], 0.4, 1e-4);
  BOOST_CHECK_CLOSE(n1.velocity[1], 0.5, 1e-4);
  BOOST_CHECK_CLOSE(n1.velocity[2], 0.6, 1e-4);
  BOOST_CHECK_CLOSE(n1.ypr[0], 0.7, 1e-4);
  BOOST_CHECK_CLOSE(n1.ypr[1], 0.8, 1e-4);
  BOOST_CHECK_CLOSE(n1.ypr[2], 0.9, 1e-4);
  BOOST_CHECK_CLOSE(n1.ypr_rate[0], 1.0, 1e-4);
  BOOST_CHECK_CLOSE(n1.ypr_rate[1], 1.1, 1e-4);
  BOOST_CHECK_CLOSE(n1.ypr_rate[2], 1.2, 1e-4);
  BOOST_CHECK_CLOSE(n1.gimbal_pos, 1.3, 1e-4);
  BOOST_CHECK_CLOSE(n1.gimbal_vel, 1.4, 1e-4);

  BOOST_CHECK_CLOSE(n2.position.latitude() * Constant::deg_to_rad,
		    0.1, 1e-4);
  BOOST_CHECK_CLOSE(n2.position.longitude() * Constant::deg_to_rad,
		    0.2, 1e-4);
  BOOST_CHECK_CLOSE(n2.position.height_reference_surface(),
		    10 + 0.3, 1e-4);
  BOOST_CHECK_CLOSE(n2.velocity[0], 0.4, 1e-4);
  BOOST_CHECK_CLOSE(n2.velocity[1], 0.5, 1e-4);
  BOOST_CHECK_CLOSE(n2.velocity[2], 0.6, 1e-4);
  BOOST_CHECK_CLOSE(n2.ypr[0], atan2(0.4, 0.5) - 0.7, 1e-4);
  BOOST_CHECK_CLOSE(n2.ypr[1], 0.8, 1e-4);
  BOOST_CHECK_CLOSE(n2.ypr[2], 0.9, 1e-4);
  BOOST_CHECK_CLOSE(n2.ypr_rate[0], -1.0, 1e-4);
  BOOST_CHECK_CLOSE(n2.ypr_rate[1], 1.1, 1e-4);
  BOOST_CHECK_CLOSE(n2.ypr_rate[2], 1.2, 1e-4);
  BOOST_CHECK_CLOSE(n2.gimbal_pos, 1.3, 1e-4);
  BOOST_CHECK_CLOSE(n2.gimbal_vel, 1.4, 1e-4);
}

BOOST_AUTO_TEST_CASE(basic_test)
{
  // We might be missing orb if GDAL support wasn't built in. Silently
  // skip test if we are missing orb.
  if(!orb)
    return;
  BOOST_CHECK(orb->file_name() ==
	      air_mspi_permanent_file_name(test_data_dir() + 
					   "airmspi_orbit_file_test.hdf"));
  BOOST_CHECK(orb->flight_description() == "This is a dummy flight description");
  BOOST_CHECK(orb->data_version() == "v1");
  BOOST_CHECK_CLOSE(orb->time_spacing(), 0.015625, 1e-4);
  BOOST_CHECK_CLOSE(orb->min_time() - epoch, 0.490792, 1e-4);
  BOOST_CHECK_CLOSE(orb->max_time() - epoch, 16.975167, 1e-4);
  BOOST_CHECK_CLOSE(orb->nav_data(634).gimbal_pos, 0.0522535, 1e-4);
  BOOST_CHECK_CLOSE(orb->nav_data(634).gimbal_vel, -0.00102626475, 1e-4);
  // The original test data was in ECI, so go ahead and convert this
  // to compare.
  Time t1 = epoch + 10;
  Time t2 = epoch + 10.5;
  Time t3 = epoch + 11.0;
  boost::shared_ptr<CartesianFixed> p1, p2, p3;
  p1 = Eci(-4.04227e+06, 2.89117e+06, 4.01598e+06).convert_to_cf(t1);
  p2 = Eci(-4.04237e+06, 2.89102e+06, 4.01598e+06).convert_to_cf(t2);
  p3 = Eci(-4.04248e+06, 2.89087e+06, 4.01598e+06).convert_to_cf(t3);
  BOOST_CHECK(GeoCal::distance(*orb->position_cf(t1), *p1) < 5);
  BOOST_CHECK(GeoCal::distance(*orb->position_cf(t2), *p2) < 5);
  BOOST_CHECK(GeoCal::distance(*orb->position_cf(t3), *p3) < 5);
}

BOOST_AUTO_TEST_CASE(derivative_ci)
{
  // We might be missing orb if GDAL support wasn't built in. Silently
  // skip test if we are missing orb.
  if(!orb)
    return;
  Time t = epoch + 10.5;
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
  // We might be missing orb if GDAL support wasn't built in. Silently
  // skip test if we are missing orb.
  if(!orb)
    return;
  Time t = epoch + 10.5;
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
  // We might be missing orb if GDAL support wasn't built in. Silently
  // skip test if we are missing orb.
  if(!orb)
    return;
  Time t = epoch + 10.5;
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
  // We might be missing orb if GDAL support wasn't built in. Silently
  // skip test if we are missing orb.
  if(!orb)
    return;
  Time t = epoch + 10.5;
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
  // We might be missing orb if GDAL support wasn't built in. Silently
  // skip test if we are missing orb.
  if(!orb)
    return;
  Time t = epoch + 10.5;
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
  // We might be missing orb if GDAL support wasn't built in. Silently
  // skip test if we are missing orb.
  if(!orb)
    return;
  Time t = epoch + 10.5;
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
  // We might be missing orb if GDAL support wasn't built in. Silently
  // skip test if we are missing orb.
  if(!orb)
    return;
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<AirMspiOrbit> orbr = 
    serialize_read_string<AirMspiOrbit>(d);
  BOOST_CHECK(orb->file_name() ==
	      air_mspi_permanent_file_name(test_data_dir() + 
					   "airmspi_orbit_file_test.hdf"));
  BOOST_CHECK(orbr->flight_description() == "This is a dummy flight description");
  BOOST_CHECK(orbr->data_version() == "v1");
  BOOST_CHECK_CLOSE(orbr->time_spacing(), 0.015625, 1e-4);
  BOOST_CHECK_CLOSE(orbr->min_time() - epoch, 0.490792, 1e-4);
  BOOST_CHECK_CLOSE(orbr->max_time() - epoch, 16.975167, 1e-4);
  BOOST_CHECK_CLOSE(orbr->nav_data(634).gimbal_pos, 0.0522535, 1e-4);
  BOOST_CHECK_CLOSE(orbr->nav_data(634).gimbal_vel, -0.00102626475, 1e-4);
  Time t1 = epoch + 10;
  Time t2 = epoch + 10.5;
  Time t3 = epoch + 11.0;
  boost::shared_ptr<CartesianFixed> p1, p2, p3;
  p1 = Eci(-4.04227e+06, 2.89117e+06, 4.01598e+06).convert_to_cf(t1);
  p2 = Eci(-4.04237e+06, 2.89102e+06, 4.01598e+06).convert_to_cf(t2);
  p3 = Eci(-4.04248e+06, 2.89087e+06, 4.01598e+06).convert_to_cf(t3);
  BOOST_CHECK(GeoCal::distance(*orbr->position_cf(t1), *p1) < 5);
  BOOST_CHECK(GeoCal::distance(*orbr->position_cf(t2), *p2) < 5);
  BOOST_CHECK(GeoCal::distance(*orbr->position_cf(t3), *p3) < 5);
}

BOOST_AUTO_TEST_SUITE_END()
