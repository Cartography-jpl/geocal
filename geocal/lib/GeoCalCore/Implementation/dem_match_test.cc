#include "unit_test_support.h"
#include "dem_match.h"
#include "surface_image_to_image_match.h"
#include "simple_dem.h"
#include "vicar_lite_file.h"
#include "gdal_raster_image.h"
#include "rpc_image_ground_connection.h"
#include "ccorr_lsm_matcher.h"
#include "memory_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(dem_match, GlobalFixture)
BOOST_AUTO_TEST_CASE(with_surface_image)
{
  boost::shared_ptr<Dem> 
    dem(new VicarLiteDem(stereo_test_data_dir() + "nevada_elv_aoi.img", true));
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
  GdalRasterImage simg1_raw(stereo_test_data_dir() + "10MAY21-1_projected.tif");
  GdalRasterImage simg2_raw(stereo_test_data_dir() + "10MAY21-2_projected.tif");
  boost::shared_ptr<RasterImage> simg1(new MemoryRasterImage(simg1_raw));
  boost::shared_ptr<RasterImage> simg2(new MemoryRasterImage(simg2_raw));
  boost::shared_ptr<ImageToImageMatch> 
    imgtoimg(new SurfaceImageToImageMatch(igc1, simg1, igc2, simg2, im));
  boost::shared_ptr<RayIntersect>
    ri(new RayIntersect(igc1, igc2));
  DemMatch dm(imgtoimg, ri, 0.5);
  blitz::Array<double, 2> res = dm.surface_point(0, 0, 100, 100);
}
BOOST_AUTO_TEST_SUITE_END()
