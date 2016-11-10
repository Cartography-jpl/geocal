#include "unit_test_support.h"
#ifdef HAVE_HDF5
#include "hdf_orbit.h"
#endif
#include "eci_tod.h"
#include "orbit_time_shift.h"
using namespace GeoCal;
using namespace blitz;

class OrbitTimeShiftFixture : public GlobalFixture {
public:
  OrbitTimeShiftFixture() 
  {
    std::string fname = test_data_dir() + "sample_orbit.h5";
#ifdef HAVE_HDF5
    orb_orig.reset(new HdfOrbit<EciTod, TimeAcsCreator>(fname));
#endif
    tshift = 60.0;
    t = Time::time_acs(215077459.472) + tshift;
    if(orb_orig) {
      orb.reset(new OrbitTimeShift(orb_orig, tshift));
    }
  }
  boost::shared_ptr<Orbit> orb_orig;
  boost::shared_ptr<OrbitTimeShift> orb;
  double tshift;
  Time t;
};

BOOST_FIXTURE_TEST_SUITE(orbit_time_shift, OrbitTimeShiftFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  // Skip test if we don't have HDF5 support
  if(!orb_orig)
    return;
  for(int i = 0; i < 3; ++i)
    BOOST_CHECK_CLOSE(orb->position_cf(t)->position[i],
		      orb_orig->position_cf(t-tshift)->position[i],  1e-4);
}



BOOST_AUTO_TEST_CASE(serialization)
{
  // Skip test if we don't have HDF5 or serialization support
  if(!orb_orig || !have_serialize_supported())
    return;
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Orbit> orbr = 
    serialize_read_string<OrbitTimeShift>(d);
  for(int i = 0; i < 3; ++i)
    BOOST_CHECK_CLOSE(orbr->position_cf(t)->position[i],
		      orb_orig->position_cf(t-tshift)->position[i],  1e-4);
}

BOOST_AUTO_TEST_SUITE_END()

