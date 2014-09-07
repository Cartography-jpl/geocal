#include "air_mspi_orbit.h"
#include "unit_test_support.h"
#include "eci.h"
#include <iostream>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(air_mspi_orbit, GlobalFixture)

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
  // This corresponds to what the DidDem returns. We hardcode this, so
  // the unit test isn't dependent on having the DidDem available.
  boost::shared_ptr<Datum> datum(new SimpleDatum(-26));
  blitz::Array<double, 1> gimbal_angle(3);
  blitz::Array<double, 1> ypr_corr(3);
  gimbal_angle = 0, 0, 0;
  ypr_corr = 0, 0, 0;
  boost::shared_ptr<AirMspiOrbit> orb;
  try {
    // This might fail if GDAL doesn't support h
    orb.reset(new AirMspiOrbit(test_data_dir() + "airmspi_orbit_file_test.hdf", 
			       gimbal_angle, ypr_corr,
			       datum));
  } catch(Exception& E) {
    return;
  }
  BOOST_CHECK(orb->file_name() == test_data_dir() + 
	      "airmspi_orbit_file_test.hdf");
  BOOST_CHECK(orb->flight_description() == "This is a dummy flight description");
  BOOST_CHECK(orb->data_version() == "v1");
  BOOST_CHECK_CLOSE(orb->time_spacing(), 0.015625, 1e-4);
  Time epoch = Time::parse_time("1997-08-25T19:33:52.905714Z");
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
  BOOST_CHECK(distance(*orb->position_cf(t1), *p1) < 5);
  BOOST_CHECK(distance(*orb->position_cf(t2), *p2) < 5);
  BOOST_CHECK(distance(*orb->position_cf(t3), *p3) < 5);
}

BOOST_AUTO_TEST_SUITE_END()
