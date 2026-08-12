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
  auto lp = boost::make_shared<LocalRcParameter>(*igc);
  auto cconv = boost::make_shared<LocalRcConverter>(lp);
  auto lp_dem = boost::make_shared<LocalZDem>(cconv, 100);
  ImageCoordinate ic_center(igc->number_line() / 2, igc->number_sample() / 2);
  ImageCoordinate ic_p1s(igc->number_line() / 2, igc->number_sample() / 2 + 1);
  ImageCoordinate ic_p1l(igc->number_line() / 2 + 1, igc->number_sample() / 2);
  LocalRectangularCoordinate lc(lp, *igc->ground_coordinate(ic_center));
  LocalRectangularCoordinate lc_expect(lp, 0, 0, 0);
  BOOST_CHECK(GeoCal::distance(lc,lc_expect) < 1e-4);
  BOOST_CHECK(GeoCal::distance(lc,*cconv->convert_from_coordinate(0,0,0)) < 1e-4);
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
  ImageCoordinate ic(0,0);
  double x, y, z;
  auto gp = igc->ground_coordinate_dem(ic, *lp_dem);
  cconv->convert_to_coordinate(*gp, x, y, z);
  BOOST_CHECK_CLOSE(z, lp_dem->z(), 1e-8);
  BOOST_CHECK_CLOSE(gp->height_reference_surface(),
		    lp_dem->height_reference_surface(*gp), 1e-8);
  auto gp2 = cconv->convert_from_coordinate(x, y, 0);
  auto gp3 = lp_dem->surface_point(*gp2);
  BOOST_CHECK(distance(*gp, *gp3) < 1e-3);
}

BOOST_AUTO_TEST_CASE(serialize_coordinate)
{
  if(!have_serialize_supported())
    return;
  auto lp = boost::make_shared<LocalRcParameter>(*igc);
  auto lc = boost::make_shared<LocalRectangularCoordinate>(lp, 0, 1, 2);
  std::string d = serialize_write_string(lc);
  if(false)
    std::cerr << d;
  auto lcr = serialize_read_string<LocalRectangularCoordinate>(d);
  BOOST_CHECK(GeoCal::distance(*lc,*lcr) < 1e-4);
}

BOOST_AUTO_TEST_CASE(serialize_cconv)
{
  if(!have_serialize_supported())
    return;
  auto lp = boost::make_shared<LocalRcParameter>(*igc);
  auto cconv = boost::make_shared<LocalRcConverter>(lp);
  std::string d = serialize_write_string(cconv);
  if(false)
    std::cerr << d;
  auto cconvr = serialize_read_string<LocalRcConverter>(d);
  BOOST_CHECK(GeoCal::distance(*cconv->convert_from_coordinate(0,1,2),
			       *cconvr->convert_from_coordinate(0,1,2))
	      < 1e-4);
}

BOOST_AUTO_TEST_CASE(serialize_dem)
{
  if(!have_serialize_supported())
    return;
  auto lp = boost::make_shared<LocalRcParameter>(*igc);
  auto cconv = boost::make_shared<LocalRcConverter>(lp);
  auto lp_dem = boost::make_shared<LocalZDem>(cconv, 100);
  std::string d = serialize_write_string(lp_dem);
  if(false)
    std::cerr << d;
  auto lp_demr = serialize_read_string<LocalZDem>(d);
  BOOST_CHECK_CLOSE(lp_dem->z(), lp_demr->z(), 1e-8);
  ImageCoordinate ic(0,0);
  double x, y, z;
  auto gp = igc->ground_coordinate_dem(ic, *lp_demr);
  cconv->convert_to_coordinate(*gp, x, y, z);
  BOOST_CHECK_CLOSE(z, lp_dem->z(), 1e-8);
}

BOOST_AUTO_TEST_SUITE_END()
