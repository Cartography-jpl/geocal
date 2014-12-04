#include "unit_test_support.h"
#include "gdal_datum.h"
#include "geodetic.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(gdal_datum, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  std::string fname = test_data_dir() + "egm96.img";
  GdalDatum datum(fname);
  BOOST_CHECK_CLOSE(datum.undulation(Geodetic(40, -100, 0)), -25.0, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_BOOST_SERIALIZATON
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  std::string fname = test_data_dir() + "egm96.img";
  boost::shared_ptr<Datum> d(new GdalDatum(fname));
  oa << GEOCAL_NVP(d);
  if(false)
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<Datum> dr;
  ia >> GEOCAL_NVP(dr);

  BOOST_CHECK_CLOSE(dr->undulation(Geodetic(40, -100, 0)), -25.0, 1e-4);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
