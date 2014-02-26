#ifndef IMAGE_POINT_DISPLAY_H
#define IMAGE_POINT_DISPLAY_H
#include "gdal_raster_image.h"

namespace GeoCal {
/****************************************************************//**
  This class is used to generate a display of image points. This is
  useful for example to show the difference between initial and final
  location during image matching
*******************************************************************/

class ImagePointDisplay : public GdalRasterImage {
public:
//-----------------------------------------------------------------------
/// Constructor to write a new file.
//-----------------------------------------------------------------------

  ImagePointDisplay(const boost::shared_ptr<GDALDataset>&
		    Data_set, int Band_id = 1, int Number_tile = 4)
    : GdalRasterImage(Data_set, Band_id, Number_tile)
  { 
  }

//-----------------------------------------------------------------------
/// Constructor to write a new file.
//-----------------------------------------------------------------------

  ImagePointDisplay(const std::string& Fname, const std::string& 
		    Driver_name, const MapInfo& M, int Dt,
		    const std::string& Options = "", 
		    int Number_tile = 4,
		    int Tile_number_line = -1, int Tile_number_sample = -1)
  : GdalRasterImage(Fname, Driver_name, M, 1, Dt, Options, Number_tile,
		    Tile_number_line, Tile_number_sample)
  { }

//-----------------------------------------------------------------------
/// Constructor to write a new file.
//-----------------------------------------------------------------------

  ImagePointDisplay(const std::string& Fname, const std::string& 
		    Driver_name, int Number_line, int Number_sample, int Dt,
		    const std::string& Options = "", 
		    int Number_tile = 4,
		    int Tile_number_line = -1, int Tile_number_sample = -1)
    : GdalRasterImage(Fname, Driver_name, Number_line, Number_sample, 1, 
		      Dt, Options, Number_tile,
		      Tile_number_line, Tile_number_sample)
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ImagePointDisplay() {}

  void image_point(const ImageCoordinate& Ic, int Cindex);

//-----------------------------------------------------------------------
/// Display Point with the given color index.
//-----------------------------------------------------------------------

  void image_point(const GroundCoordinate& Gc, int Cindex)
  { image_point(coordinate(Gc), Cindex); }
private:
  void initialize();
};
}
#endif
