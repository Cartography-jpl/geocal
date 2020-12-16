#include "unit_test_support.h"
#include "trex_dem.h"
#include <stdlib.h>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(trex_dem, GlobalFixture)

// Note, we don't have the environment variable yet in AFIDS. The
// location in the cartlab is
// /bigdata/tllogan30/trex_D4_update/trex_L3_lwm_D4
// and /bigdata/tllogan30/trex_D4_update/trex_L3_dem_D4b

BOOST_AUTO_TEST_CASE(basic_test)
{

  if(!VicarFile::vicar_available())
    return;
  try {
      TrexDem d;
      // Compared with google earth, using a datum undulation of
      // -33.3435.
      // Google Earth is 926 m. Add undulation to get
      // 892.6565. Google earth is height above sea level
      BOOST_CHECK_CLOSE(d.height_reference_surface(Geodetic(34.2,-118.03)),
			891.6565, 1e-4);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping TrexDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_CASE(lwm_test)
{

  if(!VicarFile::vicar_available())
    return;
  try {
    // Not working yet. We'll come back to this
    TrexLwmData d;
    std::cerr << d.coordinate(Geodetic(34.2,-118.03)) << "\n";
    std::cerr << d(448200, 557730) << "\n";
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping TrexLwmData test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported() || !VicarFile::vicar_available())
    return;
  boost::shared_ptr<Dem> dem;
  try {
    dem.reset(new TrexDem());
    dem->height_reference_surface(Geodetic(34.2,-118.03));
  } catch(const Exception&) {
    // Don't worry if we can't find the data, just skip test.
    BOOST_WARN_MESSAGE(false, "Skipping TrexDem test, data wasn't found");
    return;
  } 
  std::string d = serialize_write_string(dem);
  if(false)
    std::cerr << d;
  boost::shared_ptr<TrexDem> demr = 
    serialize_read_string<TrexDem>(d);

  BOOST_CHECK_CLOSE(demr->height_reference_surface(Geodetic(34.2,-118.03)),
		    891.6565, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()


