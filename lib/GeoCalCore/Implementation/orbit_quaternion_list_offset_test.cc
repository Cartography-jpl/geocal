#include "unit_test_support.h"
#include "orbit_quaternion_list_offset.h"
#include "hdf_orbit.h"
#include "eci_tod.h"
#include "quaternion_camera.h"
#include "geodetic.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;
using namespace blitz;

class OrbitQuaternionListOffsetFixture : public GlobalFixture {
public:
  OrbitQuaternionListOffsetFixture() 
  {
    std::string fname = test_data_dir() + "sample_orbit.h5";
    blitz::Array<double, 1> pos_off(3);
    pos_off = 10, 20, 30;
    orb = boost::make_shared<OrbitQuaternionListOffset>
      (boost::make_shared<HdfOrbit<EciTod, TimeAcsCreator> >(fname),
       pos_off);
    t = Time::time_acs(215077459.472);
  }
  boost::shared_ptr<OrbitQuaternionListOffset> orb;
  Time t;
};

BOOST_FIXTURE_TEST_SUITE(orbit_quaternion_list_offset,
			 OrbitQuaternionListOffsetFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<CartesianFixed> p1 = orb->position_cf(t);
  boost::shared_ptr<CartesianFixed> p2 = orb->orbit_underlying()->position_cf(t);
  BOOST_CHECK_CLOSE(GeoCal::distance(*p1, *p2), sqrt(10*10+20*20+30*30), 1e-4);
  CartesianFixedLookVector lv(p1->position[0] - p2->position[0],
			      p1->position[1] - p2->position[1],
			      p1->position[2] - p2->position[2]);
  ScLookVector slv = orb->sc_look_vector(t, lv);
  BOOST_CHECK_CLOSE(slv.look_vector[0], 10, 1e-3);
  BOOST_CHECK_CLOSE(slv.look_vector[1], 20, 1e-3);
  BOOST_CHECK_CLOSE(slv.look_vector[2], 30, 1e-3);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Orbit> orbr = 
    serialize_read_string<Orbit>(d);
  BOOST_CHECK(fabs(orb->min_time() - orbr->min_time()) < 1e-3);
  BOOST_CHECK(fabs(orb->max_time() - orbr->max_time()) < 1e-3);
}

BOOST_AUTO_TEST_SUITE_END()

