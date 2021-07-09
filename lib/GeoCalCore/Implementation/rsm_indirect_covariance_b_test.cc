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
  blitz::Array<int, 2> row_power(3,3);
  row_power = 1,0,0,
              0,1,0,
              0,0,1;
  cov->row_power(row_power);
  blitz::Array<double, 2> cov_sub1(2,2);
  cov_sub1 = 1, 2,
             2, 3;
  blitz::Array<double, 2> cov_sub2(1,1);
  cov_sub2 = 4;
  cov->add_subgroup(boost::make_shared<RsmBSubgroup>(cov_sub1, 1, 1,0,0,10));
  cov->add_subgroup(boost::make_shared<RsmBSubgroup>(cov_sub2, 1, 1,0,0,10));
  blitz::Array<double, 2> mapping_matrix(3,3);
  mapping_matrix = 1, 0, 0,
                   0, 2, 0,
                   0, 0, 3;
  cov->mapping_matrix(mapping_matrix);
  blitz::Array<double, 2> cov_u(2,2);
  cov_u = 0.25, 0,
          0, 0.25;
  cov->unmodeled_covariance(boost::make_shared<RsmBUnmodeledCovariance>
			    (cov_u, 1,0,0,10,1,0,0,10));
  r.rsm_indirect_covariance(cov);
  if(false)
    std::cerr << r << "\n";
  std::string tre = cov->tre_string();
  auto cov2 = RsmIndirectCovarianceB::read_tre_string(tre);
  r.rsm_indirect_covariance(cov2);
  if(false)
    std::cerr << r << "\n";
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  Rsm r(rp_from_rpc, cconv);
  auto cov = boost::make_shared<RsmIndirectCovarianceB>
    (*igc, -100, 100, *r.rsm_id(), "Triangulation_1");
  blitz::Array<int, 2> row_power(3,3);
  row_power = 1,0,0,
              0,1,0,
              0,0,1;
  cov->row_power(row_power);
  blitz::Array<double, 2> cov_sub1(2,2);
  cov_sub1 = 1, 0,
             0, 3;
  blitz::Array<double, 2> cov_sub2(1,1);
  cov_sub2 = 4;
  cov->add_subgroup(boost::make_shared<RsmBSubgroup>(cov_sub1, 1, 1,0,0,10));
  cov->add_subgroup(boost::make_shared<RsmBSubgroup>(cov_sub2, 1, 1,0,0,10));
  blitz::Array<double, 2> mapping_matrix(3,3);
  mapping_matrix = 1, 0, 0,
                   0, 2, 0,
                   0, 0, 3;
  cov->mapping_matrix(mapping_matrix);
  blitz::Array<double, 2> cov_u(2,2);
  cov_u = 0.25, 0,
          0, 0.25;
  cov->unmodeled_covariance(boost::make_shared<RsmBUnmodeledCovariance>
			    (cov_u, 1,0,0,10,1,0,0,10));
  std::string d = serialize_write_string(cov);
  if(false)
    std::cerr << d;
  auto rcov = serialize_read_string<RsmIndirectCovarianceB>(d);
  BOOST_CHECK_EQUAL(cov->image_identifier(), rcov->image_identifier());
  BOOST_CHECK_EQUAL(cov->rsm_support_data_edition(),
		    rcov->rsm_support_data_edition());
  BOOST_CHECK_EQUAL(cov->triangulation_id(),
		    rcov->triangulation_id());
}
BOOST_AUTO_TEST_SUITE_END()
