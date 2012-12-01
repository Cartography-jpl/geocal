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
#include "vicar_lite_file.h"
#include <boost/foreach.hpp>

using std::cerr;
using boost::shared_ptr;
using namespace GeoCal;

inline double sqr(double x) {return x * x;}
int main(int Argc, char** Argv)
{
  std::string basedir = "/data/L3/smyth/AlStuff/data/";
  boost::shared_ptr<Datum> datum(new DidDatum("/data/bank/anc/DID/MeanSeaLevel/current/geoid_2160h_4320w.half"));
  GdalMapProjectedImage ref_img(basedir + "qb1_cib_erdas.img");
  boost::shared_ptr<VicarLiteRasterImage> 
    new_img_raw(new VicarLiteRasterImage(basedir + "newiraq"));
  boost::shared_ptr<RasterImage> new_img(new
    SubRasterImage(new_img_raw, 0, 0, new_img_raw->number_line(), 5500));
  Rpc rpc = gdal_rpc(basedir + 
 		     "03MAY05072951-P1BS-000000050393_01_P005.NTF");
  rpc.line_offset = 2934.19;
  rpc.sample_offset = 13049;
  RpcImage new_proj(new_img,
		    rpc, 
		    boost::shared_ptr<Dem>(new
		    GdalDem(basedir + "qb1_iraq_dem.img", datum)),
		    ref_img.map_info());
  new_proj.map_info_bounding_update();
  Rpc rpc1 = rpc;
  rpc1.line_numerator[0] = 0.0174748;
  rpc1.line_numerator[1] = -0.148568;
  rpc1.line_numerator[2] = -1.13393;
  rpc1.line_numerator[3] = -0.0135206;
  rpc1.line_numerator[4] =  0.003585;
  rpc1.sample_numerator[0] = -0.0104666;
  rpc1.sample_numerator[1] =  0.967772;
  rpc1.sample_numerator[2] =  -0.000429609;
  rpc1.sample_numerator[3] =  -0.0343213;
  rpc1.sample_numerator[4] =   -0.00101994;
  Rpc rpc2 = rpc;
  GDALDriver* driver = (GDALDriver*) GDALGetDriverByName("hfa");
  shared_ptr<GDALDataset> gd(
  driver->Create("diff.img", new_proj.number_sample(),
		   new_proj.number_line(), 3, GDT_Float32, 0));
  Gdal<float> tot(gd, 1);
  tot.map_info(new_proj.map_info());
  Gdal<float> ln(gd, 2);
  Gdal<float> smp(gd, 3);
  blitz::Array<double, 2> lat(new_proj.number_line(), new_proj.number_sample());
  blitz::Array<double, 2> lon(lat.shape());
  blitz::Array<double, 2> height(lat.shape());
  for(int i = 0; i < new_proj.number_line(); ++i)
    for(int j = 0; j < new_proj.number_sample(); ++j) {
      boost::shared_ptr<GroundCoordinate> gc = 
	new_proj.ground_coordinate(ImageCoordinate(i, j), new_proj.dem());
      lat(i, j) = gc->latitude();
      lon(i, j) = gc->longitude();
      height(i, j) = gc->height_reference_surface();
    }
  blitz::Array<double, 3> ic1 = rpc1.image_coordinate(lat, lon, height);
  blitz::Array<double, 3> ic2 = rpc2.image_coordinate(lat, lon, height);
  for(int i = 0; i < new_proj.number_line(); i += tot.tile_size()[0])
    for(int j = 0; j < new_proj.number_sample(); j += tot.tile_size()[1]) {
      for(int k = i; k < new_proj.number_line() && k < i + tot.tile_size()[0]; 
	  ++k)
	for(int l = j; l < new_proj.number_sample() && 
	      l < j + tot.tile_size()[1]; ++l) {
	  double l1 = ic1(0, k, l);
	  double s1 = ic1(1, k, l);
	  double l2 = ic2(0, k, l);
	  double s2 = ic2(1, k, l);
	  if(l1 < 0 || l1 >= new_img->number_line() ||
	     l2 < 0 || l2 >= new_img->number_line() ||
	     s1 < 0 || s1 >= new_img->number_sample() ||
	     s2 < 0 || s2 >= new_img->number_sample()) {
	    tot(k, l) = 0.0;
	    ln(k, l) = 0.0;
	    smp(k, l) = 0.0;
	  } else {
	    tot(k, l) = sqrt(sqr(l1 - l2) +
			     sqr(s1 - s2));
	    ln(k, l) = l1 - l2;
	    smp(k, l) = s1 - s2;
	  }
	}
    }
  
}
	    
      
  
  
