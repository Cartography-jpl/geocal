#include "unit_test_support.h"
#include "rsm_rational_polynomial.h"
#include "rsm_fixture.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_rational_polynomial, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  RsmRationalPolynomial r(3,3,3,3,3,3,3,3);
  r.set_rpc_coeff(rpc);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp.longitude(), gp.latitude(),
					  gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
  Array<double, 2> jac_exp =
    rpc.image_coordinate_jac(gp.latitude(), gp.longitude(),
			     gp.height_reference_surface());
  Array<double, 2> jac =
    r.image_coordinate_jacobian(gp.longitude(), gp.latitude(),
				gp.height_reference_surface());
  // We have first column swapped because RPC has lat at x, while RSM
  // has lon as x.
  BOOST_CHECK_MATRIX_CLOSE(jac(Range::all(), 0), jac_exp(Range::all(), 1));
  BOOST_CHECK_MATRIX_CLOSE(jac(Range::all(), 1), jac_exp(Range::all(), 0));
  BOOST_CHECK_MATRIX_CLOSE(jac(Range::all(), 2), jac_exp(Range::all(), 2));
}

BOOST_AUTO_TEST_CASE(tre)
{
  RsmRationalPolynomial r(3,3,3,3,3,3,3,3);
  r.set_rpc_coeff(rpc);
  boost::shared_ptr<RsmRationalPolynomial> r2 =
    RsmRationalPolynomial::read_tre_string(r.tre_string());
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r2->image_coordinate(gp.longitude(), gp.latitude(),
					  gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
}

BOOST_AUTO_TEST_CASE(fit_data_test)
{
  RsmRationalPolynomial r(3,3,3,3,3,3,3,3);
  double hmin = rpc.height_offset - rpc.height_scale;
  double hmax = rpc.height_offset + rpc.height_scale;
  double lmin = 0;
  double smin = 0;
  double lmax = rpc.line_offset * 2;
  double smax = rpc.sample_offset * 2;
  std::vector<double> line, sample, x, y, z;
  for(int i = 0; i < 20; ++i)
    for(int j = 0; j < 20; ++j)
      for(int k = 0; k < 20; ++k) {
	double ln = lmin + (lmax - lmin) / (20 - 1) * i;
	double smp = smin + (smax - smin) / (20 - 1) * j;
	double h = hmin + (hmax - hmin) / (20 - 1) * k;
	boost::shared_ptr<GroundCoordinate> gc =
	  rpc.ground_coordinate(ImageCoordinate(ln, smp), h);
	line.push_back(ln);
	sample.push_back(smp);
	x.push_back(gc->longitude());
	y.push_back(gc->latitude());
	z.push_back(gc->height_reference_surface());
      }
  r.fit_offset_and_scale(*std::min_element(line.begin(), line.end()),
			 *std::max_element(line.begin(), line.end()),
			 *std::min_element(sample.begin(), sample.end()),
			 *std::max_element(sample.begin(), sample.end()),
			 *std::min_element(x.begin(), x.end()),
			 *std::max_element(x.begin(), x.end()),
			 *std::min_element(y.begin(), y.end()),
			 *std::max_element(y.begin(), y.end()),
			 *std::min_element(z.begin(), z.end()),
			 *std::max_element(z.begin(), z.end()));
  r.fit_data(line, sample, x, y, z);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp.longitude(), gp.latitude(),
					  gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
}

BOOST_AUTO_TEST_CASE(fit)
{
  RsmRationalPolynomial r(3,3,3,3,3,3,3,3);
  double hmin = rpc.height_offset - rpc.height_scale;
  double hmax = rpc.height_offset + rpc.height_scale;
  double lmin = 0;
  double smin = 0;
  double lmax = rpc.line_offset * 2;
  double smax = rpc.sample_offset * 2;
  GeodeticConverter cconv;
  r.fit(*igc, cconv, hmin, hmax, lmin, lmax, smin, smax);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp.longitude(), gp.latitude(),
					  gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<RsmRationalPolynomial> r =
    boost::make_shared<RsmRationalPolynomial>(3,3,3,3,3,3,3,3);
  r->set_rpc_coeff(rpc);
  std::string d = serialize_write_string(r);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RsmRationalPolynomial> rr = 
    serialize_read_string<RsmRationalPolynomial>(d);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = rr->image_coordinate(gp.longitude(), gp.latitude(),
					    gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
}
BOOST_AUTO_TEST_SUITE_END()
