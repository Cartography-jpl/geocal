#include "unit_test_support.h"
#include "rsm_adjustable_parameter_a.h"
#include "rsm.h"
#include "rsm_image_ground_connection.h"
#include "rsm_fixture.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_adjustable_parameter_a, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  Rsm r(rp_from_rpc, cconv);
  boost::shared_ptr<RsmAdjustableParameterA> adj = boost::make_shared<RsmAdjustableParameterA>(*igc, *r.rsm_id(), "Triangulation_1", true, false, false);
  r.rsm_adjustable_parameter(adj);
  if(false)
    std::cerr << r << "\n";
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp);
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
  boost::shared_ptr<GroundCoordinate> gpcalc =
    r.ground_coordinate(ic_expect, gp.height_reference_surface());
  BOOST_CHECK(distance(gp, *gpcalc) < 1.0);
}

BOOST_AUTO_TEST_CASE(image_correction_jac_test)
{
  boost::shared_ptr<Rsm> r = boost::make_shared<Rsm>(rp_from_rpc, cconv);
  RsmImageGroundConnection rigc(r, boost::make_shared<SimpleDem>(),
		boost::make_shared<MemoryRasterImage>(100, 300, 0));
  boost::shared_ptr<RsmAdjustableParameterA> adj = boost::make_shared<RsmAdjustableParameterA>(*igc, *r->rsm_id(), "Triangulation_1", true, false, false);
  r->rsm_adjustable_parameter(adj);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = rigc.image_coordinate(gp);
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
  blitz::Array<double, 1> eps(rigc.parameter_subset().rows());
  eps = 1e-2;
  blitz::Array<double, 2> jacfd = rigc.image_coordinate_jac_parm_fd(gp, eps);
  blitz::Array<double, 2> jac = rigc.image_coordinate_jac_parm(gp);
  BOOST_CHECK_MATRIX_CLOSE_TOL(jacfd, jac, 1e-4);
}

BOOST_AUTO_TEST_CASE(tre)
{
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  Rsm r(rp_from_rpc, cconv);
  boost::shared_ptr<RsmAdjustableParameterA> adj = boost::make_shared<RsmAdjustableParameterA>(*igc, *r.rsm_id(), "Triangulation_1", true, false, false);
  std::string d = serialize_write_string(adj);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RsmAdjustableParameterA> radj =
    serialize_read_string<RsmAdjustableParameterA>(d);
  BOOST_CHECK_EQUAL(adj->image_identifier(), radj->image_identifier());
  BOOST_CHECK_EQUAL(adj->rsm_suport_data_edition(),
		    radj->rsm_suport_data_edition());
  BOOST_CHECK_EQUAL(adj->triangulation_id(),
		    radj->triangulation_id());
  BOOST_CHECK_MATRIX_CLOSE(adj->parameter(),
			   radj->parameter());
}
BOOST_AUTO_TEST_SUITE_END()
