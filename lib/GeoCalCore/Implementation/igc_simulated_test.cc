#include "unit_test_support.h"
#include "igc_simulated.h"
#include "vicar_lite_file.h"
#include "gdal_raster_image.h"
#include "rpc_image_ground_connection.h"
#ifdef HAVE_VICAR_RTL
#include "vicar_raster_image.h"
#endif

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(igc_simulated, GlobalFixture)
BOOST_AUTO_TEST_CASE(full_res)
{
  boost::shared_ptr<Dem> 
    dem(new VicarLiteDem(stereo_test_data_dir() + "nevada_elv_aoi.img", true));
  boost::shared_ptr<RasterImage> 
    img(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-1.img"));
  boost::shared_ptr<ImageGroundConnection>
    igc(new RpcImageGroundConnection(img->rpc(), dem, img));
  boost::shared_ptr<RasterImage> 
    simg(new GdalRasterImage(stereo_test_data_dir() + 
			     "10MAY21-1_projected.tif"));
  IgcSimulated igc_sim(igc, simg);

  // Manually inspected output as ok. For testing, just check a single
  // value. If this fails, you may want to enable writing out the full
  // output and seeing what is going on.
  BOOST_CHECK_CLOSE(igc_sim.read_double(473, 1000, 5, 5)(0, 0),
  		    378.45949734573293, 1e-4);
  // Optionally write this out for manual inspection
  if(false) {
#ifdef HAVE_VICAR_RTL
    VicarRasterImage out("full_res.img", "HALF", igc_sim.number_line(),
			 igc_sim.number_sample());
    copy(igc_sim, out);
#endif
  }
}

BOOST_AUTO_TEST_SUITE_END()
