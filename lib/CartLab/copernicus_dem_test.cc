#include "unit_test_support.h"
#include "copernicus_dem.h"
#include <stdlib.h>
#include <boost/filesystem.hpp>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(copernicus_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!VicarFile::vicar_available())
    return;
  try {
      CopernicusDem d;
      // Compared with google earth, using a datum undulation of -33.3435
      BOOST_CHECK_CLOSE(d.height_reference_surface(Geodetic(34.2,-118.03)),
			889.1565, 1e-4);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping CopernicusDem test, data wasn't found");
    // Don't worry if we can't find the data.
  } catch(const boost::filesystem::filesystem_error&) {
    BOOST_WARN_MESSAGE(false, "Skipping CopernicusDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_CASE(near_dateline)
{
  if(!VicarFile::vicar_available())
    return;
  try {
    CopernicusDem d;
    BOOST_CHECK_CLOSE(d.height_reference_surface(Geodetic(70,179.99)),
		      2.56005, 1e-4);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping CopernicusDem test, data wasn't found");
    // Don't worry if we can't find the data.
  } catch(const boost::filesystem::filesystem_error&) {
    BOOST_WARN_MESSAGE(false, "Skipping CopernicusDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_CASE(cache_problem)
{
  if(!VicarFile::vicar_available())
    return;
  try {
    // We found an issue with the no coverage caching. This was
    // extracted from a specific ECOSTRESS problem, but we tracked it
    // back to this.
    CopernicusDem dem("", false);

    // On land
    Geodetic pt1(50.51221665841272, -4.222361227183813);
    // In water, with no tile
    Geodetic pt2(49.9907, -4.31486);

    // This works, because we already have the tile we need
    double h = dem.height_datum(pt1);
    BOOST_CHECK_CLOSE(h, 144.92176989114103, 1e-4);
    h = dem.height_datum(pt2);
    BOOST_CHECK_CLOSE(h, 0, 1e-4);
    h = dem.height_datum(pt1);
    BOOST_CHECK_CLOSE(h, 144.92176989114103, 1e-4);

    // This originally didn't work.
    CopernicusDem dem2("", false);
    h = dem2.height_datum(pt2);
    BOOST_CHECK_CLOSE(h, 0, 1e-4);
    h = dem2.height_datum(pt1);
    BOOST_CHECK_CLOSE(h, 144.92176989114103, 1e-4);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping CopernicusDem test, data wasn't found");
    // Don't worry if we can't find the data.
  } catch(const boost::filesystem::filesystem_error&) {
    BOOST_WARN_MESSAGE(false, "Skipping CopernicusDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported() || !VicarFile::vicar_available())
    return;
  boost::shared_ptr<Dem> dem;
  try {
    dem.reset(new CopernicusDem());
    dem->height_reference_surface(Geodetic(34.2,-118.03));
  } catch(const Exception&) {
    // Don't worry if we can't find the data, just skip test.
    BOOST_WARN_MESSAGE(false, "Skipping CopernicusDem test, data wasn't found");
    return;
  } catch(const boost::filesystem::filesystem_error&) {
    BOOST_WARN_MESSAGE(false, "Skipping CopernicusDem test, data wasn't found");
    return;
  } 
  std::string d = serialize_write_string(dem);
  if(false)
    std::cerr << d;
  boost::shared_ptr<CopernicusDem> demr = 
    serialize_read_string<CopernicusDem>(d);

  BOOST_CHECK_CLOSE(demr->height_reference_surface(Geodetic(34.2,-118.03)),
		    889.1565, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()


