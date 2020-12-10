#include "unit_test_support.h"
#include "orbit_des.h"
#include "orbit_data_image_ground_connection.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "constant_raster_image.h"
#include <sstream>

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(orbit_des, GlobalFixture)

BOOST_AUTO_TEST_CASE(pos_csephb)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  PosCsephb p(korb, 1.0);
  BOOST_CHECK_CLOSE(p.min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(p.max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_CLOSE(p.pos_vel(t)(0), -1788501.0, 1e-4);
  BOOST_CHECK_CLOSE(p.pos_vel(t)(1), -6854177.0, 1e-4);
  BOOST_CHECK_CLOSE(p.pos_vel(t)(2), -16811.0, 1e-3);
  std::ostringstream os;
  p.des_write(os);
  if(false)
    std::cerr << os.str() << "\n";
  std::istringstream is(os.str());
  boost::shared_ptr<PosCsephb> p2 = PosCsephb::des_read(is);
  BOOST_CHECK(fabs(p2->min_time() - t) < 1e-6);
  BOOST_CHECK_CLOSE(p2->max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_CLOSE(p2->pos_vel(t)(0), -1788501.0, 1e-4);
  BOOST_CHECK_CLOSE(p2->pos_vel(t)(1), -6854177.0, 1e-4);
  BOOST_CHECK_CLOSE(p2->pos_vel(t)(2), -16811.0, 1e-3);
  BOOST_CHECK_EQUAL(p2->is_cf(), p.is_cf());
  BOOST_CHECK_CLOSE(p2->time_step(), p.time_step(), 1e-4);
  if(false)
    std::cerr << p << "\n"
	      << *p2 << "\n";
}

BOOST_AUTO_TEST_CASE(att_csattb)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  AttCsattb att(korb, 1.0);
  BOOST_CHECK_CLOSE(att.min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(att.max_time() - t, 100.0, 1e-4);
  std::ostringstream os;
  att.des_write(os);
  if(false)
    std::cerr << os.str() << "\n";
  std::istringstream is(os.str());
  boost::shared_ptr<AttCsattb> att2 = AttCsattb::des_read(is);
  BOOST_CHECK(fabs(att2->min_time() - t) < 1e-6);
  BOOST_CHECK_CLOSE(att2->max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_EQUAL(att2->is_cf(), att.is_cf());
  BOOST_CHECK_CLOSE(att2->time_step(), att.time_step(), 1e-4);
  if(false)
    std::cerr << att << "\n"
	      << *att2 << "\n";
}

BOOST_AUTO_TEST_CASE(orbit_des)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  OrbitDes orb(boost::make_shared<PosCsephb>(korb, 1.0, PosCsephb::LAGRANGE,
					     PosCsephb::LAGRANGE_5),
	       boost::make_shared<AttCsattb>(korb, 1.0, AttCsattb::LAGRANGE,
					     AttCsattb::LAGRANGE_3));
  boost::shared_ptr<Camera> cam(new SimpleCamera);
  boost::shared_ptr<Dem> dem(new SimpleDem(100));
  boost::shared_ptr<RasterImage> img(new ConstantRasterImage(cam->number_line(0),
				     cam->number_sample(0), 10));
  ImageCoordinate ic(1.0, 1504 / 2);
  for(Time ti = orb.min_time(); ti < orb.max_time(); ti += 0.5) {
    BOOST_CHECK(GeoCal::distance(*orb.position_cf(ti), *korb.position_cf(ti)) < 1e-2);
    OrbitDataImageGroundConnection igc(orb.orbit_data(ti), cam, dem, img);
    OrbitDataImageGroundConnection kigc(korb.orbit_data(ti), cam, dem, img);
    if(false)
      std::cerr << ti << ": "
		<< GeoCal::distance(*igc.ground_coordinate(ic),
				    *kigc.ground_coordinate(ic)) << "\n";
    BOOST_CHECK(GeoCal::distance(*igc.ground_coordinate(ic),
     				 *kigc.ground_coordinate(ic)) < 1e-2);
  }
  // Check that initially CartesianInertial
  BOOST_CHECK(!orb.pos_csephb()->is_cf());
  BOOST_CHECK(!orb.att_csattb()->is_cf());
  // Should be a noop
  orb.convert_to_ci();
  BOOST_CHECK(!orb.pos_csephb()->is_cf());
  BOOST_CHECK(!orb.att_csattb()->is_cf());
  for(Time ti = orb.min_time(); ti < orb.max_time(); ti += 0.5) {
    BOOST_CHECK(GeoCal::distance(*orb.position_cf(ti), *korb.position_cf(ti)) < 1e-2);
    OrbitDataImageGroundConnection igc(orb.orbit_data(ti), cam, dem, img);
    OrbitDataImageGroundConnection kigc(korb.orbit_data(ti), cam, dem, img);
    if(false)
      std::cerr << ti << ": "
		<< GeoCal::distance(*igc.ground_coordinate(ic),
				    *kigc.ground_coordinate(ic)) << "\n";
    BOOST_CHECK(GeoCal::distance(*igc.ground_coordinate(ic),
     				 *kigc.ground_coordinate(ic)) < 1e-2);
  }

  // Convert to cf
  orb.convert_to_cf();
  BOOST_CHECK(orb.pos_csephb()->is_cf());
  BOOST_CHECK(orb.att_csattb()->is_cf());
  for(Time ti = orb.min_time(); ti < orb.max_time(); ti += 0.5) {
    BOOST_CHECK(GeoCal::distance(*orb.position_cf(ti), *korb.position_cf(ti)) < 1e-2);
    OrbitDataImageGroundConnection igc(orb.orbit_data(ti), cam, dem, img);
    OrbitDataImageGroundConnection kigc(korb.orbit_data(ti), cam, dem, img);
    if(false)
      std::cerr << ti << ": "
		<< GeoCal::distance(*igc.ground_coordinate(ic),
				    *kigc.ground_coordinate(ic)) << "\n";
    BOOST_CHECK(GeoCal::distance(*igc.ground_coordinate(ic),
     				 *kigc.ground_coordinate(ic)) < 1e-2);
  }

  // Convert back
  orb.convert_to_ci();
  BOOST_CHECK(!orb.pos_csephb()->is_cf());
  BOOST_CHECK(!orb.att_csattb()->is_cf());
  for(Time ti = orb.min_time(); ti < orb.max_time(); ti += 0.5) {
    BOOST_CHECK(GeoCal::distance(*orb.position_cf(ti), *korb.position_cf(ti)) < 1e-2);
    OrbitDataImageGroundConnection igc(orb.orbit_data(ti), cam, dem, img);
    OrbitDataImageGroundConnection kigc(korb.orbit_data(ti), cam, dem, img);
    if(false)
      std::cerr << ti << ": "
		<< GeoCal::distance(*igc.ground_coordinate(ic),
				    *kigc.ground_coordinate(ic)) << "\n";
    BOOST_CHECK(GeoCal::distance(*igc.ground_coordinate(ic),
     				 *kigc.ground_coordinate(ic)) < 1e-2);
  }
}

BOOST_AUTO_TEST_CASE(serialization_pos_csephb)
{
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  boost::shared_ptr<PosCsephb> p = boost::make_shared<PosCsephb>(korb, 1.0);
  std::string d = serialize_write_string(p);
  if(false)
    std::cerr << d;
  boost::shared_ptr<PosCsephb> pr = 
    serialize_read_string<PosCsephb>(d);
  BOOST_CHECK_CLOSE(pr->min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(pr->max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_CLOSE(pr->pos_vel(t)(0), -1788501.0, 1e-4);
  BOOST_CHECK_CLOSE(pr->pos_vel(t)(1), -6854177.0, 1e-4);
  BOOST_CHECK_CLOSE(pr->pos_vel(t)(2), -16811.0, 1e-3);
}

BOOST_AUTO_TEST_CASE(serialization_att_csattb)
{
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  boost::shared_ptr<AttCsattb> att = boost::make_shared<AttCsattb>(korb, 1.0);
  std::string d = serialize_write_string(att);
  if(false)
    std::cerr << d;
  boost::shared_ptr<AttCsattb> attr = 
    serialize_read_string<AttCsattb>(d);
  BOOST_CHECK_CLOSE(attr->min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(attr->max_time() - t, 100.0, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization_orbit_des)
{
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  boost::shared_ptr<Orbit> orb =
    boost::make_shared<OrbitDes>(boost::make_shared<PosCsephb>(korb, 1.0),
				 boost::make_shared<AttCsattb>(korb, 1.0));
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Orbit> orbr = 
    serialize_read_string<Orbit>(d);
  BOOST_CHECK_CLOSE(orbr->min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(orbr->max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_CLOSE(orbr->position_ci(t)->position[0], -1788501.0, 1e-4);
  BOOST_CHECK_CLOSE(orbr->position_ci(t)->position[1], -6854177.0, 1e-4);
  BOOST_CHECK_CLOSE(orbr->position_ci(t)->position[2], -16811.0, 1e-3);
}

BOOST_AUTO_TEST_CASE(snip_example)
{
  // Skip normally, depends on hardcoded path to data that isn't
  // generally available. This is really just to sort out NITF
  // conventions, we'll likely remove this test when we are done with it.
  return;
  // Test to sort out orientation etc.
  boost::shared_ptr<OrbitDes> orb_des = 
    serialize_read<OrbitDes>("/home/smyth/Local/SNIP NITF Example/orb_des.xml");
  Time tm = Time::parse_time("2005-04-07T07:24:10Z");
  std::cerr << *orb_des->orbit_data(tm);
}

BOOST_AUTO_TEST_SUITE_END()

