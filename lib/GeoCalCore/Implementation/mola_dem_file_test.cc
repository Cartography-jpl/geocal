#include "unit_test_support.h"
#include "mola_dem_file.h"
#include "planet_coordinate.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mola_dem_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  try {
    MolaDemFile d("/raid26/marsdem/meg128/megr44n270hb.lbl");
    Planetocentric gp(22.1074, -19.9284, 0, PlanetConstant::MARS_NAIF_CODE);
    BOOST_CHECK_CLOSE(d.height_reference_surface(gp), -2617.99, 1e-4);
  } catch(const Exception&) {
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_SUITE_END()
