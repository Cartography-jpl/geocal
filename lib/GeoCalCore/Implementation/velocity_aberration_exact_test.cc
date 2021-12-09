#include "unit_test_support.h"
#include "orbit.h"
#include "velocity_aberration_exact.h"
#include "orbit_data_image_ground_connection.h"
#include "simple_dem.h"
#include "constant.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(velocity_aberration_exact, GlobalFixture)

BOOST_AUTO_TEST_CASE(velocity_aberration_exact)
{
  // Test data we generated in python. We could duplicate the
  // generation in C++, but no real reason to. This comes from
  // test_create_steep_igc in glas_gfm_test.py
  auto igc = serialize_read<OrbitDataImageGroundConnection>
    (test_data_dir() + "steep_igc_gfm.xml");
  std::cerr << *igc << "\n";
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<VelocityAberrationExact> a =
    boost::make_shared<VelocityAberrationExact>();
  std::string d = serialize_write_string(a);
  if(false)
    std::cerr << d;
  boost::shared_ptr<VelocityAberrationExact> ar = 
    serialize_read_string<VelocityAberrationExact>(d);
}

BOOST_AUTO_TEST_SUITE_END()
