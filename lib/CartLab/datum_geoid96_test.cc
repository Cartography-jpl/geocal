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

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!VicarFile::vicar_available())
    return;
  if(!have_serialize_supported())
    return;

  boost::shared_ptr<Datum> datum;
  try {
    datum.reset(new DatumGeoid96());
  } catch(const Exception&) {
    // Don't worry if we can't find the data.
  }
  if(!datum)
    return;
  std::string d = serialize_write_string(datum);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Datum> datumr = 
    serialize_read_string<Datum>(d);

  BOOST_CHECK_CLOSE(datumr->undulation(Geodetic(34.2,-118.03)), -33.3435, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()


