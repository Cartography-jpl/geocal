#include "unit_test_support.h"
#include "simple_dem.h"
#include "geocal_datum.h"
#include <cmath>

using namespace GeoCal;

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
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Dem> dem(new SimpleDem());
  std::string d = serialize_write_string(dem);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Dem> demr = serialize_read_string<Dem>(d);
  double height1 = 120;
  double height2 = -230;
  Geodetic g1(50, 40, height1);
  Geodetic g2(60, 30, height2);
  BOOST_CHECK_CLOSE(demr->distance_to_surface(g1), height1, 1e-4);
  BOOST_CHECK_CLOSE(demr->distance_to_surface(g2), height2, 1e-4);
}


BOOST_AUTO_TEST_CASE(simple_datum)
{
  SimpleDatum datum;
  Geodetic g(60, 30, 100);
  BOOST_CHECK(fabs(datum.undulation(g) - 0) < 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization_datum)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Datum> datum(new SimpleDatum());
  std::string d = serialize_write_string(datum);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Datum> datumr = serialize_read_string<Datum>(d);
  Geodetic g(60, 30, 100);
  BOOST_CHECK(fabs(datumr->undulation(g) - 0) < 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
