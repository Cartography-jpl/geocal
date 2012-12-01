#include "unit_test_support.h"
#include "srtm_dem.h"
#include <stdlib.h>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(srtm_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!VicarFile::vicar_available())
    return;
  try {
      SrtmDem d;
      // Compared with google earth, using a datum undulation of -33.3435
      BOOST_CHECK_CLOSE(d.height_reference_surface(Geodetic(34.2,-118.03)),
			895.1565, 1e-4);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_SUITE_END()


