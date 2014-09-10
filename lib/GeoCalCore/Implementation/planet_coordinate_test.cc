#include "unit_test_support.h"
#include "planet_coordinate.h"
#include "geocal_time.h"
#include "spice_helper.h"
#include "vicar_lite_file.h"
#include "quaternion_camera.h"
#include "orbit_data_image_ground_connection.h"
#include "galileo_camera.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mars_coordinate, GlobalFixture)

BOOST_AUTO_TEST_CASE(mars_constant)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  BOOST_CHECK_CLOSE(MarsConstant::planet_a(), 3396190.0, 1e-8);
  BOOST_CHECK_CLOSE(MarsConstant::planet_b(), 3376200.0, 1e-8);
  BOOST_CHECK_CLOSE(MarsConstant::planet_esq(), 0.0117373700261, 1e-8);
}

BOOST_AUTO_TEST_CASE(mars_fixed)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  MarsFixed mf(10, 20, 30);
  BOOST_CHECK_CLOSE(mf.position[0], 10.0, 1e-8);
  BOOST_CHECK_CLOSE(mf.position[1], 20.0, 1e-8);
  BOOST_CHECK_CLOSE(mf.position[2], 30.0, 1e-8);
  boost::shared_ptr<GroundCoordinate> gc = mf.convert_to_cf();
  MarsFixed mf2(*gc);
  BOOST_CHECK_CLOSE(mf2.position[0], 10.0, 1e-8);
  BOOST_CHECK_CLOSE(mf2.position[1], 20.0, 1e-8);
  BOOST_CHECK_CLOSE(mf2.position[2], 30.0, 1e-8);
  BOOST_CHECK_CLOSE(mf.min_radius_reference_surface(), 3376200.0, 1e-8);

  // Results from an old unit test in the old version of GeoCal.
  MarsFixed e3mf(0.5, 0.75, 0.90);
  Time t = Time::parse_time("1991-01-01T11:29:30.123211Z") + 3600.0;
  boost::shared_ptr<CartesianInertial> m3mi = e3mf.convert_to_ci(t);
  BOOST_CHECK(boost::dynamic_pointer_cast<MarsInertial>(m3mi));
  BOOST_CHECK_CLOSE(m3mi->position[0], 1.1578655, 1e-4);
  BOOST_CHECK_CLOSE(m3mi->position[1], -0.4738199, 1e-4);
  BOOST_CHECK_CLOSE(m3mi->position[2], 0.23946256, 1e-4);

  MarsPlanetocentric mp(10, 20, 30);
  MarsFixed mf4(mp);
  BOOST_CHECK_CLOSE(mf4.latitude(), 10, 1e-8);
  BOOST_CHECK_CLOSE(mf4.longitude(), 20, 1e-8);
  BOOST_CHECK_CLOSE(mf4.height_reference_surface(), 30, 1e-8);
}

BOOST_AUTO_TEST_CASE(mars_inertial)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  MarsInertial mi(10, 20, 30);
  BOOST_CHECK_CLOSE(mi.position[0], 10.0, 1e-8);
  BOOST_CHECK_CLOSE(mi.position[1], 20.0, 1e-8);
  BOOST_CHECK_CLOSE(mi.position[2], 30.0, 1e-8);

  // Results from an old unit test in the old version of GeoCal.
  MarsInertial m3mi(0.5, 0.75, 0.90);
  Time t = Time::parse_time("1991-01-01T11:29:30.123211Z") + 3600.0;
  boost::shared_ptr<CartesianFixed> m3mf = m3mi.convert_to_cf(t);
  BOOST_CHECK(boost::dynamic_pointer_cast<MarsFixed>(m3mf));
  BOOST_CHECK_CLOSE(m3mf->position[0], -0.992024, 1e-4);
  BOOST_CHECK_CLOSE(m3mf->position[1], 0.48329419, 1e-4);
  BOOST_CHECK_CLOSE(m3mf->position[2], 0.636251, 1e-4);
}

BOOST_AUTO_TEST_CASE(mars_planetocentric)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  MarsPlanetocentric mp(10, 20, 30);
  BOOST_CHECK_CLOSE(mp.height_reference_surface(), 30, 1e-8);
  BOOST_CHECK_CLOSE(mp.latitude(), 10, 1e-8);
  BOOST_CHECK_CLOSE(mp.longitude(), 20, 1e-8);
  MarsFixed mf(mp);
  MarsPlanetocentric mp2(mf);
  MarsFixed mf2(mp2);
  BOOST_CHECK(distance(mp, mp2) < 1e-8);
  BOOST_CHECK(distance(mf, mf2) < 1e-8);
}
BOOST_AUTO_TEST_SUITE_END()

class GalileoFixture: public GlobalFixture {
public:
  GalileoFixture()
    : galileo_name("GLL"),
      sc_frame_name("GLL_SCAN_PLANE")
  {
    SpiceHelper::add_kernel(test_data_dir() + "/galileo_kernels",
     			    "galileo.ker");
    double day_to_sec = 24 * 60 * 60;
    // We can write a general conversion from the time given in VICAR
    // to time. But this is "day 350". The -1 is because we start with
    // day 1 (not 0), and the +1 is because we had a leapsecond in
    // 1997. 
    tm3800 = Time::parse_time("1997-01-01T12:18:20.130Z") + 
      (350 - 1) * day_to_sec + 1;
    tm2828 = Time::parse_time("1997-01-01T12:08:31.202Z") + 
      (350 - 1) * day_to_sec + 1;
    // Don't have +1 because no leapsecond in 1996
    tm9400 = Time::parse_time("1996-01-01T19:50:52.696Z") + 
      (311 - 1) * day_to_sec;
  }
  // Note that 3800, 2828, 9400 refer to some VICAR data that we
  // acquired at one point. This just provides some test data, we know 
  // what the range, latitude, and longitude should be for this data.
  Time tm3800, tm2828, tm9400;
  std::string galileo_name;
  std::string sc_frame_name;
};

BOOST_FIXTURE_TEST_SUITE(galileo, GalileoFixture)
BOOST_AUTO_TEST_CASE(target_position)
{
  EuropaFixed p3800 = EuropaFixed::target_position(galileo_name, tm3800);
  EuropaFixed p2828 = EuropaFixed::target_position(galileo_name, tm2828);
  EuropaFixed p9400 = EuropaFixed::target_position(galileo_name, tm9400);
  BOOST_CHECK_CLOSE(norm(p3800.position), 5.79122e+06, 1e-4);
  BOOST_CHECK_CLOSE(norm(p2828.position), 2.60614e+06, 1e-4);
  BOOST_CHECK_CLOSE(norm(p9400.position), 4.29518e+07, 1e-4);
  BOOST_CHECK_CLOSE(p3800.latitude(), -2.61439, 5e-4);
  BOOST_CHECK_CLOSE(p3800.longitude(), -152.651, 5e-4);
  BOOST_CHECK_CLOSE(p2828.latitude(), -5.94665, 5e-4);
  BOOST_CHECK_CLOSE(p2828.longitude(), -177.516, 5e-4);
  BOOST_CHECK_CLOSE(p9400.latitude(), 0.638369, 5e-4);
  BOOST_CHECK_CLOSE(p9400.longitude(), 153.444, 5e-4);
}

BOOST_AUTO_TEST_CASE(orbit_data)
{
  boost::shared_ptr<QuaternionOrbitData> od = 
    EuropaFixed::orbit_data(galileo_name, sc_frame_name, tm3800);
  BOOST_CHECK_CLOSE(od->position_cf()->latitude(), -2.61439, 5e-4);
  BOOST_CHECK_CLOSE(od->position_cf()->longitude(), -152.651, 5e-4);
  boost::shared_ptr<Dem> dem(new EuropaSimpleDem());
  boost::shared_ptr<RasterImage> 
    img(new VicarLiteRasterImage(test_data_dir() + "3800r.img"));
  // We include rotation about the z axis, called "twist" angle. This
  // is -0.0022797905 radians (from gll360001.ti). We have an extra
  // "-" sign here since the definition of twist goes from what we
  // call spacecraft to frame. The camera constructor takes the
  // quaternion for the other direction.
  // 
  // We also have a small distortion, which we aren't including yet.
  // A quick calculation shows that at the edges the distortion gives 
  // about 0.4 pixel correction, so we probably want to include this.
  // Pixel size and focal length are in mm, and come from gll360001.ti
  // file. We don't bother getting this through spice, instead I just 
  // manually read the file.
  //
  // Note that the camera definition file uses 1 based indexing, where we
  // use 0 based. So things are off by a pixel
  // boost::shared_ptr<Camera> cam(
  //   cam(new QuaternionCamera(quat_rot("z", -(-0.0022797905)),
  // 			     800, 800, 15.24e-3, 15.24e-3, 1501.039,
  // 			     FrameCoordinate(399,399),
  // 			     QuaternionCamera::LINE_IS_Y));
  boost::shared_ptr<Camera> cam(new GalileoCamera());
  OrbitDataImageGroundConnection igc(od, cam, dem, img);
  EuropaPlanetocentric gp(*igc.ground_coordinate(ImageCoordinate(399,399)));
  BOOST_CHECK_CLOSE(gp.latitude(), -15.8989, 1e-3);
  BOOST_CHECK_CLOSE(gp.longitude(), 164.426, 1e-3);
  
  // This is close to what we get from MIPL, which is -7904.05,  21811.58
  // (1 based, so expect to be 1 off from what we would get if we had the
  // idential calculation).
  // 
  // Difference is much larger when we put the nonlinearity correction
  // in for the GalileoCamera (I think MIPL wasn't including this).
  
  VicarImageCoordinate ic_planet_center = 
    igc.image_coordinate(*dem->surface_point(*od->position_cf()));
				// Convert to VicarImageCoordinate, so can
				// more directly compare
  BOOST_CHECK_CLOSE(ic_planet_center.line, -3932.85756, 1e-4);
  BOOST_CHECK_CLOSE(ic_planet_center.sample, 11478.387, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
