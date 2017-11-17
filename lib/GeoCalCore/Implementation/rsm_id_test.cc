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
  r.rsm_id()->timing(boost::make_shared<RsmIdTiming>(2,3,0.1,0.2));
  r.rsm_id()->image_sequence_identifier("iseq");
  r.rsm_id()->sensor_identifier("sena");
  r.rsm_id()->sensor_type("footype");
  Time t = Time::time_pgs(100.0);
  r.rsm_id()->image_acquistion_time(boost::make_shared<Time>(t));
  rid = RsmId::read_tre_string(r.rsm_id()->tre_string());
  BOOST_CHECK_EQUAL(r.rsm_id()->image_identifier(),
		    rid->image_identifier());
  BOOST_CHECK_EQUAL(r.rsm_id()->rsm_suport_data_edition(),
		    rid->rsm_suport_data_edition());
  BOOST_CHECK_EQUAL(r.rsm_id()->image_sequence_identifier(),
		    rid->image_sequence_identifier());
  BOOST_CHECK_EQUAL(r.rsm_id()->sensor_identifier(),
		    rid->sensor_identifier());
  BOOST_CHECK_EQUAL(r.rsm_id()->sensor_type(),
		    rid->sensor_type());
  BOOST_CHECK(fabs(*r.rsm_id()->image_acquistion_time() -
		   *rid->image_acquistion_time()) < 1e-7);
  BOOST_CHECK_EQUAL(r.rsm_id()->timing()->number_row_acquired_simultaneously(),
		    rid->timing()->number_row_acquired_simultaneously());
  BOOST_CHECK_EQUAL(r.rsm_id()->timing()->number_col_acquired_simultaneously(),
		    rid->timing()->number_col_acquired_simultaneously());
  BOOST_CHECK_CLOSE(r.rsm_id()->timing()->time_between_adjacent_row_group(),
		    rid->timing()->time_between_adjacent_row_group(), 1e-4);
  BOOST_CHECK_CLOSE(r.rsm_id()->timing()->time_between_adjacent_col_group(),
		    rid->timing()->time_between_adjacent_col_group(), 1e-4);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  Rsm r(rp_from_rpc, cconv);
  r.rsm_id()->timing(boost::make_shared<RsmIdTiming>(2,3,0.1,0.2));
  r.rsm_id()->image_sequence_identifier("iseq");
  r.rsm_id()->sensor_identifier("sena");
  r.rsm_id()->sensor_type("footype");
  Time t = Time::time_pgs(100.0);
  r.rsm_id()->image_acquistion_time(boost::make_shared<Time>(t));
  std::string d = serialize_write_string(r.rsm_id());
  if(false)
    std::cerr << d;
  boost::shared_ptr<RsmId> rid = 
    serialize_read_string<RsmId>(d);
  BOOST_CHECK_EQUAL(r.rsm_id()->image_identifier(),
		    rid->image_identifier());
  BOOST_CHECK_EQUAL(r.rsm_id()->rsm_suport_data_edition(),
		    rid->rsm_suport_data_edition());
  BOOST_CHECK_EQUAL(r.rsm_id()->image_sequence_identifier(),
		    rid->image_sequence_identifier());
  BOOST_CHECK_EQUAL(r.rsm_id()->sensor_identifier(),
		    rid->sensor_identifier());
  BOOST_CHECK_EQUAL(r.rsm_id()->sensor_type(),
		    rid->sensor_type());
  BOOST_CHECK(fabs(*r.rsm_id()->image_acquistion_time() -
		   *rid->image_acquistion_time()) < 1e-7);
  BOOST_CHECK_EQUAL(r.rsm_id()->timing()->number_row_acquired_simultaneously(),
		    rid->timing()->number_row_acquired_simultaneously());
  BOOST_CHECK_EQUAL(r.rsm_id()->timing()->number_col_acquired_simultaneously(),
		    rid->timing()->number_col_acquired_simultaneously());
  BOOST_CHECK_CLOSE(r.rsm_id()->timing()->time_between_adjacent_row_group(),
		    rid->timing()->time_between_adjacent_row_group(), 1e-4);
  BOOST_CHECK_CLOSE(r.rsm_id()->timing()->time_between_adjacent_col_group(),
		    rid->timing()->time_between_adjacent_col_group(), 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
