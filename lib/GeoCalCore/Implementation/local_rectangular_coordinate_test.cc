#include "unit_test_support.h"
#include "local_rectangular_coordinate.h"
#include "rsm_fixture.h"
#include "vicar_file.h"
#include "srtm_dem.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(local_rectangular_coordinate, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  boost::shared_ptr<LocalRcParameter> lp =
    boost::make_shared<LocalRcParameter>(*igc);
  LocalRcConverter cconv(lp);
  ImageCoordinate ic_center(igc->number_line() / 2, igc->number_sample() / 2);
  ImageCoordinate ic_p1s(igc->number_line() / 2, igc->number_sample() / 2 + 1);
  ImageCoordinate ic_p1l(igc->number_line() / 2 + 1, igc->number_sample() / 2);
  LocalRectangularCoordinate lc(lp, *igc->ground_coordinate(ic_center));
  LocalRectangularCoordinate lc_expect(lp, 0, 0, 0);
  BOOST_CHECK(GeoCal::distance(lc,lc_expect) < 1e-4);
  BOOST_CHECK(GeoCal::distance(lc,*cconv.convert_from_coordinate(0,0,0)) < 1e-4);
  LocalRectangularCoordinate lc2(lp, *igc->ground_coordinate_approx_height(ic_center, 10));
  LocalRectangularCoordinate lc2_expect(lp, 0, 0, 11.6381);
  BOOST_CHECK(GeoCal::distance(lc2,lc2_expect) < 1e-4);
  LocalRectangularCoordinate lc3(lp, *igc->ground_coordinate(ic_p1s));
  LocalRectangularCoordinate lc4(lp, *igc->ground_coordinate(ic_p1l));
  // lc3 should be mostly in x direction, with some in z because it
  // isn't orthogonal to z. Check that y is close to zero
  BOOST_CHECK(fabs(lc3.position[1]) < 0.001);
  // lc4 should be mostly in y direction.
  BOOST_CHECK(fabs(lc4.position[0]) < 0.001);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<LocalRcParameter> lp =
    boost::make_shared<LocalRcParameter>(*igc);
  boost::shared_ptr<LocalRectangularCoordinate> lc =
    boost::make_shared<LocalRectangularCoordinate>(lp, 0, 1, 2);
  std::string d = serialize_write_string(lc);
  if(false)
    std::cerr << d;
  boost::shared_ptr<LocalRectangularCoordinate> lcr = 
    serialize_read_string<LocalRectangularCoordinate>(d);
  BOOST_CHECK(GeoCal::distance(*lc,*lcr) < 1e-4);
}

BOOST_AUTO_TEST_CASE(serialize2)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<LocalRcParameter> lp =
    boost::make_shared<LocalRcParameter>(*igc);
  boost::shared_ptr<LocalRcConverter> cconv =
    boost::make_shared<LocalRcConverter>(lp);
  std::string d = serialize_write_string(cconv);
  if(false)
    std::cerr << d;
  boost::shared_ptr<LocalRcConverter> cconvr = 
    serialize_read_string<LocalRcConverter>(d);
  BOOST_CHECK(GeoCal::distance(*cconv->convert_from_coordinate(0,1,2),
			       *cconvr->convert_from_coordinate(0,1,2))
	      < 1e-4);
}
BOOST_AUTO_TEST_SUITE_END()
