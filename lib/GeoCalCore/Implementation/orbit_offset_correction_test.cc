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
  std::cerr << orb << "\n";
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

