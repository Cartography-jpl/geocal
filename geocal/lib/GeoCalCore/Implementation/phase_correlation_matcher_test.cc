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
  BOOST_CHECK_CLOSE(new_res.line, new_ic_expected.line, 1e-4);
  BOOST_CHECK_CLOSE(new_res.sample, new_ic_expected.sample, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
