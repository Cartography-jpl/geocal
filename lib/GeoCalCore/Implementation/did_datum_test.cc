#include "unit_test_support.h"
#include "did_datum.h"
#include "geodetic.h"

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(did_datum, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  try {
    DidDatum datum("/data/bank/anc/DID/MeanSeaLevel/current/geoid_2160h_4320w.half");
    BOOST_CHECK_CLOSE(datum.undulation(Geodetic(40, -100, 0)), -25.0, 1e-4);
    BOOST_CHECK_CLOSE(datum.undulation(Geodetic(40, -89.9, 0)), -34.0, 1e-4);
    BOOST_CHECK_CLOSE(datum.undulation(Geodetic(40, -79.9, 0)), -33.0, 1e-4);
    BOOST_CHECK_CLOSE(datum.undulation(Geodetic(40, -69.9, 0)), -34.0, 1e-4);
    BOOST_CHECK_CLOSE(datum.undulation(Geodetic(40, -59.9, 0)), -25.0, 1e-4);
  } catch(const Exception&) {
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_SUITE_END()
