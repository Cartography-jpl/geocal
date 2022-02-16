#include "unit_test_support.h"
#include "orbit.h"
#include "velocity_aberration_exact.h"
#include "orbit_data_image_ground_connection.h"
#include "simple_dem.h"
#include "constant.h"
#include "ecr.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(velocity_aberration_exact, GlobalFixture)

BOOST_AUTO_TEST_CASE(velocity_aberration_exact)
{
  // Test data we generated in python. We could duplicate the
  // generation in C++, but no real reason to. This comes from
  // test_create_steep_igc in glas_gfm_test.py
  auto igc = serialize_read<OrbitDataImageGroundConnection>
    (test_data_dir() + "steep_igc_gfm.xml");
  
  // A sample point that we got the MSP calculation for
  ImageCoordinate ic(100, 50);	// Not 100% sure about 1/2 pixel
				// differences between GeoCal and MSP
				// image coordinate definition
  Ecr pt_msp(-6157942.08181512, 1596836.45707547, -457258.25167518);

  // Starting point, using our first order aberration correction.
  // This is pretty small, 0.434168 m. But at least as a reference we
  // want our calculation identical to MSP. And for WV-2 like data
  // this is about a pixel
  double initial_distance = distance(*igc->ground_coordinate(ic), pt_msp);
  if(true)
    std::cerr << "Initial distance: " << initial_distance << "\n";

  igc->velocity_aberration(boost::make_shared<VelocityAberrationExact>());
  double corrected_distance = distance(*igc->ground_coordinate(ic), pt_msp);
  if(true)
    std::cerr << "corrected distance: " << corrected_distance << "\n";
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
