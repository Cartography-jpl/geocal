#include "unit_test_support.h"
#include "igc_collection_rolling_shutter.h"
#include "simple_dem.h"
#include "quaternion_camera.h"
using namespace GeoCal;

class IgcCollectionRollingShutteryFixture : public GlobalFixture {
public:
  IgcCollectionRollingShutteryFixture()
  {
    boost::shared_ptr<Dem> d(new SimpleDem());
    boost::shared_ptr<Camera> 
      cam(new QuaternionCamera(boost::math::quaternion<double>(1,0,0,0),
			       3375, 3648, 1.0 / 2500000, 1.0 / 2500000,
			       1.0, FrameCoordinate(1688.0, 1824.5),
			       QuaternionCamera::LINE_IS_Y));
    Time tmin = Time::parse_time("1998-06-30T10:51:28.32Z");
    boost::shared_ptr<Orbit>
      orb_uncorr(new KeplerOrbit(tmin, tmin + 1000));
  }
  virtual ~IgcCollectionRollingShutteryFixture() {}
  boost::shared_ptr<IgcCollectionRollingShutter> igccol;
};

BOOST_FIXTURE_TEST_SUITE(igc_collection_rolling_shutter, 
			 IgcCollectionRollingShutteryFixture)

BOOST_AUTO_TEST_CASE(basic)
{
}

BOOST_AUTO_TEST_CASE(serialization)
{
  // if(!have_serialize_supported())
  //   return;
  // std::string d = serialize_write_string(igccol);
  // if(false)
  //   std::cerr << d;
  // boost::shared_ptr<IgcArray> igccolr = 
  //   serialize_read_string<IgcArray>(d);
  // ImageCoordinate ic(100, 200);
  // BOOST_CHECK(igccolr->image_coordinate(0, *igccol->ground_coordinate(0, ic)) == ic);
  // BOOST_CHECK_MATRIX_CLOSE(igccol->parameter(), igccolr->parameter());
}

BOOST_AUTO_TEST_SUITE_END()

