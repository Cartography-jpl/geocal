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

inline double sqr(double x) {return x * x;}

int main(int Argc, char** Argv)
{
  GDALAllRegister();
  GDALDriver* driver = (GDALDriver*) GDALGetDriverByName("hfa");
  std::string basedir = "/data/L3/smyth/AlStuff/data/";
  std::string basedir2 = "/Users/Smyth/AlStuff/";
  std::cerr << "Read in Dem image\n";
  shared_ptr<Dem> dem(new VicarLiteDem(basedir2 + "qb1_iraq_dem84.hlf"));
  std::cerr << "Read in Ref image\n";
  VicarLiteMapProjectedImage ref_img(basedir2 + "b4482_3583_4509_3589.qcib");
  boost::shared_ptr<MapProjectedImage> 
  lw(new VicarLiteMapProjectedImage(basedir2 + "world_30as_lwm.img"));
  MaskImage mask(lw);
  //  CombinedMask mask;
  if(false) {
    shared_ptr<GDALDataset> gd(
    driver->Create("b4482_3583_4509_3589.img", ref_img.number_sample(),
		   ref_img.number_line(), 1, GDT_UInt16, 0));
    GdalMapProjectedImage out_img(gd, ref_img.map_info());
    copy(ref_img, out_img);
    return 0;
  }
  if(false) {
    GDALColorTable ct;
    GDALColorEntry transparent = {0, 0, 0, 0};
    ct.SetColorEntry(0, &transparent);
    GDALColorEntry blue = {0, 0, 255, 255};
    ct.SetColorEntry(1, &blue);
    shared_ptr<GDALDataset> gd(
    driver->Create("mask.img", ref_img.number_sample(),
		   ref_img.number_line(), 1, GDT_Byte, 0));
    gd->GetRasterBand(1)->SetColorTable(&ct);
    GdalMapProjectedImage out_img(gd, ref_img.map_info());
    for(int istart = 0; istart < out_img.number_line();
      istart += out_img.number_tile_line())
      for(int jstart = 0; jstart < out_img.number_sample();
	  jstart += out_img.number_tile_sample())
	for(int i = istart; i < istart + out_img.number_tile_line() &&
	      i < out_img.number_line(); ++i)
	  for(int j = jstart; j < jstart + out_img.number_tile_sample() &&
		j < out_img.number_sample(); ++j)
	    out_img.write(i, j, (mask.mask(*out_img.ground_coordinate(ImageCoordinate(i, j))) ? 1 : 0));
    return 0;
  }
  std::cerr << "Read in New image\n";
  std::string new_img_name = basedir2 + "03MAY05072951-P1BS-000000050393_01_P005.NTF";
  Rpc rpc = gdal_rpc(new_img_name);
  shared_ptr<RasterImage> new_img(new GdalRasterImage(new_img_name, 1, 8));
  RpcImage new_proj(new_img, rpc, dem, ref_img.map_info());
  new_proj.rpc().fit_line_numerator[0] = true;
  new_proj.rpc().fit_line_numerator[1] = true;
  new_proj.rpc().fit_line_numerator[2] = true;
  new_proj.rpc().fit_line_numerator[3] = true;
  new_proj.rpc().fit_sample_numerator[0] = true;
  new_proj.rpc().fit_sample_numerator[1] = true;
  new_proj.rpc().fit_sample_numerator[2] = true;
  new_proj.rpc().fit_sample_numerator[3] = true;
  std::cerr << new_proj.rpc() << "\n";
  int maxdiff = 1000;
//  int maxdiff = 10;
  new_proj.fit(ref_img, maxdiff, mask);
  new_proj.map_info_bounding_update();
  shared_ptr<GDALDataset> gd2(
    driver->Create("rpc_resample.img", new_proj.number_sample(),
		   new_proj.number_line(), 1, GDT_UInt16, 0));
  GdalMapProjectedImage out_img2(gd2, new_proj.map_info(), 1, 4,
				 new_proj.number_tile_line(),
				 new_proj.number_tile_sample());
  std::cerr << "Starting rpc_resample.img out\n";
  copy(new_proj, out_img2);
  shared_ptr<GDALDataset> gd3(
    driver->Create("rpc_image_point.img", new_proj.number_sample(),
		   new_proj.number_line(), 1, GDT_Byte, 0));
  std::cerr << "Starting rpc_image_point.img out\n";
  ImagePointDisplay id2(gd3, new_proj.map_info());
  for(int i = 0; i < new_proj.new_image_ground_point().size(); ++i)
    if(new_proj.blunder().count(i) ==0) {
      int Cindex = (int) floor(new_proj.distance_new_and_reference()[i] 
			       / 10.0) + 1;
      if(Cindex > 10)
	Cindex = 10;
      id2.image_point(*(new_proj.new_image_ground_point()[i]), Cindex);
    }
  shared_ptr<GDALDataset> gd4(
    driver->Create("ref_image_point.img", ref_img.number_sample(),
		   ref_img.number_line(), 1, GDT_Byte, 0));
  std::cerr << "Starting ref_image_point.img out\n";
  ImagePointDisplay id3(gd4, ref_img.map_info());
  std::vector<int> hist(10, 0);
  for(int i = 0; i < new_proj.new_image_ground_point().size(); ++i)
    if(new_proj.blunder().count(i) ==0) {
      int Cindex = (int) floor(new_proj.distance_new_and_reference()[i] / 
			       10.0) + 1;
      if(Cindex > 10)
	Cindex = 10;
      hist[Cindex - 1] += 1;
      id3.image_point(*(new_proj.reference_image_ground_point()[i]), Cindex);
    }
  std::vector<int> diagnostic(8, 0);
  BOOST_FOREACH(int d, new_proj.image_match_diagnostic())
    diagnostic[d] += 1;
  std::cerr << "Failed points: " << new_proj.failed_ground_point().size() 
	    << "\n";
  std::cerr << "To close to edge:   " << diagnostic[1] << "\n"
	    << "Variance to low:    " << diagnostic[2] << "\n"
	    << "Correlation to low: " << diagnostic[3] << "\n"
	    << "Exceed max sigma:   " << diagnostic[4] << "\n"
	    << "Exceed max rad var: " << diagnostic[5] << "\n"
	    << "Exceed prec req:    " << diagnostic[6] << "\n"
	    << "Move past target:   " << diagnostic[7] << "\n";
  for(int i = 0; i < new_proj.failed_ground_point().size(); 
      ++i) {
    int Cindex = 10 + new_proj.image_match_diagnostic()[i];
    if(Cindex < 10 || Cindex > 17)
      cerr << "Cindex: " << Cindex << "\n";
    else {
      id2.image_point(*(new_proj.failed_ground_point()[i]), Cindex);
      id3.image_point(*(new_proj.failed_ground_point()[i]), Cindex);
    }
  }
  std::cerr << "Histogram\n";
  for(int i = 0; i < 10; ++i)
    cerr << "> " << i * 10.0 << " : " << hist[i] << "\n";
}


