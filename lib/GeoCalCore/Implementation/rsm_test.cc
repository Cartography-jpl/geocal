#include "unit_test_support.h"
#include "rsm.h"
#include "rsm_fixture.h"
#include "vicar_file.h"
#include "srtm_dem.h"
#include "rsm_rp_plus_grid.h"
#include "rsm_rational_polynomial.h"
#include "local_rectangular_coordinate.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(rsm, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  Rsm r(rp_from_rpc, cconv);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp);
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
  boost::shared_ptr<GroundCoordinate> gpcalc =
    r.ground_coordinate(ic_expect, gp.height_reference_surface());
  BOOST_CHECK(distance(gp, *gpcalc) < 1.0);
}

BOOST_AUTO_TEST_CASE(fit_test)
{
  double hmin = rpc.height_offset - rpc.height_scale;
  double hmax = rpc.height_offset + rpc.height_scale;
  Rsm r(boost::make_shared<RsmRationalPolynomial>(3,3,3,3,3,3,3,3), cconv);
  r.fit(*igc, hmin, hmax);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp);
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
  boost::shared_ptr<GroundCoordinate> gpcalc =
    r.ground_coordinate(ic_expect, gp.height_reference_surface());
  BOOST_CHECK(distance(gp, *gpcalc) < 1.0);
}

BOOST_AUTO_TEST_CASE(ground_coordinate_dem)
{
  Rsm r(rp_from_rpc, cconv);
  // Need to have SrtmDem available for doing this test.
  if(!VicarFile::vicar_available())
    return;
  try {
    SrtmDem d;
    boost::shared_ptr<GroundCoordinate> gpsurf = d.surface_point(gp);
    ImageCoordinate ic = r.image_coordinate(*gpsurf);
    boost::shared_ptr<GroundCoordinate> gpcalc =
      r.ground_coordinate(ic, d);
    BOOST_CHECK(GeoCal::distance(*gpsurf, *gpcalc) < 1.0);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Rsm> r = boost::make_shared<Rsm>(rp_from_rpc,cconv);
  std::string d = serialize_write_string(r);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Rsm> rr = 
    serialize_read_string<Rsm>(d);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = rr->image_coordinate(gp);
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
}

BOOST_AUTO_TEST_CASE(mars_example)
{
// Don't normally run, this depends on specific test data we have on
// the system
  //return;
  boost::shared_ptr<ImageGroundConnection> igc =
    serialize_read<ImageGroundConnection>("/home/smyth/Local/MarsRsm/ctx1_igc.xml");
  Rsm r(boost::make_shared<RsmRpPlusGrid>
	(boost::shared_ptr<RsmRationalPolynomial>(new RsmRationalPolynomial(5,5,3,1,1,1,5,1,40,40,20,40)),
	 boost::make_shared<RsmGrid>(3, 1000, 3)),
	boost::make_shared<LocalRcConverter>
	(boost::make_shared<LocalRcParameter>(*igc)));
  r.fit(*igc, 0, 1000);
}
					  
    

BOOST_AUTO_TEST_SUITE_END()
