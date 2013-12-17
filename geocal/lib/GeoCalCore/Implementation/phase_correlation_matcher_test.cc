#include "unit_test_support.h"
#include "phase_correlation_matcher.h"
#include "vicar_lite_file.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(phase_correlation_matcher, GlobalFixture)

BOOST_AUTO_TEST_CASE(picmtch4_test)
{
  // This test matches the picmtch4 tests. The data used here was
  // extracted for that test.
  VicarLiteRasterImage img1(test_data_dir() + "phase_correlation/xxim1");
  VicarLiteRasterImage img2(test_data_dir() + "phase_correlation/xxim2");
  ImageCoordinate ref_ic(150.0, 150.0);
  PhaseCorrelationMatcher m(32, 32);
  ImageCoordinate new_res;
  double line_sigma, sample_sigma;
  bool success;
  ImageCoordinate new_ic_expected(ref_ic.line - 4, ref_ic.sample - 5);
  // Perfect prediction
  ImageCoordinate new_ic = new_ic_expected;
  m.match(img1, img2, ref_ic, new_ic, new_res, line_sigma, 
	  sample_sigma, success);
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(new_res.line, new_ic_expected.line, 1e-2);
  BOOST_CHECK_CLOSE(new_res.sample, new_ic_expected.sample, 1e-2);
  BOOST_CHECK_CLOSE(m.correlation_last_match(), 0.4805, 1e-2);
  // Off by 6 pixels
  PhaseCorrelationMatcher m2(32, 96);
  new_ic.line = ref_ic.line;
  new_ic.sample = ref_ic.sample;
  m2.match(img1, img2, ref_ic, new_ic, new_res, line_sigma, 
	   sample_sigma, success);
  BOOST_CHECK(success);
  BOOST_CHECK(fabs(new_res.line - new_ic_expected.line) < line_sigma);
  BOOST_CHECK(fabs(new_res.sample - new_ic_expected.sample) < sample_sigma);
  BOOST_CHECK_CLOSE(m2.correlation_last_match(), 0.128168, 1e-2);
  // More careful check that we get very close to what picmtch4 got
  // (even including the slight error). This was compared to the
  // results from picmtch4 to get the expected results here.
  BOOST_CHECK_CLOSE(new_res.line, ref_ic.line - 3.98539, 1e-4);
  BOOST_CHECK_CLOSE(new_res.sample, ref_ic.sample - 5.017241, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
