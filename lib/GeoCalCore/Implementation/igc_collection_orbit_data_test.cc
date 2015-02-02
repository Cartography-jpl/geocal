#include "unit_test_support.h"
#include "igc_collection_orbit_data.h"
#include "simple_dem.h"
#include "quaternion_camera.h"
#include <boost/lexical_cast.hpp>
using namespace GeoCal;

class IgcCollectionOrbitDataFixture : public GlobalFixture {
public:
  IgcCollectionOrbitDataFixture()
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
    igccol.reset(new IgcCollectionOrbitData(orb_uncorr, cam, d));
    for(int i = 0; i < 10; ++i)  {
      Time tm = tmin + 20 * i;
      boost::shared_ptr<RasterImage> img;
      igccol->add_image(img, tm, "Image" + boost::lexical_cast<std::string>(i+1));
    }
  }
  virtual ~IgcCollectionOrbitDataFixture() {}
  boost::shared_ptr<IgcCollectionOrbitData> igccol;
};

BOOST_FIXTURE_TEST_SUITE(igc_collection_orbit_data, 
			 IgcCollectionOrbitDataFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  BOOST_CHECK_EQUAL(igccol->number_image(), 10);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  std::string d = serialize_write_string(igccol);
  if(false)
    std::cerr << d;
  boost::shared_ptr<IgcCollectionOrbitData> igccolr = 
    serialize_read_string<IgcCollectionOrbitData>(d);
  ImageCoordinate ic(100, 200);
  BOOST_CHECK(igccolr->image_coordinate(0, *igccol->ground_coordinate(0, ic)) == ic);
  BOOST_CHECK_MATRIX_CLOSE(igccol->parameter(), igccolr->parameter());
}

BOOST_AUTO_TEST_SUITE_END()

