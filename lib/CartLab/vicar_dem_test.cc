#include "unit_test_support.h"
#include "vicar_dem.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(vicar_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!VicarFile::vicar_available())
    return;
  std::string fname = test_data_dir() + "vicar.img";
  VicarDem d(fname);
  Geodetic g1(35.9, 44.800, 100);
  BOOST_CHECK_CLOSE(d.height_reference_surface(g1), 1.0, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  if(!VicarFile::vicar_available())
    return;
  std::string fname = test_data_dir() + "vicar.img";
  boost::shared_ptr<VicarDem> f(new VicarDem(fname));
  std::string d = serialize_write_string(f);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<VicarDem> fr =
    serialize_read_string<VicarDem>(d);
  Geodetic g1(35.9, 44.800, 100);
  BOOST_CHECK_CLOSE(fr->height_reference_surface(g1), 1.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
