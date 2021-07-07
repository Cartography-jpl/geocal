#include "unit_test_support.h"
#include "rsm_indirect_covariance_b.h"
#include "rsm.h"
#include "rsm_image_ground_connection.h"
#include "rsm_fixture.h"
#include "local_rectangular_coordinate.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_indirect_covariance_b, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  Rsm r(rp_from_rpc, cconv);
  auto cov = boost::make_shared<RsmIndirectCovarianceB>
    (*igc, -100, 100, *r.rsm_id(), "Triangulation_1");
  r.rsm_indirect_covariance(cov);
  if(true)
    std::cerr << r << "\n";
  std::cerr << cov->tre_string() << "\n";
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  Rsm r(rp_from_rpc, cconv);
  auto cov = boost::make_shared<RsmIndirectCovarianceB>
    (*igc, -100, 100, *r.rsm_id(), "Triangulation_1");
  std::string d = serialize_write_string(cov);
  if(false)
    std::cerr << d;
  auto rcov = serialize_read_string<RsmIndirectCovarianceB>(d);
  BOOST_CHECK_EQUAL(cov->image_identifier(), rcov->image_identifier());
  BOOST_CHECK_EQUAL(cov->rsm_suport_data_edition(),
		    rcov->rsm_suport_data_edition());
  BOOST_CHECK_EQUAL(cov->triangulation_id(),
		    rcov->triangulation_id());
}
BOOST_AUTO_TEST_SUITE_END()
