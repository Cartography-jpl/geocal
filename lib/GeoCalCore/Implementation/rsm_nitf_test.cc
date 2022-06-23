#include "unit_test_support.h"
#include "rsm_nitf.h"
#include "rsm_fixture.h"
#include "ecr.h"
#include <boost/make_shared.hpp>
#include <boost/process.hpp>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(rsm_nitf, RsmFixtureRadian)
BOOST_AUTO_TEST_CASE(basic_test)
{
  // This test depends on having geocal and python scripts installed,
  // it is really a installcheck test rather than a check test. Check
  // to see if boost_nitf_rsm is in the path, if not then don't run this.
  if(boost::process::search_path("boost_nitf_rsm") == "")
    return;
  rsm_write_nitf("test_out/rsm_out.ntf", rsm);
  boost::shared_ptr<Rsm> rsmr = rsm_read_nitf("test_out/rsm_out.ntf",
					      Ecr::EARTH_NAIF_CODE);
  std::cerr << *rsmr << "\n";
}

BOOST_AUTO_TEST_SUITE_END()
