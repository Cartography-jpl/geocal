#include "unit_test_support.h"
#include "igc_image_to_image_match.h"
#include "simple_dem.h"
#include "vicar_lite_file.h"
#include "rpc_image_ground_connection.h"
#include "ccorr_lsm_matcher.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(igc_image_to_image_match, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  double initial_height = 1291;
  boost::shared_ptr<Dem> dem(new SimpleDem(initial_height));
  boost::shared_ptr<RasterImage> 
    img1(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-1.img"));
  boost::shared_ptr<RasterImage> 
    img2(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-2.img"));
  boost::shared_ptr<ImageGroundConnection>
    igc1(new RpcImageGroundConnection(img1->rpc(), dem, img1));
  boost::shared_ptr<ImageGroundConnection>
    igc2(new RpcImageGroundConnection(img2->rpc(), dem, img2));
  boost::shared_ptr<ImageMatcher> im(new CcorrLsmMatcher());
  IgcImageToImageMatch imgtoimg(igc1, igc2, im);
  ImageCoordinate ic2;
  double line_sigma, sample_sigma;
  bool success;
  imgtoimg.match(ImageCoordinate(1000,1000), ic2, line_sigma, sample_sigma, 
		 success);
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(ic2.line, 1032.52, 1e-3);
  BOOST_CHECK_CLOSE(ic2.sample, 1022.16, 1e-3);
}
BOOST_AUTO_TEST_SUITE_END()
