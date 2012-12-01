#include <iostream>
#include <cstdlib>
#include "gdal_dem.h"
#include "gdal_map_projected_image.h"
#include "gdal_raster_image.h"
#include "raster_averaged.h"
#include "memory_raster_image.h"
#include "geodetic.h"
#include "geocal_rpc.h"
#include "forstner_feature_detector.h"
#include "ccorr_lsm_matcher.h"
#include "rpc_image.h"
#include "image_point_display.h"
#include "mask_image.h"
#include "vicar_lite_file.h"
#include <boost/foreach.hpp>
using std::cerr;
using boost::shared_ptr;
using namespace GeoCal;

int main(int Argc, char** Argv)
{
  std::string basedir = "/Users/Smyth/AlStuff/";
  shared_ptr<Dem> dem(new VicarLiteDem(basedir + "china_aoi_wgs84.hlf"));
  std::string in_img_name = basedir + 
    "05NOV23034641-P1BS-005545406170_01_P001.NTF";
  Rpc rpc = gdal_rpc(in_img_name);
  VicarLiteMapProjectedImage ref_img(basedir + "china_aoi02.cib5");
  shared_ptr<RasterImage> in_img(new GdalRasterImage(in_img_name));
  MapInfo miref = ref_img.map_info();
// Full map, takes a very long time.
  MapInfo mifull(miref.coordinate_converter_ptr(), miref.ulc_x(),
		 miref.ulc_y(), miref.lrc_x(), miref.lrc_y(),
		 miref.number_x_pixel() * 10, miref.number_y_pixel() *
		 10);
  // Working with full map, picked a much smaller area to focus on.
  // Much quicker time to generate.
  double ulc_x = 110 + (59 + 11.42 / 60.0) / 60.0;
  double ulc_y = 34 + (30 + 53.86 / 60) / 60;
  int nxpix = 5000;
  int nypix = 3000;
  MapInfo mi(miref.coordinate_converter_ptr(), ulc_x, ulc_y,
	     ulc_x + mifull.pixel_x_resolution() * nxpix,
	     ulc_y + mifull.pixel_y_resolution() * nypix,
 	     nxpix, nypix);
  RpcImage proj(in_img, rpc, dem, mi);
  //  proj.map_info_bounding_update();
  GDALDriver* driver = (GDALDriver*) GDALGetDriverByName("hfa");
  shared_ptr<GDALDataset> gd(
    driver->Create("05NOV23_rpc_sub.img", proj.number_sample(),
		   proj.number_line(), 1, GDT_UInt16, 0));
  std::cerr << "First image...\n";
  GdalMapProjectedImage out_img(gd, proj.map_info());
  copy(proj, out_img);
  std::cerr << "Second image...\n";
  in_img_name = basedir + 
    "05SEP12034327-P1BS-005545406060_01_P001.NTF";
  rpc = gdal_rpc(in_img_name);
  shared_ptr<RasterImage> in_img2(new GdalRasterImage(in_img_name));
  RpcImage proj2(in_img2, rpc, dem, mi);
  //  proj2.map_info_bounding_update();
  shared_ptr<GDALDataset> gd2(
    driver->Create("05SEP12_rpc_sub.img", proj2.number_sample(),
		   proj2.number_line(), 1, GDT_UInt16, 0));
  std::cerr << "Second image...\n";
  GdalMapProjectedImage out_img2(gd2, proj2.map_info());
  copy(proj2, out_img2);
}


