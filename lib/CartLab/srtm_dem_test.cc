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
			888.656, 1e-4);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_BOOST_SERIALIZATON
  if(!VicarFile::vicar_available())
    return;
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  boost::shared_ptr<Dem> d(new SrtmDem());
  oa << GEOCAL_NVP(d);
  if(false)
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<Dem> dr;
  ia >> GEOCAL_NVP(dr);

  BOOST_CHECK_CLOSE(dr->height_reference_surface(Geodetic(34.2,-118.03)),
		    888.656, 1e-4);
#endif
}

BOOST_AUTO_TEST_SUITE_END()


