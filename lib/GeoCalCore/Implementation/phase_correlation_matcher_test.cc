#include "unit_test_support.h"
#include "phase_correlation_matcher.h"
#include "geometric_model_image.h"
#include "quadratic_geometric_model.h"
#include "geocal_config.h"
#include "vicar_lite_file.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(phase_correlation_matcher, GlobalFixture)

BOOST_AUTO_TEST_CASE(picmtch4_test)
{
#ifndef HAVE_FFTW
  return;
#endif    
  // This test matches the picmtch4 tests. The data used here was
  // extracted for that test.
  boost::shared_ptr<RasterImage> img1(new VicarLiteRasterImage(test_data_dir() + "phase_correlation/xxim1"));
  boost::shared_ptr<RasterImage> img2(new VicarLiteRasterImage(test_data_dir() + "phase_correlation/xxim2"));
  ImageCoordinate ref_ic(150.0, 150.0);
  PhaseCorrelationMatcher m(32, 32);
  ImageCoordinate new_res;
  double line_sigma, sample_sigma;
  bool success;
  ImageCoordinate new_ic_expected(ref_ic.line - 4, ref_ic.sample - 5);
  // Perfect prediction
  ImageCoordinate new_ic = new_ic_expected;
  m.match(*img1, *img2, ref_ic, new_ic, new_res, line_sigma, 
	  sample_sigma, success);
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(new_res.line, new_ic_expected.line, 1e-2);
  BOOST_CHECK_CLOSE(new_res.sample, new_ic_expected.sample, 1e-2);
  BOOST_CHECK_CLOSE(m.correlation_last_match(), 0.4805, 1e-2);
  // Off by 6 pixels
  PhaseCorrelationMatcher m2(32, 96);
  new_ic.line = ref_ic.line;
  new_ic.sample = ref_ic.sample;
  m2.match(*img1, *img2, ref_ic, new_ic, new_res, line_sigma, 
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

  // Now, start doing geometric resampling of the second image to
  // match what picmtch4 is doing.
  boost::shared_ptr<QuadraticGeometricModel> geom_model(new QuadraticGeometricModel());
  GeometricModelImage gimg2(img2, geom_model, img1->number_line(), 
			    img1->number_sample());
  GeometricTiePoints tp;
  tp.add_point(ImageCoordinate(1,2), ImageCoordinate(1,1));
  tp.add_point(ImageCoordinate(1,310), ImageCoordinate(1,301));
  tp.add_point(ImageCoordinate(309,2), ImageCoordinate(301, 1));
  geom_model->fit_transformation(tp);
  m2.match(*img1, gimg2, ref_ic, ref_ic, new_res, line_sigma, 
	   sample_sigma, success);
  BOOST_CHECK(new_res == ImageCoordinate(149.96, 150.01));
  BOOST_CHECK_CLOSE(m2.correlation_last_match(), 0.104, 1e-2);
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifndef HAVE_FFTW
  return;
#endif    
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<ImageMatcher> m(new PhaseCorrelationMatcher(32,96));
  std::string d = serialize_write_string(m);
  if(false)
    std::cerr << d;
  boost::shared_ptr<PhaseCorrelationMatcher> mr = 
    serialize_read_string<PhaseCorrelationMatcher>(d);

  boost::shared_ptr<RasterImage> img1(new VicarLiteRasterImage(test_data_dir() + "phase_correlation/xxim1"));
  boost::shared_ptr<RasterImage> img2(new VicarLiteRasterImage(test_data_dir() + "phase_correlation/xxim2"));
  ImageCoordinate ref_ic(150.0, 150.0);
  ImageCoordinate new_ic;
  new_ic.line = ref_ic.line;
  new_ic.sample = ref_ic.sample;
  ImageCoordinate new_ic_expected(ref_ic.line - 4, ref_ic.sample - 5);
  ImageCoordinate new_res;
  double line_sigma, sample_sigma;
  bool success;
  mr->match(*img1, *img2, ref_ic, new_ic, new_res, line_sigma, 
	   sample_sigma, success);
  BOOST_CHECK(success);
  BOOST_CHECK(fabs(new_res.line - new_ic_expected.line) < line_sigma);
  BOOST_CHECK(fabs(new_res.sample - new_ic_expected.sample) < sample_sigma);
  BOOST_CHECK_CLOSE(mr->correlation_last_match(), 0.128168, 1e-2);
}

BOOST_AUTO_TEST_SUITE_END()
