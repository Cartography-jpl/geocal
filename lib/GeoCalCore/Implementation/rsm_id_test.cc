#include "unit_test_support.h"
#include "rsm.h"
#include "rsm_id.h"
#include "rsm_fixture.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_id, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  Rsm r(rp_from_rpc, cconv);
  BOOST_CHECK_EQUAL(r.rsm_id()->image_identifier(), "");
  BOOST_CHECK_EQUAL(r.rsm_id()->rsm_suport_data_edition(), "fake-1");
}

BOOST_AUTO_TEST_CASE(tre)
{
  Rsm r(rp_from_rpc, cconv);
  boost::shared_ptr<RsmId> rid =
    RsmId::read_tre_string(r.rsm_id()->tre_string());
  BOOST_CHECK_EQUAL(r.rsm_id()->image_identifier(),
		    rid->image_identifier());
  BOOST_CHECK_EQUAL(r.rsm_id()->rsm_suport_data_edition(),
		    rid->rsm_suport_data_edition());
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  Rsm r(rp_from_rpc, cconv);
  std::string d = serialize_write_string(r.rsm_id());
  if(false)
    std::cerr << d;
  boost::shared_ptr<RsmId> rid = 
    serialize_read_string<RsmId>(d);
  BOOST_CHECK_EQUAL(r.rsm_id()->image_identifier(),
		    rid->image_identifier());
  BOOST_CHECK_EQUAL(r.rsm_id()->rsm_suport_data_edition(),
		    rid->rsm_suport_data_edition());
}

BOOST_AUTO_TEST_SUITE_END()
