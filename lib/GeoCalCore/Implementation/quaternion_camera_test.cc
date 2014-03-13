#include "unit_test_support.h"
#include "quaternion_camera.h"
#include "ecr.h"
#include "geodetic.h"
#include "eci_tod.h"
#include "orbit.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(quaternion_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  // u sample, v line.
  // Output is longitude, latitude, height, line, sample
  QuaternionCamera cam(boost::math::quaternion<double>(1,0,0,0),
		       3375, 3648,
		       1.0 / 2500000,
		       1.0 / 2500000,
		       1.0,
		       FrameCoordinate(1688.0, 1824.5),
		       QuaternionCamera::LINE_IS_Y);
  BOOST_CHECK_CLOSE(cam.focal_length(), 1.0, 1e-4);
  BOOST_CHECK_CLOSE(cam.line_pitch(),   1.0 / 2500000, 1e-4);
  BOOST_CHECK_CLOSE(cam.sample_pitch(), 1.0 / 2500000, 1e-4);
  BOOST_CHECK_CLOSE(cam.principal_point(0).line, 1688, 1e-4);
  BOOST_CHECK_CLOSE(cam.principal_point(0).sample, 1824.5, 1e-4);
  BOOST_CHECK_EQUAL(cam.number_line(0), 3375);
  BOOST_CHECK_EQUAL(cam.number_sample(0), 3648);
  BOOST_CHECK_EQUAL(cam.number_band(), 1);
}

// This attempts to match the results of the sc2rpc unit tests.
// **NOTE** We get results that are about 7 meters different than what
// sc2rpc gets. This was tracked down to what I believe is a more
// accurate calculation the spice toolkit for J2000 to ECEF.  We allow
// a 10 meter error in comparing to sc2rpc.

BOOST_AUTO_TEST_CASE(sc2rpc)
{
  Time t = Time::time_acs(215077459.472);
  boost::shared_ptr<EciTod> eci_tod
    (new EciTod(3435100.496, 945571.538, -6053387.573));
  boost::array<double, 3> vel = {{0, 0, 0}};
  boost::math::quaternion<double> sc_to_ci(0.946366, 0.0, -0.323096813, 0.0);
  QuaternionOrbitData od(t, eci_tod, vel, sc_to_ci);
  QuaternionCamera cam(boost::math::quaternion<double>(1,0,0,0),
		       3375, 3648,
		       1.0 / 2500000,
		       1.0 / 2500000,
		       1.0,
		       FrameCoordinate(1688.0, 1824.5),
		       QuaternionCamera::LINE_IS_Y);
  boost::shared_ptr<CartesianFixed> pt =
    od.reference_surface_intersect_approximate(cam, FrameCoordinate(3375, 3648));
  Ecr pt_ecr(*pt);
  Geodetic pt_geod(-60.3162137, 47.2465154);
  BOOST_CHECK(distance(pt_geod, *pt) < 10.0);
  //  std::cerr << distance(pt_geod, *pt) << "\n";
  pt = od.reference_surface_intersect_approximate(cam, 
			  FrameCoordinate(1, 3648), 0, 200);
  pt_geod = Geodetic(-60.3179857, 47.2296261, 199.9997909);
  BOOST_CHECK(distance(pt_geod, *pt) < 10.0);
  //  std::cerr << distance(pt_geod, *pt) << "\n";
  pt = od.reference_surface_intersect_approximate(cam, 
			  FrameCoordinate(3375, 1), 0, 100);
  pt_geod = Geodetic(-60.3247623, 47.2509243, 99.9998955);
  BOOST_CHECK(distance(pt_geod, *pt) < 10.0);
  //  std::cerr << distance(pt_geod, *pt) << "\n";
}

BOOST_AUTO_TEST_CASE(compare_simple_camera)
{
  // Make sure we duplicate what a SimpleCamera generates.
  SimpleCamera cam1;
  QuaternionCamera cam2(quat_rot_z(cam1.epsilon()) *
			quat_rot_y(cam1.beta()) *
			quat_rot_x(cam1.delta()),
			cam1.number_line(0), cam1.number_sample(0), 
			cam1.line_pitch() / 1e-3, 
			cam1.sample_pitch() / 1e-3,
			cam1.focal_length() / 1e-3,
			FrameCoordinate(cam1.number_line(0) / 2.0, 
					cam1.number_sample(0) / 2.0));
  FrameCoordinate fc(0,0);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).sample -  fc.sample) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
					 0).sample -  fc.sample) < 1e-6);
  fc = FrameCoordinate(10,0);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).sample -  fc.sample) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
					 0).sample -  fc.sample) < 1e-6);
  fc = FrameCoordinate(0,1504);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).sample -  fc.sample) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
					 0).sample -  fc.sample) < 1e-6);
  fc = FrameCoordinate(10,1504);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).sample -  fc.sample) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
					 0).sample -  fc.sample) < 1e-6);
  fc = FrameCoordinate(-10,1504);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).sample -  fc.sample) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
					 0).sample -  fc.sample) < 1e-6);
  fc = FrameCoordinate(-10,-1504);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam2.frame_coordinate(cam1.sc_look_vector(fc, 0), 
	     			       0).sample -  fc.sample) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam1.frame_coordinate(cam2.sc_look_vector(fc, 0), 
					 0).sample -  fc.sample) < 1e-6);
}

BOOST_AUTO_TEST_SUITE_END()

