#include "unit_test_support.h"
#include "igc_array.h"
#include "rpc_image_ground_connection.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(igc_array, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
}

BOOST_AUTO_TEST_CASE(serialization)
{
  // if(!have_serialize_supported())
  //   return;
  // std::string d = serialize_write_string(igc);
  // if(true)
  //   std::cerr << d;
  // boost::shared_ptr<IgcRollingShutter> igcr = 
  //   serialize_read_string<IgcRollingShutter>(d);
  // ImageCoordinate ic(100, 200);
  // BOOST_CHECK(igcr->image_coordinate(*igc->ground_coordinate(ic)) == ic);
  // BOOST_CHECK_MATRIX_CLOSE(igc->parameter(), igcr->parameter());
}

BOOST_AUTO_TEST_SUITE_END()

