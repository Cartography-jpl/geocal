#include "geocal_config.h"
#include "unit_test_support.h"
#include "material_detect.h"
#include "vicar_lite_file.h"
#include "gdal_multi_band.h"
using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(material_detect, GlobalFixture)
// Compare with test data from a previous Shiva run.
BOOST_AUTO_TEST_CASE(shiva)
{
#ifndef HAVE_VICAR_GDALPLUGIN
  // We can perhaps set this up to work without the VICAR gdalplugin
  // at some point, but for now just punt if we don't have this.
  return;
#endif
#ifndef HAVE_VICAR_RTL
  return;
#endif
  boost::shared_ptr<GdalMultiBand> mul
    (new GdalMultiBand(shiva_test_data_dir() + "post_b1:8.img"));
  boost::shared_ptr<VicarLiteRasterImage> pandata
    (new VicarLiteRasterImage(shiva_test_data_dir() + "pre_pan_sub.img"));
  boost::shared_ptr<VicarLiteRasterImage> pandif
    (new VicarLiteRasterImage(shiva_test_data_dir() + "pandif_sub.img"));
  blitz::Array<double, 1> pdiff_thresh(11), sdiff_thresh(11);
  blitz::Array<int, 1> priority(11);
  pdiff_thresh = 2900,3700,3700,3700,3700,3700,3700,3700,3700,3700,3700;
  sdiff_thresh = 40,20,20,20,20,20,20,20,20,20,20;
  priority = 2,2,2,1,1,1,2,2,1,2,1;
  double pan_shadow_threshold = 80;
  MaterialDetect md(pandata, pandif, mul, pdiff_thresh, sdiff_thresh,
		    priority, shiva_test_data_dir() + "portgaston_bk.int",
		    pan_shadow_threshold);
  VicarLiteRasterImage md_expect(shiva_test_data_dir() + "material_sub.img");
  Array<int, 2> md1 = md.read(0, 0, md.number_line(), md.number_sample());
  Array<int, 2> md2 = 
    md_expect.read(0, 0, md_expect.number_line(), md_expect.number_sample());
  for(int i = 0; i < md1.rows(); ++i)
    for(int j = 0; j < md1.cols(); ++j)
      if(md1(i, j) != md2(i, j)) {
	std::cerr << "(" << i << ", " << j << "): "
		  << md1(i, j) << " " << md2(i, j) << "\n";
      }
  BOOST_CHECK_MATRIX_CLOSE(md.read(0, 0, md.number_line(), md.number_sample()),
   md_expect.read(0, 0, md_expect.number_line(), md_expect.number_sample()));
}

BOOST_AUTO_TEST_SUITE_END()
