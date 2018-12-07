#include "unit_test_support.h"
#include "rsm_nitf.h"
#include "rsm_fixture.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(rsm_nitf, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  rsm_write_nitf("test_out/rsm_out.ntf", rsm);
}

BOOST_AUTO_TEST_SUITE_END()
