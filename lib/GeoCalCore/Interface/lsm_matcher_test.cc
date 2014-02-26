#include "unit_test_support.h"
#include "lsm_matcher.h"
#include "matcher_fixture.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(lsm_matcher, MatcherFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  LsmMatcher m(21, 19, 3, 0.06, 0.2, 0.5, 2.0, 0.125, 0.1);
  LsmMatcher m2(m);
  BOOST_CHECK_EQUAL(m.number_line(), 21);
  BOOST_CHECK_EQUAL(m.number_sample(), 19);
  BOOST_CHECK_EQUAL(m.border_size(), 3);
  BOOST_CHECK_CLOSE(m.precision_goal(), 0.06, 1e-4);
  BOOST_CHECK_CLOSE(m.precision_requirement(), 0.2, 1e-4);
  BOOST_CHECK_CLOSE(m.rad_uncertainty_factor(), 2.0, 1e-4);
  BOOST_CHECK_CLOSE(m.precision_min_geo_goal(), 0.125, 1e-4);
  BOOST_CHECK_CLOSE(m.precision_min_rad_goal(), 0.1, 1e-4);
  BOOST_CHECK_EQUAL(m2.number_line(), 21);
  BOOST_CHECK_EQUAL(m2.number_sample(), 19);
  BOOST_CHECK_EQUAL(m2.border_size(), 3);
  BOOST_CHECK_CLOSE(m2.precision_goal(), 0.06, 1e-4);
  BOOST_CHECK_CLOSE(m2.precision_requirement(), 0.2, 1e-4);
  BOOST_CHECK_CLOSE(m2.rad_uncertainty_factor(), 2.0, 1e-4);
  BOOST_CHECK_CLOSE(m2.precision_min_geo_goal(), 0.125, 1e-4);
  BOOST_CHECK_CLOSE(m2.precision_min_rad_goal(), 0.1, 1e-4);

  ImageCoordinate new_res;
  double line_sigma, sample_sigma;
  bool success;
  m.match(ref_img, new_img, ref_ic, 
	  ImageCoordinate(new_ic.line + 0.8, new_ic.sample - 0.5), new_res,
	  line_sigma, sample_sigma, success);
  BOOST_CHECK(fabs(new_res.line - new_ic.line) < m.precision_requirement());
  BOOST_CHECK(fabs(new_res.sample - new_ic.sample) < m.precision_requirement());
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(line_sigma, m.precision_goal(), 1e-4);
  BOOST_CHECK_CLOSE(sample_sigma, m.precision_goal(), 1e-4);
  m2.match(ref_img, new_img, ref_ic, 
	  ImageCoordinate(new_ic.line + 0.8, new_ic.sample - 0.5), new_res,
	  line_sigma, sample_sigma, success);
  BOOST_CHECK(fabs(new_res.line - new_ic.line) < m.precision_requirement());
  BOOST_CHECK(fabs(new_res.sample - new_ic.sample) < m.precision_requirement());
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(line_sigma, m2.precision_goal(), 1e-4);
  BOOST_CHECK_CLOSE(sample_sigma, m2.precision_goal(), 1e-4);
  m.match(ref_img, new_img, ref_ic, 
	  ImageCoordinate(new_ic.line + 4, new_ic.sample - 5), new_res,
	  line_sigma, sample_sigma, success);
  BOOST_CHECK(!success);
}

BOOST_AUTO_TEST_SUITE_END()

