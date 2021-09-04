#include "unit_test_support.h"
#include "rsm.h"
#include "rsm_fixture.h"
#include "vicar_file.h"
#include "srtm_dem.h"
#include "rsm_rp_plus_grid.h"
#include "rsm_rational_polynomial.h"
#include "local_rectangular_coordinate.h"
#include "spice_helper.h"
#include "planet_coordinate.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(rsm, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  Rsm r(rp_from_rpc, cconv);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic;
  bool in_valid_range;
  r.image_coordinate(gp, ic, in_valid_range);
  BOOST_CHECK(in_valid_range);
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
  boost::shared_ptr<GroundCoordinate> gpcalc =
    r.ground_coordinate_z(ic_expect, gp.height_reference_surface());
  BOOST_CHECK(distance(gp, *gpcalc) < 1.0);
}

BOOST_AUTO_TEST_CASE(fit_test)
{
  double hmin = rpc.height_offset - rpc.height_scale;
  double hmax = rpc.height_offset + rpc.height_scale;
  Rsm r(boost::make_shared<RsmRationalPolynomial>(3,3,3,3,3,3,3,3), cconv);
  r.fit(*igc, hmin, hmax);
  boost::shared_ptr<GroundCoordinate> gc = rpc.ground_coordinate(ImageCoordinate(1000, 1000), 1017);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic;
  bool in_valid_range;
  r.image_coordinate(gp, ic, in_valid_range);
  // std::cerr << r.rsm_base()->min_line() << "\n"
  // 	    << r.rsm_base()->max_line() << "\n"
  // 	    << r.rsm_base()->min_sample() << "\n"
  // 	    << r.rsm_base()->max_sample() << "\n"
  // 	    << r.rsm_base()->min_x() << "\n"
  // 	    << r.rsm_base()->max_x() << "\n"
  // 	    << r.rsm_base()->min_y() << "\n"
  // 	    << r.rsm_base()->max_y() << "\n"
  // 	    << r.rsm_base()->min_z() << "\n"
  // 	    << r.rsm_base()->max_z() << "\n";
  BOOST_CHECK(in_valid_range);
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
  boost::shared_ptr<GroundCoordinate> gpcalc =
    r.ground_coordinate_z(ic_expect, gp.height_reference_surface());
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
    ImageCoordinate ic;
    bool in_valid_range;
    r.image_coordinate(*gpsurf, ic, in_valid_range);
    BOOST_CHECK(in_valid_range);
    boost::shared_ptr<GroundCoordinate> gpcalc =
      r.ground_coordinate(ic, d);
    BOOST_CHECK(GeoCal::distance(*gpsurf, *gpcalc) < 1.0);
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    // Don't worry if we can't find the data.
  } catch(const std::exception&) {
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
  ImageCoordinate ic;
  bool in_valid_range;
  rr->image_coordinate(gp, ic, in_valid_range);
  BOOST_CHECK(in_valid_range);
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
}

BOOST_AUTO_TEST_CASE(mars_example)
{
// Don't normally run, this depends on specific test data we have on
// python system
  return;
  boost::shared_ptr<ImageGroundConnection> igc =
    serialize_read<ImageGroundConnection>("/home/smyth/Local/MarsRsm/ctx1_full_igc.xml");
  Rsm r(boost::make_shared<RsmRpPlusGrid>
	(boost::shared_ptr<RsmRationalPolynomial>(new RsmRationalPolynomial(5,5,3,1,1,1,5,1,40,40,20,40)),
	 boost::make_shared<RsmGrid>(3, 1000, 3)),
	boost::make_shared<LocalRcConverter>
	(boost::make_shared<LocalRcParameter>(*igc)));
  r.fit(*igc, 0, 1000);
}

BOOST_AUTO_TEST_CASE(mars_example2)
{
// Don't normally run, this depends on specific test data we have on
// python system
  return;
  boost::shared_ptr<ImageGroundConnection> igc =
    serialize_read<ImageGroundConnection>("/home/smyth/Local/MarsRsm/hrsc1_full_igc.xml");
  boost::shared_ptr<Rsm> r1 = boost::make_shared<Rsm>
    (boost::shared_ptr<RsmRationalPolynomial>(new RsmRationalPolynomial(5,5,3,0,0,0,5,1,40,40,20,40)),
     boost::make_shared<LocalRcConverter>
     (boost::make_shared<LocalRcParameter>(*igc)));
  boost::shared_ptr<Rsm> r2 = boost::make_shared<Rsm>
    (boost::make_shared<RsmRpPlusGrid>
	(boost::shared_ptr<RsmRationalPolynomial>(new RsmRationalPolynomial(5,5,3,1,1,1,5,1,40,40,20,40)),
	 boost::make_shared<RsmGrid>(3, 1000, 50)),
	boost::make_shared<LocalRcConverter>
     (boost::make_shared<LocalRcParameter>(*igc)));
  boost::shared_ptr<Rsm> r = r1;
  r->fit(*igc, -5000, -1500);
  Array<double, 2> ln, smp, lncalc, smpcalc, distance_true_vs_calc;
  r->compare_igc(*igc, igc->number_line(), 5, -3000, ln, smp, lncalc, smpcalc,
		 distance_true_vs_calc);
  std::cerr << max(abs(ln-lncalc)) << "\n";
  std::string d = serialize_write_string(r);
  if(true)
    std::cerr << d;
}

BOOST_AUTO_TEST_CASE(mars_example3)
{
// Don't normally run, this depends on specific test data we have on
// python system
  return;
  if(!SpiceHelper::spice_available())
    return;
// Get FOV information, and compare with what we calculate
  boost::shared_ptr<ImageGroundConnection> igc =
    serialize_read<ImageGroundConnection>("/home/smyth/Local/MarsRsm/hrsc1_full_igc.xml");
  ImageCoordinate ic(200, igc->number_sample() / 2.0);
  std::vector<boost::shared_ptr<GroundCoordinate> > pt =
    SpiceHelper::boresight_and_footprint
    (igc->pixel_time(ic), PlanetConstant::MARS_NAIF_CODE, "MEX",
     "MEX_HRSC_NADIR");
  std::cerr << *pt[0] << "\n"
	    << GeoCal::distance(*pt[0], *igc->ground_coordinate(ic)) << "\n"
	    << igc->image_coordinate(*pt[0]) << "\n";
}

BOOST_AUTO_TEST_SUITE_END()
