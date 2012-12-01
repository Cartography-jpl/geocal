#ifndef GDAL_MULTI_BAND_H
#define GDAL_MULTI_BAND_H
#include "raster_image_multi_band_variable.h"
#include "gdal_raster_image.h"
#include "geocal_gdal.h"

namespace GeoCal {
/****************************************************************//**
  This is a set of Gdal RasterImages, one for each band.
*******************************************************************/

class GdalMultiBand : public RasterImageMultiBandVariable {
public:
  GdalMultiBand(const std::string& Fname, int Number_tile = 4,
		int Tile_number_line = -1, int Tile_number_sample = -1);
  GdalMultiBand(const std::string& Fname,
		const std::string& Driver_name,
		const GdalRasterImage& Source,
		const std::string& Options = "", 
		bool Log_progress = false,
		int Number_tile = 4,
		int Tile_number_line = -1, int Tile_number_sample = -1);
  GdalMultiBand(const std::string& Fname,
		const std::string& Driver_name,
		const GdalMultiBand& Source,
		const std::string& Options = "", 
		bool Log_progress = false,
		int Number_tile = 4,
		int Tile_number_line = -1, int Tile_number_sample = -1);
  GdalMultiBand(const std::string& Fname, const std::string& 
		Driver_name, int Number_line, int Number_sample,
		int Number_band,
		int Dt, const std::string& Options="", 
		int Number_tile = 4,
		int Tile_number_line = -1, int Tile_number_sample = -1);
  virtual ~GdalMultiBand() {}
  void set_rpc(const Rpc& R) { gdal_rpc(*gd, R); }
  void set_map_info(const MapInfo& M) { gdal_map_info(*gd, M); }
  static void save(const std::string& Oname, 
		   const std::string& Driver,
		   const RasterImageMultiBand& d,
		   bool Log_progress = false,
		   int Dt = GDT_Byte,
		   const std::string& Options="");
  void flush() const;
  void close() const;

//-----------------------------------------------------------------------
/// RasterImage, cast to GdalRasterImage.
//-----------------------------------------------------------------------
  boost::shared_ptr<GdalRasterImage> gdal_raster_image(int band) const
  { return 
      boost::dynamic_pointer_cast<GdalRasterImage>(raster_image_ptr(band)); }

//-----------------------------------------------------------------------
/// The underlying dataset object.
//-----------------------------------------------------------------------

  const boost::shared_ptr<GDALDataset>& data_set() const {return gd;}
private:
  boost::shared_ptr<GDALDataset> gd;
};
}
#endif

