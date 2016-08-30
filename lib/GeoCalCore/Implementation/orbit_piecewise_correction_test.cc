#include "unit_test_support.h"
#ifdef HAVE_HDF5
#include "hdf_orbit.h"
#endif
#include "eci_tod.h"
#include "orbit_piecewise_correction.h"
#include "geocal_quaternion.h"
using namespace GeoCal;
using namespace blitz;

class OrbitPiecewiseCorrectionFixture : public GlobalFixture {
public:
  OrbitPiecewiseCorrectionFixture() 
  {
    std::string fname = test_data_dir() + "sample_orbit.h5";
#ifdef HAVE_HDF5
    orb_uncorr.reset(new HdfOrbit<EciTod, TimeAcsCreator>(fname));
#endif
    t = Time::time_acs(215077459.472);
    Array<Time, 1> x(3);
    Array<int, 1> tp(2);
    x = t, t+10, t+20;
    tp = PiecewiseLinear::LINEAR, PiecewiseLinear::LINEAR_TO_ZERO;
    Array<double, 1> y(2);
    y = 0, 0;
    boost::shared_ptr<PiecewiseLinear> corr1(new PiecewiseLinear(x, y, tp));
    boost::shared_ptr<PiecewiseLinear> corr2(new PiecewiseLinear(x, y, tp));
    boost::shared_ptr<PiecewiseLinear> corr3(new PiecewiseLinear(x, y, tp));
    if(orb_uncorr) {
      orb.reset(new OrbitPiecewiseCorrection(orb_uncorr, corr1, corr2, corr3));
    }
  }
  boost::shared_ptr<Orbit> orb_uncorr;
  boost::shared_ptr<OrbitPiecewiseCorrection> orb;
  Time t;
};

BOOST_FIXTURE_TEST_SUITE(orbit_piecewise_correction, OrbitPiecewiseCorrectionFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  // Skip test if we don't have HDF5 support
  if(!orb_uncorr)
    return;
  Array<double, 1> parm(6);
  parm = 10, 20, 10, 20, 10, 20;
  orb->parameter(parm);
}


BOOST_AUTO_TEST_CASE(serialization)
{
  // Skip test if we don't have HDF5 or serialization support
  if(!orb_uncorr || !have_serialize_supported())
    return;
  blitz::Array<double, 1> parm(6);
  parm = 10, 20, 10, 20, 10, 20;
  orb->parameter(parm);
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Orbit> orbr = 
    serialize_read_string<OrbitPiecewiseCorrection>(d);
  BOOST_CHECK(fabs(orb->min_time() - orbr->min_time()) < 1e-3);
  BOOST_CHECK(fabs(orb->max_time() - orbr->max_time()) < 1e-3);
  BOOST_CHECK_MATRIX_CLOSE_TOL(orb->parameter(), orbr->parameter(), 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()

