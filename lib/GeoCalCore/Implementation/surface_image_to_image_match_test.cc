#include "unit_test_support.h"
#include "surface_image_to_image_match.h"
#include "simple_dem.h"
#include "vicar_lite_file.h"
#include "gdal_raster_image.h"
#include "rpc_image_ground_connection.h"
#include "ccorr_lsm_matcher.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(surface_image_to_image_match, GlobalFixture)
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
  boost::shared_ptr<RpcImageGroundConnection>
    igc2(new RpcImageGroundConnection(img2->rpc(), dem, img2));
  boost::shared_ptr<ImageMatcher> im(new CcorrLsmMatcher());
  boost::shared_ptr<RasterImage> 
    aoi(new VicarLiteRasterImage(stereo_test_data_dir() + "aoi.img"));
  SurfaceImageToImageMatch imgtoimg(igc1, igc2, aoi->map_info(), im);
  ImageCoordinate ic2;
  double line_sigma, sample_sigma;
  bool success;
  imgtoimg.match(ImageCoordinate(1000,1000), ic2, line_sigma, sample_sigma, 
		 success);
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(ic2.line, 1032.539, 1e-3);
  BOOST_CHECK_CLOSE(ic2.sample, 1022.133, 1e-3);
  ImageCoordinate ic1;
  boost::shared_ptr<GroundCoordinate> gc = 
    igc1->ground_coordinate(ImageCoordinate(1000,1000));
  imgtoimg.match_surf(*gc, ic1, ic2, line_sigma, sample_sigma, 
		 success);
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(ic2.line, 1032.539, 1e-3);
  BOOST_CHECK_CLOSE(ic2.sample, 1022.133, 1e-3);
  BOOST_CHECK_CLOSE(ic1.line, 1000.0, 1e-3);
  BOOST_CHECK_CLOSE(ic1.sample, 1000.0, 1e-3);
  
}

BOOST_AUTO_TEST_CASE(with_surface_image)
{
  double initial_height = 1291;
  boost::shared_ptr<Dem> dem(new SimpleDem(initial_height));
  boost::shared_ptr<RasterImage> 
    img1(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-1.img"));
  boost::shared_ptr<RasterImage> 
    img2(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-2.img"));
  boost::shared_ptr<ImageGroundConnection>
    igc1(new RpcImageGroundConnection(img1->rpc(), dem, img1));
  boost::shared_ptr<RpcImageGroundConnection>
    igc2(new RpcImageGroundConnection(img2->rpc(), dem, img2));
  // We are working with raw WV2 data which hasn't been corrected yet for 
  // misregistration. We'll eventually code and test an SBA, but for the 
  // purpose of these tests just correct the RPC to coregister the data.
  igc2->rpc().line_offset -= -0.949821385786;
  igc2->rpc().sample_offset -= -0.97417620076;
  boost::shared_ptr<ImageMatcher> im(new CcorrLsmMatcher());
  boost::shared_ptr<RasterImage> 
    simg1(new GdalRasterImage(stereo_test_data_dir() + 
			      "10MAY21-1_projected.tif"));
  boost::shared_ptr<RasterImage> 
    simg2(new GdalRasterImage(stereo_test_data_dir() + 
			      "10MAY21-2_projected.tif"));
  SurfaceImageToImageMatch imgtoimg(igc1, simg1, igc2, simg2, im);
  ImageCoordinate ic2;
  double line_sigma, sample_sigma;
  bool success;
  imgtoimg.match(ImageCoordinate(1000,1000), ic2, line_sigma, sample_sigma, 
		 success);
  BOOST_CHECK(success);
  BOOST_CHECK_CLOSE(ic2.line, 1032.539, 1e-2);
  BOOST_CHECK_CLOSE(ic2.sample, 1022.133, 1e-2);
}
BOOST_AUTO_TEST_SUITE_END()
