#include "unit_test_support.h"
#include "datum_geoid96.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(datum_geoid96, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!VicarFile::vicar_available())
    return;
  try {
    DatumGeoid96 d;
    // We compare this the geoid calculation found at 
    // http://earth-info.nga.mil/nga-bin/gandg-bin/intpt.cgi
    BOOST_CHECK_CLOSE(d.undulation(Geodetic(34.2,-118.03)), -33.3435, 1e-4);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping DatumGeoid96 test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_SUITE_END()


