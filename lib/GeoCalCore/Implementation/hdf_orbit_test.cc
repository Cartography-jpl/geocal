#include "unit_test_support.h"
#include "hdf_orbit.h"
#include "eci_tod.h"
#include "quaternion_camera.h"
#include "geodetic.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(hdf_orbit, GlobalFixture)

// This attempts to match the results of the sc2rpc unit tests.
// **NOTE** We get results that are about 7 meters different than what
// sc2rpc gets. This was tracked down to what I believe is a more
// accurate calculation the spice toolkit for J2000 to ECEF.  We allow
// a 10 meter error in comparing to sc2rpc.

BOOST_AUTO_TEST_CASE(sc2rpc)
{
  std::string fname = test_data_dir() + "sample_orbit.h5";
  HdfOrbit<EciTod, TimeAcsCreator> orb(fname);
  QuaternionCamera cam(boost::math::quaternion<double>(1,0,0,0),
		       3375, 3648,
		       1.0 / 2500000,
		       1.0 / 2500000,
		       1.0,
		       FrameCoordinate(1688.0, 1824.5),
		       1.0, 1.0, QuaternionCamera::LINE_IS_Y);
  Time t = Time::time_acs(215077459.472);
  boost::shared_ptr<CartesianFixed> pt =
    orb.reference_surface_intersect_approximate(t, cam, FrameCoordinate(3375, 3648));
  Geodetic pt_geod(-60.3162137, 47.2465154);
  BOOST_CHECK(distance(pt_geod, *pt) < 10.0);
}

BOOST_AUTO_TEST_SUITE_END()

