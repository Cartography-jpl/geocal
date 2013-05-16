// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "image_point_display.h"
#include "ecr.h"
#include "raster_image_multi_band_variable.h"
#include "image_ground_connection.h"
%}
%base_import(gdal_raster_image)
%geocal_shared_ptr(GeoCal::ImagePointDisplay);
namespace GeoCal {
class ImagePointDisplay : public GdalRasterImage {
public:
  ImagePointDisplay(const boost::shared_ptr<GDALDataset>&
		    Data_set, int Band_id = 1, 
		    int Number_tile = 4);
  ImagePointDisplay(const std::string& Fname, const std::string& 
		    Driver_name, const MapInfo& M, int Dt,
		    const std::string& Options = "", 
		    int Number_tile = 4,
		    int Tile_number_line = -1, int Tile_number_sample = -1);
  ImagePointDisplay(const std::string& Fname, const std::string& 
		    Driver_name, int Number_line, int Number_sample, int Dt,
		    const std::string& Options = "", 
		    int Number_tile = 4,
		    int Tile_number_line = -1, int Tile_number_sample = -1);
  void image_point(const ImageCoordinate& Ic, int Cindex);
  void image_point(const GroundCoordinate& Gc, int Cindex);
};

}
