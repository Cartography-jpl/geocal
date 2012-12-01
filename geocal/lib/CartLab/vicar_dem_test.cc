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

BOOST_AUTO_TEST_SUITE_END()
