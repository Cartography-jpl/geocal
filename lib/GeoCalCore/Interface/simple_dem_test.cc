#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "geocal_serialize_common.h"
#include "unit_test_support.h"
#include "simple_dem.h"
#include "geocal_datum.h"
#include <cmath>

using namespace GeoCal;

BOOST_CLASS_EXPORT(GeoCal::Dem);
BOOST_CLASS_EXPORT(GeoCal::SimpleDem);
BOOST_CLASS_EXPORT(GeoCal::Datum);
BOOST_CLASS_EXPORT(GeoCal::SimpleDatum);

BOOST_FIXTURE_TEST_SUITE(simple_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  SimpleDem d;
  double height1 = 120;
  double height2 = -230;
  Geodetic g1(50, 40, height1);
  Geodetic g2(60, 30, height2);
  BOOST_CHECK_CLOSE(d.distance_to_surface(g1), height1, 1e-4);
  BOOST_CHECK_CLOSE(d.distance_to_surface(g2), height2, 1e-4);
  Geodetic g1expect(50, 40, 0);
  Geodetic g2expect(60, 30, 0);
  BOOST_CHECK(distance(*d.surface_point(g1), g1expect) < 1e-4);
  BOOST_CHECK(distance(*d.surface_point(g2), g2expect) < 1e-4);
  BOOST_CHECK(fabs(d.height_reference_surface(g1) - 0) < 1e-4);
  BOOST_CHECK(fabs(d.height_reference_surface(g2) - 0) < 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  boost::shared_ptr<Dem> d(new SimpleDem());
  oa << GEOCAL_NVP(d);
  if(false)
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<Dem> dr;
  ia >> GEOCAL_NVP(dr);

  double height1 = 120;
  double height2 = -230;
  Geodetic g1(50, 40, height1);
  Geodetic g2(60, 30, height2);
  BOOST_CHECK_CLOSE(dr->distance_to_surface(g1), height1, 1e-4);
  BOOST_CHECK_CLOSE(dr->distance_to_surface(g2), height2, 1e-4);
}


BOOST_AUTO_TEST_CASE(simple_datum)
{
  SimpleDatum datum;
  Geodetic g(60, 30, 100);
  BOOST_CHECK(fabs(datum.undulation(g) - 0) < 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization_datum)
{
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  boost::shared_ptr<Datum> d(new SimpleDatum());
  oa << GEOCAL_NVP(d);
  if(false)
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<Datum> dr;
  ia >> GEOCAL_NVP(dr);

  Geodetic g(60, 30, 100);
  BOOST_CHECK(fabs(dr->undulation(g) - 0) < 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
