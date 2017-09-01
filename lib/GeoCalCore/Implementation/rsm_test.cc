#include "unit_test_support.h"
#include "rsm.h"
#include "geocal_rpc.h"
#include "geodetic.h"
#include "vicar_file.h"
#include "srtm_dem.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;
using namespace blitz;

class RsmFixture : public GlobalFixture {
public:
  RsmFixture()
  {
  rpc.error_bias = 0;
  rpc.error_random = 0;
  rpc.rpc_type = Rpc::RPC_B;
  rpc.line_offset = 2881;
  rpc.line_scale = 2921;
  rpc.sample_offset = 13763;
  rpc.sample_scale = 14238;
  rpc.latitude_offset = 35.8606;
  rpc.latitude_scale = 0.0209;
  rpc.longitude_offset = 44.9534;
  rpc.longitude_scale = 0.1239;
  rpc.height_offset = 1017;
  rpc.height_scale = 634;
  boost::array<double, 20> t1 = 
    {{ 0.0132748, -0.14751, -1.13465, -0.0138959, 0.0020018,
      6.35242e-05, 0.000115861, -0.00286551, -0.00778726, 
      3.88308e-06, -1.26487e-06, 7.881069999999999e-06, 
      3.65929e-05, 2.32154e-06, -2.25421e-05, -2.08933e-05, 
       1.8091e-05, 3.6393e-07, -9.39815e-07, -4.31269e-08 }};
  rpc.line_numerator = t1;
  boost::array<double, 20> t2 = 
    {{ 1, 0.00380256, 0.00643151, 0.00031479,
      1.84958e-05, -1.98243e-06, -1.16422e-06,
      -1.92637e-05, 7.224010000000001e-05, -1.61779e-05,
      4.95049e-06, 1.26794e-06, 0.000190771, -1.57429e-07,
      2.46815e-05, 0.00049166, -5.41022e-07, 3.209e-07,
       1.81401e-05, 1.43592e-07}};
  rpc.line_denominator = t2;
  boost::array<double, 20> t3 = 
    {{ -0.0104025, 0.96885, -0.000487887, -0.0325142,
      -0.000710444, 0.000217572, -6.549690000000001e-05,
      0.0107384, -5.19453e-06, -1.76761e-05, -1.21058e-06,
      0.000106017, 5.41226e-06, -3.8968e-06, 1.03307e-05,
      5.84016e-05, 3.80777e-08, 9.01765e-06, 1.65988e-06,
       -1.19374e-07}};
  rpc.sample_numerator = t3;
  boost::array<double, 20> t4 = 
    {{1, -0.000319831, 0.000681092, -0.000549762,
     -2.67977e-06, -6.19388e-06, 2.67975e-08, 4.76371e-06,
     -1.47985e-05, -4.23457e-06, 1.44009e-08, -1.07213e-06,
     1.1059e-07, 4.10217e-08, -1.69482e-07, 1.08104e-06,
      1e-9, -2.33038e-07, 1.86413e-08, -1.35637e-08}};
  rpc.sample_denominator = t4;
  gp = Geodetic(35.8399968, 45.0770183, 1017);
  rp = boost::make_shared<RsmRationalPolynomial>(3,3,3,3,3,3,3,3);
  rp->set_rpc_coeff(rpc);
  cconv = boost::make_shared<GeodeticConverter>();
  }
  boost::shared_ptr<RsmRationalPolynomial> rp;
  boost::shared_ptr<CoordinateConverter> cconv;
  Rpc rpc;
  Geodetic gp;
};
  
BOOST_FIXTURE_TEST_SUITE(rsm, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  Rsm r(rp, cconv);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp);
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
  boost::shared_ptr<GroundCoordinate> gpcalc =
    r.ground_coordinate(ic_expect, gp.height_reference_surface());
  BOOST_CHECK(distance(gp, *gpcalc) < 1.0);
}

BOOST_AUTO_TEST_CASE(ground_coordinate_dem)
{
  Rsm r(rp, cconv);
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
    std::cerr << GeoCal::distance(*gpsurf, *gpcalc) << "\n";
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Rsm> r = boost::make_shared<Rsm>(rp,cconv);
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
BOOST_AUTO_TEST_SUITE_END()
