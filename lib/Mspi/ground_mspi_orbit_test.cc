#include "ground_mspi_orbit.h"
#include "unit_test_support.h"
#include "geodetic.h"
#include <iostream>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(ground_mspi_orbit, GlobalFixture)


BOOST_AUTO_TEST_CASE(basic_test)
{
  Time t = Time::parse_time("1996-07-03T04:13:57.987654Z") + 10;
  boost::shared_ptr<GroundCoordinate> p(new Geodetic(10, 20, 30));
  GroundMspiOrbit orb(t, p, 40, 50, 1.7);
  CartesianInertialLookVector lv = 
    orb.ci_look_vector(t, ScLookVector(0.1,0.2,0.3));
  // From the old unit test.
  CartesianInertialLookVector lv_exp(0.766628,-0.6288180,0.12987982709088683);
  for(int i = 0; i < 3; ++i)
    BOOST_CHECK_CLOSE(lv.direction()[i], lv_exp.direction()[i], 1e-4);
  lv = orb.ci_look_vector(t+100, ScLookVector(0.1,0.2,0.3));
  BOOST_CHECK_CLOSE(lv.direction()[0], -0.851217, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[1], -0.29221994757447994, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[2], 0.435934, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("1996-07-03T04:13:57.987654Z") + 10;
  boost::shared_ptr<GroundCoordinate> p(new Geodetic(10, 20, 30));
  boost::shared_ptr<Orbit> orb(new GroundMspiOrbit(t, p, 40, 50, 1.7));
  CartesianInertialLookVector lv = 
    orb->ci_look_vector(t, ScLookVector(0.1,0.2,0.3));
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<GroundMspiOrbit> orbr = 
    serialize_read_string<GroundMspiOrbit>(d);
  CartesianInertialLookVector lv2 = 
    orb->ci_look_vector(t, ScLookVector(0.1,0.2,0.3));
  for(int i = 0; i < 3; ++i)
    BOOST_CHECK_CLOSE(lv.direction()[i], lv2.direction()[i], 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
