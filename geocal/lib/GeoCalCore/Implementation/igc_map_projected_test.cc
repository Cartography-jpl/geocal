#include "unit_test_support.h"
#include "igc_map_projected.h"
#include "vicar_lite_file.h"
#include "vicar_raster_image.h"
#include "gdal_raster_image.h"
#include "rpc_image_ground_connection.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(igc_map_projected, GlobalFixture)
BOOST_AUTO_TEST_CASE(full_res)
{
  boost::shared_ptr<Dem> 
    dem(new VicarLiteDem(stereo_test_data_dir() + "nevada_elv_aoi.img", true));
  boost::shared_ptr<RasterImage> 
    img(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-1.img"));
  boost::shared_ptr<ImageGroundConnection>
    igc(new RpcImageGroundConnection(img->rpc(), dem, img));
  GdalRasterImage simg(stereo_test_data_dir() + "10MAY21-1_projected.tif");
  IgcMapProjected mp(simg.map_info(), igc);

  // Manually inspected output as ok. For testing, just check a single
  // value. If this fails, you may want to enable writing out the full
  // output and seeing what is going on.
  BOOST_CHECK_CLOSE(mp.read_double(473, 1000, 100, 100)(0, 0),
		    394.18319143107396, 1e-4);
  // Optionally write this out for manual inspection
  if(true) {
    VicarRasterImage out("full_res.img", mp.map_info(), "HALF");
    copy(mp, out);
  }
}

BOOST_AUTO_TEST_CASE(averaged_res)
{
  boost::shared_ptr<Dem> 
    dem(new VicarLiteDem(stereo_test_data_dir() + "nevada_elv_aoi.img", true));
  boost::shared_ptr<RasterImage> 
    img(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-1.img"));
  boost::shared_ptr<ImageGroundConnection>
    igc(new RpcImageGroundConnection(img->rpc(), dem, img));
  GdalRasterImage simg(stereo_test_data_dir() + "10MAY21-1_projected.tif");
  IgcMapProjected mp(simg.map_info().scale(2.5, 2.5), igc);

  // Manually inspected output as ok. For testing, just check a single
  // value. If this fails, you may want to enable writing out the full
  // output and seeing what is going on.
  BOOST_CHECK_CLOSE(mp.read_double(273, 200, 100, 100)(0, 0),
		    464.77649443975633, 1e-4);
  // Optionally write this out for manual inspection
  if(true) {
    VicarRasterImage out("averaged_res.img", mp.map_info(), "HALF");
    copy(mp, out);
  }
}

BOOST_AUTO_TEST_CASE(grid_res)
{
  boost::shared_ptr<Dem> 
    dem(new VicarLiteDem(stereo_test_data_dir() + "nevada_elv_aoi.img", true));
  boost::shared_ptr<RasterImage> 
    img(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-1.img"));
  boost::shared_ptr<ImageGroundConnection>
    igc(new RpcImageGroundConnection(img->rpc(), dem, img));
  GdalRasterImage simg(stereo_test_data_dir() + "10MAY21-1_projected.tif");
  IgcMapProjected mp(simg.map_info(), igc);

  // Manually inspected output as ok. For testing, just check a single
  // value. If this fails, you may want to enable writing out the full
  // output and seeing what is going on.
  BOOST_CHECK_CLOSE(mp.read_double(473, 1000, 100, 100)(0, 0),
		    394.18319143107396, 1e-4);

  // Optionally write this out for manual inspection
  if(true) {
    VicarRasterImage out("grid_res.img", mp.map_info(), "HALF");
    mp.write_image(out, 10);
  }
}

BOOST_AUTO_TEST_SUITE_END()
