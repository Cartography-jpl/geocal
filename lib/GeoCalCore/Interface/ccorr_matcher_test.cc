#include "unit_test_support.h"
#include "ccorr_matcher.h"
#include "matcher_fixture.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ccorr_matcher, MatcherFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  CcorrMatcher m(25, 23, 9, 11, 0.7);
  BOOST_CHECK_CLOSE(m.min_correlation(), 0.7, 1e-4);
  BOOST_CHECK_EQUAL(m.target_number_line(), 25);
  BOOST_CHECK_EQUAL(m.target_number_sample(), 23);
  BOOST_CHECK_EQUAL(m.template_number_line(), 9);
  BOOST_CHECK_EQUAL(m.template_number_sample(), 11);
  ImageCoordinate new_res;
  double line_sigma, sample_sigma;
  bool success;
  m.match(ref_img, new_img, ref_ic, 
	  ImageCoordinate(new_ic.line + 5, new_ic.sample - 4), new_res,
	  line_sigma, sample_sigma, success);
  BOOST_CHECK(fabs(new_res.line - new_ic.line) < 1);
  BOOST_CHECK(fabs(new_res.sample - new_ic.sample) < 1);
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(line_sigma, 0.5, 1e-4);
  BOOST_CHECK_CLOSE(sample_sigma, 0.5, 1e-4);
  m.match(ref_img, new_img, ref_ic, 
	  ImageCoordinate(new_ic.line + 11, new_ic.sample - 4), new_res,
	  line_sigma, sample_sigma, success);
  BOOST_CHECK(!success);
}

BOOST_AUTO_TEST_SUITE_END()
