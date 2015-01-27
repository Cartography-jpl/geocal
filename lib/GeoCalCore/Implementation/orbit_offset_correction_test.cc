#include "unit_test_support.h"
#include "hdf_orbit.h"
#include "eci_tod.h"
#include "orbit_offset_correction.h"
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
  }
  boost::shared_ptr<Orbit> orb_uncorr;
  Time t;
};

BOOST_FIXTURE_TEST_SUITE(orbit_offset_correction, OrbitOffsetCorrectionFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  // Skip test if we don't have HDF5 support
  if(!orb_uncorr)
    return;
  OrbitOffsetCorrection orb(orb_uncorr);
  orb.insert_time_point(t);
  orb.insert_time_point(t + 10);
  blitz::Array<double, 1> parm(9);
  parm = 1, 2, 3, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06;
  orb.parameter(parm);
  BOOST_CHECK_MATRIX_CLOSE_TOL(orb.parameter(), parm, 1e-4);
  std::vector<Time> tp = orb.time_point();
  BOOST_CHECK(fabs(t - tp[0]) < 1e-4);
  BOOST_CHECK((t + 10 - tp[1]) < 1e-4);
  blitz::Array<bool, 1> pm(9);
  pm = true, true, true, true, true, true, true, true, true;
  BOOST_CHECK(blitz::all(orb.parameter_mask() == pm));
  orb.fit_position(false);
  pm = false, false, false, true, true, true, true, true, true;
  BOOST_CHECK(blitz::all(orb.parameter_mask() == pm));
  orb.fit_position(true);
  orb.fit_yaw(false);
  pm = true, true, true, false, true, true, false, true, true;
  BOOST_CHECK(blitz::all(orb.parameter_mask() == pm));
  orb.fit_yaw(true);
  orb.fit_pitch(false);
  pm = true, true, true, true, false, true, true, false, true;
  BOOST_CHECK(blitz::all(orb.parameter_mask() == pm));
  orb.fit_pitch(true);
  orb.fit_roll(false);
  pm = true, true, true, true, true, false, true, true, false;
  BOOST_CHECK(blitz::all(orb.parameter_mask() == pm));
  orb.fit_roll(true);
  orb.add_identity_gradient();
  boost::shared_ptr<OrbitData> od = orb.orbit_data(t + 5);
  boost::shared_ptr<OrbitData> od2 = orb.orbit_data(TimeWithDerivative(t + 5));
  for(int i = 0; i < 3; ++i)
    BOOST_CHECK_CLOSE(orb.position_ci(t + 5)->position[i] -
		      orb_uncorr->position_ci(t + 5)->position[i], parm(i), 1e-4);
  TimeWithDerivative td(t + 5);
  for(int i = 0; i < 3; ++i) {
    BOOST_CHECK_CLOSE(orb.position_ci_with_derivative(td)[i].value() -
		      orb_uncorr->position_ci_with_derivative(td)[i].value(), parm(i), 
		      1e-4);
    blitz::Array<double, 1> gexp(9);
    gexp = 0;
    gexp(i) = 1;
    BOOST_CHECK_MATRIX_CLOSE_TOL(orb.position_ci_with_derivative(td)[i].gradient(), gexp, 1e-4);
  }

}

BOOST_AUTO_TEST_CASE(serialization)
{
  // Skip test if we don't have HDF5 or serialization support
  if(!orb_uncorr || !have_serialize_supported())
    return;
  boost::shared_ptr<OrbitOffsetCorrection> 
    orb(new OrbitOffsetCorrection(orb_uncorr));
  orb->insert_time_point(t);
  orb->insert_time_point(t + 10);
  blitz::Array<double, 1> parm(9);
  parm = 1, 2, 3, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06;
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

