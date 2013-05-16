// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "gdal_multi_band.h"
%}

%geocal_shared_ptr(GeoCal::GdalMultiBand);
namespace GeoCal {
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
  void set_rpc(const Rpc& R) { gdal_rpc(*gd, R); }
  void set_map_info(const MapInfo& M) { gdal_map_info(*gd, M); }
  static void save(const std::string& Oname, 
		   const std::string& Driver,
		   const RasterImageMultiBand& d,
		   bool Log_progress = false,
		   int Dt = GDT_Byte,
		   const std::string& Options="");
  boost::shared_ptr<GdalRasterImage> gdal_raster_image(int band) const;
  const boost::shared_ptr<GDALDataset>& data_set() const;
  void flush() const;
  void close() const;
};
}
