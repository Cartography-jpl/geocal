#include "unit_test_support.h"
#include "rsm_grid.h"
#include "rsm_fixture.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_grid, RsmGridFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  RsmGrid& r = *rg_from_rpc;
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp.longitude(), gp.latitude(),
					  gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
  blitz::Array<double, 2> jac_exp(2,3);
  double eps = 1e-6;
  ImageCoordinate ic_x = r.image_coordinate(gp.longitude()+eps, gp.latitude(),
					  gp.height_reference_surface());
  ImageCoordinate ic_y = r.image_coordinate(gp.longitude(), gp.latitude()+eps,
					  gp.height_reference_surface());
  ImageCoordinate ic_z = r.image_coordinate(gp.longitude(), gp.latitude(),
					    gp.height_reference_surface()+eps);
  jac_exp(0,0) = (ic_x.line - ic.line)/eps;
  jac_exp(0,1) = (ic_y.line - ic.line)/eps;
  jac_exp(0,2) = (ic_z.line - ic.line)/eps;
  jac_exp(1,0) = (ic_x.sample - ic.sample)/eps;
  jac_exp(1,1) = (ic_y.sample - ic.sample)/eps;
  jac_exp(1,2) = (ic_z.sample - ic.sample)/eps;
  Array<double, 2> jac =
    r.image_coordinate_jacobian(gp.longitude(), gp.latitude(),
				gp.height_reference_surface());
  if(false)
    std::cerr << "jac: " << jac << "\n"
	      << "jac_exp: " << jac_exp << "\n";
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac, jac_exp,1e-2);
}

BOOST_AUTO_TEST_CASE(tre)
{
  RsmGrid& r = *rg_from_rpc;
  boost::shared_ptr<RsmGrid> r2 =
    RsmGrid::read_tre_string(r.tre_string());
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r2->image_coordinate(gp.longitude(), gp.latitude(),
   					    gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  std::string d = serialize_write_string(rg_from_rpc);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RsmGrid> rr = 
    serialize_read_string<RsmGrid>(d);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = rr->image_coordinate(gp.longitude(), gp.latitude(),
					    gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
}

BOOST_AUTO_TEST_CASE(problem_case)
{
  // Specific error case we ran into with testing. Don't normally run
  // this, but leave in place in case we need to come back to this.
  return;
  boost::shared_ptr<Rsm> rsm = serialize_read_binary<Rsm>("/home/smyth/Local/RsmPlay/rsm_test.bin");
  boost::shared_ptr<GroundCoordinate> gc = serialize_read<GroundCoordinate>("/home/smyth/Local/RsmPlay/test_point.xml");
  ImageCoordinate ic;
  bool in_valid_range;
  std::cerr << "gc: " << *gc << "\n";
  rsm->image_coordinate(*gc, ic, in_valid_range);
  std::cerr << "ic: " << ic << "\n";
}

BOOST_AUTO_TEST_SUITE_END()
