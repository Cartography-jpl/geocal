#include "unit_test_support.h"
#include "srtm_dem.h"
#include <stdlib.h>
#include <boost/filesystem.hpp>
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
			888.656, 1e-4);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    // Don't worry if we can't find the data.
  } catch(const boost::filesystem::filesystem_error&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported() || !VicarFile::vicar_available())
    return;
  boost::shared_ptr<Dem> dem;
  try {
    dem.reset(new SrtmDem());
    dem->height_reference_surface(Geodetic(34.2,-118.03));
  } catch(const Exception&) {
    // Don't worry if we can't find the data, just skip test.
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    return;
  } catch(const boost::filesystem::filesystem_error&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    return;
  } 
  std::string d = serialize_write_string(dem);
  if(false)
    std::cerr << d;
  boost::shared_ptr<SrtmDem> demr = 
    serialize_read_string<SrtmDem>(d);

  BOOST_CHECK_CLOSE(demr->height_reference_surface(Geodetic(34.2,-118.03)),
		    888.656, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()


