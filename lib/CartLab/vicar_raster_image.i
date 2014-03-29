// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "vicar_raster_image.h"
#include "ecr.h"
#include "image_ground_connection.h"
%}
%base_import(raster_image_tiled_file)
%import "vicar_file.i"
%geocal_shared_ptr(GeoCal::VicarRasterImage);
namespace GeoCal {
class VicarRasterImage : public RasterImageTiledFile {
public:
  typedef VicarFile::access_type access_type;
  typedef VicarFile::compression compression;
  VicarRasterImage(const std::string& Fname, int Band_id = 1,
		   access_type Access = VicarFile::READ,
		   int Number_line_per_tile = 100, int Number_tile = 4,
		   bool Force_area_pixel = false);
  VicarRasterImage(const std::string& Fname, 
		   const std::string& Type,
		   int Number_line, int Number_sample,
		   int Number_band = 1,
		   const std::string& Org = "BSQ",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE);
  VicarRasterImage(const std::string& Fname, 
		   const MapInfo& M,
		   const std::string& Type = "BYTE",
		   int Number_band = 1,
		   const std::string& Org = "BSQ",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE);
  VicarRasterImage(int Instance, int Band_id = 1,
		   access_type Access = VicarFile::READ, 
		   const std::string& Name = "INP",
		   int Number_line_per_tile = 100, int Number_tile = 4);
  VicarRasterImage(int Instance, 
		   const std::string& Type,
		   int Number_line, int Number_sample,
		   int Number_band = 1,
		   const std::string& Org = "BSQ",
		   const std::string& Name = "OUT",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE);
  VicarRasterImage(int Instance, const MapInfo& M,
		   const std::string& Type = "BYTE",
		   int Number_band = 1,
		   const std::string& Org = "BSQ",
		   const std::string& Name = "OUT",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE);
  %python_attribute2(vicar_file, vicar_file_ptr, boost::shared_ptr<VicarFile>)
  virtual void flush() const;
  void close();
  void set_rpc(const Rpc& R);
  void set_map_info(const MapInfo& Mi);
  %python_attribute(number_band, int)
  %python_attribute(band_id, int)
  %python_attribute(rpc, Rpc)
  %python_attribute(map_info, MapInfo)
  %pickle_init(2, self.vicar_file.file_name, self.band_id,
	       self.vicar_file.access,
	       self.number_tile_line, self.number_tile, 
	       self.vicar_file.force_area_pixel)
  %pythoncode {
@rpc.setter
def rpc(self, val):
   self.set_rpc(val)

@map_info.setter
def map_info(self, val):
   self.set_map_info(val)

def __getitem__(self, key):
    return self.vicar_file[key]

def __setitem__(self, key, v):
    self.vicar_file[key] = v
    
  }
};
}
