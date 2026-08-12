#include "unit_test_support.h"
#include "rsm_direct_covariance_a.h"
#include "rsm.h"
#include "rsm_image_ground_connection.h"
#include "rsm_fixture.h"
#include "local_rectangular_coordinate.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_direct_covariance_a, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  // Rsm r(rp_from_rpc, cconv);
  // boost::shared_ptr<RsmAdjustableParameterA> adj = boost::make_shared<RsmAdjustableParameterA>(*igc, *r.rsm_id(), "Triangulation_1", true, false, false);
  // r.rsm_adjustable_parameter(adj);
  // if(false)
  //   std::cerr << r << "\n";
  // ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  // ImageCoordinate ic;
  // bool in_valid_range;
  // r.image_coordinate(gp, ic, in_valid_range);
  // BOOST_CHECK(in_valid_range);
  // BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  // BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
  // boost::shared_ptr<GroundCoordinate> gpcalc =
  //   r.ground_coordinate_z(ic_expect, gp.height_reference_surface());
  // BOOST_CHECK(distance(gp, *gpcalc) < 1.0);
}

BOOST_AUTO_TEST_CASE(tre)
{
  // This is test data from one of the sample files found at
  // http://www.gwg.nga.mil/ntb/baseline/software/testfile/rsm/index.htm
  // Test reading this.
  std::string tre_in = "2_8                                                                             1101222272-2                            1101222272-1                            06001000062_8                                                                             06-2.42965895449297E+06-4.76049894293300E+06+3.46898407315533E+06+8.90698769551156E-01+2.48664813021570E-01-3.80554217799520E-01-4.54593996792805E-01+4.87215943350720E-01-7.45630553709282E-01+0.00000000000000E+00+8.37129879594448E-01+5.47004172461403E-01                                        010203040506                    +5.77388827727787E+04+2.60049315375747E+03+3.19928338117848E+03-1.31174811541913E+00+2.20596677933116E+01-2.84031890108660E+00+5.11824419774739E+04+1.11777635517993E+03-1.93235228164728E+01+1.74818259318562E+00+7.06800879909857E+00+1.75094830424667E+04-1.51827591855326E+00+2.88288601417383E+00-3.57854210117485E-01+9.21252244533737E-03-1.05784554995790E-03-3.62502042683891E-03+1.04702372207584E-02-1.05046419207449E-03+1.14369570920252E-02";
  auto rcov = RsmDirectCovarianceA::read_tre_string(tre_in);
  if(false)
    std::cerr << *rcov;
}

BOOST_AUTO_TEST_CASE(serialize)
{
  // if(!have_serialize_supported())
  //   return;
  // Rsm r(rp_from_rpc, cconv);
  // boost::shared_ptr<RsmAdjustableParameterA> adj = boost::make_shared<RsmAdjustableParameterA>(*igc, *r.rsm_id(), "Triangulation_1", true, false, false);
  // std::string d = serialize_write_string(adj);
  // if(false)
  //   std::cerr << d;
  // boost::shared_ptr<RsmAdjustableParameterA> radj =
  //   serialize_read_string<RsmAdjustableParameterA>(d);
  // BOOST_CHECK_EQUAL(adj->image_identifier(), radj->image_identifier());
  // BOOST_CHECK_EQUAL(adj->rsm_support_data_edition(),
  // 		    radj->rsm_support_data_edition());
  // BOOST_CHECK_EQUAL(adj->triangulation_id(),
  // 		    radj->triangulation_id());
  // BOOST_CHECK_MATRIX_CLOSE(adj->parameter(),
  // 			   radj->parameter());
}
BOOST_AUTO_TEST_SUITE_END()
