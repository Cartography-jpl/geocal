#include "unit_test_support.h"
#include "glas_gfm_camera.h"
#include <iostream>
#include <boost/make_shared.hpp>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(glas_grm_camera, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  // Dummy Test
  GlasGfmCamera cam();
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Camera> cam(new GlasGfmCamera());
  std::string d = serialize_write_string(cam);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Camera> camr = serialize_read_string<Camera>(d);
}

BOOST_AUTO_TEST_SUITE_END()
