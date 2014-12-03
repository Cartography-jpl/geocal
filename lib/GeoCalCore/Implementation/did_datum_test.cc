#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "geocal_serialize_common.h"
#include "unit_test_support.h"
#include "did_datum.h"
#include "geodetic.h"

using namespace GeoCal;
BOOST_CLASS_EXPORT(GeoCal::Datum);
BOOST_CLASS_EXPORT(GeoCal::DidDatum);

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

BOOST_AUTO_TEST_CASE(serialization)
{
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  boost::shared_ptr<Datum> d;
  try {
    d.reset(new DidDatum("/data/bank/anc/DID/MeanSeaLevel/current/geoid_2160h_4320w.half"));
  } catch(const Exception&) {
    // Don't worry if we can't find the data.
  }
  if(!d)
    return;
  oa << BOOST_SERIALIZATION_NVP(d);
  if(false)
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<Datum> dr;
  ia >> BOOST_SERIALIZATION_NVP(dr);

  BOOST_CHECK_CLOSE(dr->undulation(Geodetic(40, -100, 0)), -25.0, 1e-4);
  BOOST_CHECK_CLOSE(dr->undulation(Geodetic(40, -89.9, 0)), -34.0, 1e-4);
  BOOST_CHECK_CLOSE(dr->undulation(Geodetic(40, -79.9, 0)), -33.0, 1e-4);
  BOOST_CHECK_CLOSE(dr->undulation(Geodetic(40, -69.9, 0)), -34.0, 1e-4);
  BOOST_CHECK_CLOSE(dr->undulation(Geodetic(40, -59.9, 0)), -25.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
