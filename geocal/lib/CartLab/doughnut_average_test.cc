#include "unit_test_support.h"
#include "doughnut_average.h"
#include "memory_raster_image.h"
#include "vicar_lite_file.h"
using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(doughnut_average, GlobalFixture)

// Full, slower calculation of doughnut average.
double calc(const RasterImage& Ri, int ln, int smp)
{
  int cnt = 0;
  double sum = 0;
  const int whs = (11 - 1) / 2;
  const int dhs = (5 - 1) / 2;
  for(int i = ln - whs; i <= ln + whs; ++i)
    for(int j = smp - whs; j <= smp + whs; ++j)
      if(i >= 0 && i < Ri.number_line() &&
	 j >=0 && j < Ri.number_sample() &&
	 !(i >= ln - dhs && i <= ln + dhs &&
	   j >= smp - dhs && j <= smp + dhs))
	if(Ri(i, j) != 0) {
	  cnt += 1;
	  sum += Ri(i,j);
	}
  return sum / cnt;
}
BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<MemoryRasterImage> mi(new MemoryRasterImage(100, 200));
  int val = 0;
  for(int i = 0; i < mi->number_line(); ++i)
    for(int j = 0; j < mi->number_sample(); ++j)
      mi->write(i, j, ++val);
  boost::shared_ptr<RasterImageMultiBandVariable> mb
    (new RasterImageMultiBandVariable());
  mb->add_raster_image(mi);
  DoughnutAverage davg(mb, 11, 5);
  BOOST_CHECK_EQUAL(davg.window_size(), 11);
  BOOST_CHECK_EQUAL(davg.doughnut_size(), 5);
  // Compare to explicit calculation.
  Array<double, 3> dall = davg.read_double(0, 0, 100, 200);
  for(int i = 0; i < dall.cols(); ++i)
    for(int j = 0; j < dall.depth(); ++j)
      BOOST_CHECK_CLOSE(dall(0, i, j), calc(*mi, i, j), 1e-8);

  // Check proper handling of subsetting
  Range ra(Range::all());
  Array<double, 3> d1 = davg.read_double(10,15,20,25);
  BOOST_CHECK_MATRIX_CLOSE(d1, dall(ra,Range(10,29), Range(15,39)));
  // Check processing near edges
  Array<double, 3> d2 = davg.read_double(0,0,20,25);
  BOOST_CHECK_MATRIX_CLOSE(d2, dall(ra,Range(0,19), Range(0,24)));
  Array<double, 3> d3 = davg.read_double(80,0,20,25);
  BOOST_CHECK_MATRIX_CLOSE(d3, dall(ra,Range(80,99), Range(0,24)));
  Array<double, 3> d4 = davg.read_double(0,175,20,25);
  BOOST_CHECK_MATRIX_CLOSE(d4, dall(ra,Range(0,19), Range(175,199)));
}
// Compare with test data from a previous Shiva run.
BOOST_AUTO_TEST_CASE(shiva)
{
  boost::shared_ptr<RasterImageMultiBandVariable> mb
    (new RasterImageMultiBandVariable());
  mb->add_raster_image(boost::shared_ptr<RasterImage>
     (new VicarLiteRasterImage(shiva_test_data_dir() + "pre_pan_sub.img")));
  mb->add_raster_image(boost::shared_ptr<RasterImage>
     (new VicarLiteRasterImage(shiva_test_data_dir() + "post_pan_sub.img")));
  boost::shared_ptr<DoughnutAverage> davg(new DoughnutAverage(mb, 11, 5));
  VicarLiteRasterImage cvd_expect(shiva_test_data_dir() + "cvdnorm_sub.img");
  // Expect edges to be different because we aren't calculating
  // outside the window. So only compare middle part of image.
  int ls = 6;
  int ss = 6;
  int nl = cvd_expect.number_line() - 12;
  int ns = cvd_expect.number_sample() - 12;
  // Round off allows us to have up to a difference of 1.
  BOOST_CHECK(max(abs(cvd_expect.read_double(ls, ss, nl, ns) -
		      davg->cvdnorm(1, ls, ss, nl, ns))) < 1.01);

  VicarLiteRasterImage pandif_expect(shiva_test_data_dir() + "pandif_sub.img");
  BOOST_CHECK(max(abs(pandif_expect.read_double(ls, ss, nl, ns) -
		      davg->pandif(ls, ss, nl, ns))) < 1.01);
  boost::shared_ptr<RasterImage> cvdnorm_img(new RasterImageWrapCvdNorm(davg, 1));
  boost::shared_ptr<RasterImage> pandif_img(new RasterImageWrapPandif(davg));
  BOOST_CHECK(max(abs(cvd_expect.read(ls, ss, nl, ns) -
		      cvdnorm_img->read(ls, ss, nl, ns))) <= 2);
  BOOST_CHECK(max(abs(pandif_expect.read(ls, ss, nl, ns) -
		      pandif_img->read(ls, ss, nl, ns))) <= 2);
  
}

BOOST_AUTO_TEST_SUITE_END()
