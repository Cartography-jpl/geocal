#include "unit_test_support.h"
#include "ccorr_lsm_matcher.h"
#include "matcher_fixture.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ccorr_lsm_matcher, MatcherFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  CcorrLsmMatcher m;
  ImageCoordinate new_res;
  double line_sigma, sample_sigma;
  bool success;
  m.match(ref_img, new_img, ref_ic, 
	  ImageCoordinate(new_ic.line + 5.8, new_ic.sample - 4.5), new_res,
	  line_sigma, sample_sigma, success);
  BOOST_CHECK(fabs(new_res.line - new_ic.line) < 
	      m.lsm_matcher().precision_requirement());
  BOOST_CHECK(fabs(new_res.sample - new_ic.sample) < 
	      m.lsm_matcher().precision_requirement());
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(line_sigma, m.lsm_matcher().precision_goal(), 1e-4);
  BOOST_CHECK_CLOSE(sample_sigma, m.lsm_matcher().precision_goal(), 1e-4);
  m.match(ref_img, new_img, ref_ic, 
	  ImageCoordinate(new_ic.line + 16, new_ic.sample - 5), new_res,
	  line_sigma, sample_sigma, success);
  BOOST_CHECK(!success);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<ImageMatcher> m(new CcorrLsmMatcher());
  std::string d = serialize_write_string(m);
  if(false)
    std::cerr << d;
  boost::shared_ptr<CcorrLsmMatcher> mr = 
    serialize_read_string<CcorrLsmMatcher>(d);
  ImageCoordinate new_res;
  double line_sigma, sample_sigma;
  bool success;
  mr->match(ref_img, new_img, ref_ic, 
	  ImageCoordinate(new_ic.line + 5.8, new_ic.sample - 4.5), new_res,
	  line_sigma, sample_sigma, success);
  BOOST_CHECK(fabs(new_res.line - new_ic.line) < 
	      mr->lsm_matcher().precision_requirement());
  BOOST_CHECK(fabs(new_res.sample - new_ic.sample) < 
	      mr->lsm_matcher().precision_requirement());
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(line_sigma, mr->lsm_matcher().precision_goal(), 1e-4);
  BOOST_CHECK_CLOSE(sample_sigma, mr->lsm_matcher().precision_goal(), 1e-4);
  mr->match(ref_img, new_img, ref_ic, 
	  ImageCoordinate(new_ic.line + 16, new_ic.sample - 5), new_res,
	  line_sigma, sample_sigma, success);
  BOOST_CHECK(!success);
}

BOOST_AUTO_TEST_SUITE_END()

