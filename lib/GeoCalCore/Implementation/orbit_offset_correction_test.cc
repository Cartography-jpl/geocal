#include "unit_test_support.h"
#ifdef HAVE_HDF5
#include "hdf_orbit.h"
#endif
#include "eci_tod.h"
#include "orbit_offset_correction.h"
#include "geocal_quaternion.h"
using namespace GeoCal;
using namespace blitz;

class OrbitOffsetCorrectionFixture : public GlobalFixture {
public:
  OrbitOffsetCorrectionFixture() 
  {
    std::string fname = test_data_dir() + "sample_orbit.h5";
#ifdef HAVE_HDF5
    orb_uncorr.reset(new HdfOrbit<EciTod, TimeAcsCreator>(fname));
#endif
    t = Time::time_acs(215077459.472);
    if(orb_uncorr) {
      orb.reset(new OrbitOffsetCorrection(orb_uncorr));
    }
  }
  boost::math::quaternion<double>
  sc_to_sc_corr(Time T) const 
  {
    boost::shared_ptr<QuaternionOrbitData> od = 
      boost::dynamic_pointer_cast<QuaternionOrbitData>(orb->orbit_data(T));
    boost::shared_ptr<QuaternionOrbitData> od_uncorr = 
      boost::dynamic_pointer_cast<QuaternionOrbitData>(orb_uncorr->orbit_data(T));
    // Od has sc in corrected frame, so if you work it this expression
    // below is sc_to_sc_corr.
    return conj(conj(od_uncorr->sc_to_cf()) * od->sc_to_cf());
  }
  boost::math::quaternion<AutoDerivative<double> >
  sc_to_sc_corr_with_derivative(TimeWithDerivative T) const 
  {
    boost::shared_ptr<QuaternionOrbitData> od = 
      boost::dynamic_pointer_cast<QuaternionOrbitData>(orb->orbit_data(T));
    boost::shared_ptr<QuaternionOrbitData> od_uncorr = 
      boost::dynamic_pointer_cast<QuaternionOrbitData>(orb_uncorr->orbit_data(T));
    // Od has sc in corrected frame, so if you work it this expression
    // below is sc_to_sc_corr.
    return conj(conj(od_uncorr->sc_to_cf_with_derivative()) * od->sc_to_cf_with_derivative());
  }
  boost::shared_ptr<Orbit> orb_uncorr;
  boost::shared_ptr<OrbitOffsetCorrection> orb;
  Time t;
};

BOOST_FIXTURE_TEST_SUITE(orbit_offset_correction, OrbitOffsetCorrectionFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  // Skip test if we don't have HDF5 support
  if(!orb_uncorr)
    return;
  orb->insert_position_time_point(t);
  orb->insert_attitude_time_point(t);
  orb->insert_attitude_time_point(t + 10);
  blitz::Array<double, 1> parm(9);
  parm = 1, 2, 3, 4, 5, 6, 7, 8, 9;
  orb->parameter(parm);
  orb->add_identity_gradient();
  BOOST_CHECK_MATRIX_CLOSE_TOL(orb->parameter(), parm, 1e-4);
  std::vector<Time> tp = orb->attitude_time_point();
  BOOST_CHECK(fabs(t - tp[0]) < 1e-4);
  BOOST_CHECK((t + 10 - tp[1]) < 1e-4);
  blitz::Array<bool, 1> pm(9);
  pm = true, true, true, true, true, true, true, true, true;
  BOOST_CHECK(blitz::all(orb->parameter_mask() == pm));
  orb->fit_position_x(false);
  orb->fit_position_y(false);
  orb->fit_position_z(false);
  pm = false, false, false, true, true, true, true, true, true;
  BOOST_CHECK(blitz::all(orb->parameter_mask() == pm));
  orb->fit_position_x(true);
  orb->fit_position_y(true);
  orb->fit_position_z(true);
  orb->fit_yaw(false);
  pm = true, true, true, false, true, true, false, true, true;
  BOOST_CHECK(blitz::all(orb->parameter_mask() == pm));
  orb->fit_yaw(true);
  orb->fit_pitch(false);
  pm = true, true, true, true, false, true, true, false, true;
  BOOST_CHECK(blitz::all(orb->parameter_mask() == pm));
  orb->fit_pitch(true);
  orb->fit_roll(false);
  pm = true, true, true, true, true, false, true, true, false;
  BOOST_CHECK(blitz::all(orb->parameter_mask() == pm));
  orb->fit_roll(true);
  for(int i = 0; i < 3; ++i)
    BOOST_CHECK_CLOSE(orb->position_ci(t + 5)->position[i] -
		      orb_uncorr->position_ci(t + 5)->position[i], parm(i), 1e-4);
  TimeWithDerivative td(t + 5);
  for(int i = 0; i < 3; ++i) {
    BOOST_CHECK_CLOSE(orb->position_ci_with_derivative(td)[i].value() -
		      orb_uncorr->position_ci_with_derivative(td)[i].value(), parm(i), 
		      1e-4);
    blitz::Array<double, 1> gexp(9);
    gexp = 0;
    gexp(i) = 1;
    BOOST_CHECK_MATRIX_CLOSE_TOL(orb->position_ci_with_derivative(td)[i].gradient(), gexp, 1e-4);
  }
}


BOOST_AUTO_TEST_CASE(check_attitude)
{
  // Skip test if we don't have HDF5 support
  if(!orb_uncorr)
    return;
  // We have a separate test for the attitude, just because it is so long
  orb->insert_position_time_point(t);
  orb->insert_attitude_time_point(t);
  orb->insert_attitude_time_point(t + 10);
  blitz::Array<double, 1> parm(9);
  parm = 1, 2, 3, 4, 5, 6, 7, 8, 9;
  orb->parameter(parm);
  orb->add_identity_gradient();
  double yaw, pitch, roll;
  // Check past edge cases
  quat_to_ypr(sc_to_sc_corr(t - 5), yaw, pitch, roll);
  yaw /= Constant::arcsecond_to_rad; 
  pitch /= Constant::arcsecond_to_rad; 
  roll /= Constant::arcsecond_to_rad; 
  BOOST_CHECK_CLOSE(yaw, parm(3 + 0),1e-4);
  BOOST_CHECK_CLOSE(pitch, parm(3 + 1),1e-4);
  BOOST_CHECK_CLOSE(roll, parm(3 + 2),1e-4);
  quat_to_ypr(sc_to_sc_corr(t + 15), yaw, pitch, roll);
  yaw /= Constant::arcsecond_to_rad; 
  pitch /= Constant::arcsecond_to_rad; 
  roll /= Constant::arcsecond_to_rad; 
  BOOST_CHECK_CLOSE(yaw, parm(6 + 0),1e-4);
  BOOST_CHECK_CLOSE(pitch, parm(6 + 1),1e-4);
  BOOST_CHECK_CLOSE(roll, parm(6 + 2),1e-4);
  // Check in between time points case.
  boost::math::quaternion<AutoDerivative<double> > qexpect =
    interpolate_quaternion_rotation(sc_to_sc_corr_with_derivative(t - 5), 
				    sc_to_sc_corr_with_derivative(t + 15), 
				    AutoDerivative<double>(3.0), 
				    10.0);
  AutoDerivative<double> yaw_expect, pitch_expect, roll_expect;
  quat_to_ypr(qexpect, yaw_expect, pitch_expect, roll_expect);
  yaw_expect /= Constant::arcsecond_to_rad; 
  pitch_expect /= Constant::arcsecond_to_rad; 
  roll_expect /= Constant::arcsecond_to_rad; 
  quat_to_ypr(sc_to_sc_corr(t + 3), yaw, pitch, roll);
  yaw /= Constant::arcsecond_to_rad; 
  pitch /= Constant::arcsecond_to_rad; 
  roll /= Constant::arcsecond_to_rad; 
  BOOST_CHECK_CLOSE(yaw, yaw_expect.value(), 1e-4);
  BOOST_CHECK_CLOSE(pitch, pitch_expect.value(), 1e-4);
  BOOST_CHECK_CLOSE(roll, roll_expect.value(), 1e-4);

  // Now check with gradients.
  AutoDerivative<double> yawd, pitchd, rolld;
  blitz::Array<double, 1> gexp(9);
  quat_to_ypr(sc_to_sc_corr_with_derivative(t - 5), yawd, pitchd, rolld);
  yawd /= Constant::arcsecond_to_rad; 
  pitchd /= Constant::arcsecond_to_rad; 
  rolld /= Constant::arcsecond_to_rad; 
  BOOST_CHECK_CLOSE(yawd.value(), parm(3 + 0),1e-4);
  BOOST_CHECK_CLOSE(pitchd.value(), parm(3 + 1),1e-4);
  BOOST_CHECK_CLOSE(rolld.value(), parm(3 + 2),1e-4);
  gexp = 0;
  gexp(3+0) = 1;
  BOOST_CHECK_MATRIX_CLOSE_TOL(yawd.gradient(), gexp, 1e-4);
  gexp = 0;
  gexp(3+1) = 1;
  BOOST_CHECK_MATRIX_CLOSE_TOL(pitchd.gradient(), gexp, 1e-4);
  gexp = 0;
  gexp(3+2) = 1;
  BOOST_CHECK_MATRIX_CLOSE_TOL(rolld.gradient(), gexp, 1e-4);
  
  quat_to_ypr(sc_to_sc_corr_with_derivative(t + 15), yawd, pitchd, rolld);
  yawd /= Constant::arcsecond_to_rad; 
  pitchd /= Constant::arcsecond_to_rad; 
  rolld /= Constant::arcsecond_to_rad; 
  BOOST_CHECK_CLOSE(yawd.value(), parm(6 + 0),1e-4);
  BOOST_CHECK_CLOSE(pitchd.value(), parm(6 + 1),1e-4);
  BOOST_CHECK_CLOSE(rolld.value(), parm(6 + 2),1e-4);
  gexp = 0;
  gexp(6+0) = 1;
  BOOST_CHECK_MATRIX_CLOSE_TOL(yawd.gradient(), gexp, 1e-4);
  gexp = 0;
  gexp(6+1) = 1;
  BOOST_CHECK_MATRIX_CLOSE_TOL(pitchd.gradient(), gexp, 1e-4);
  gexp = 0;
  gexp(6+2) = 1;
  BOOST_CHECK_MATRIX_CLOSE_TOL(rolld.gradient(), gexp, 1e-4);

  quat_to_ypr(sc_to_sc_corr_with_derivative(t + 3), yawd, pitchd, rolld);
  yawd /= Constant::arcsecond_to_rad; 
  pitchd /= Constant::arcsecond_to_rad; 
  rolld /= Constant::arcsecond_to_rad; 
  BOOST_CHECK_CLOSE(yawd.value(), yaw_expect.value(), 1e-4);
  BOOST_CHECK_CLOSE(pitchd.value(), pitch_expect.value(), 1e-4);
  BOOST_CHECK_CLOSE(rolld.value(), roll_expect.value(), 1e-4);
  BOOST_CHECK_MATRIX_CLOSE_TOL(yawd.gradient(), yaw_expect.gradient(), 1e-4);
  BOOST_CHECK_MATRIX_CLOSE_TOL(pitchd.gradient(), pitch_expect.gradient(), 1e-4);
  BOOST_CHECK_MATRIX_CLOSE_TOL(rolld.gradient(), roll_expect.gradient(), 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  // Skip test if we don't have HDF5 or serialization support
  if(!orb_uncorr || !have_serialize_supported())
    return;
  orb->insert_position_time_point(t);
  orb->insert_attitude_time_point(t);
  orb->insert_attitude_time_point(t + 10);
  blitz::Array<double, 1> parm(9);
  parm = 1, 2, 3, 4, 5, 6, 7, 8, 9;
  orb->parameter(parm);
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Orbit> orbr = 
    serialize_read_string<OrbitOffsetCorrection>(d);
  BOOST_CHECK(fabs(orb->min_time() - orbr->min_time()) < 1e-3);
  BOOST_CHECK(fabs(orb->max_time() - orbr->max_time()) < 1e-3);
  BOOST_CHECK_MATRIX_CLOSE_TOL(orb->parameter(), orbr->parameter(), 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()

