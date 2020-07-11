// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

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
  VicarRasterImage(const boost::shared_ptr<VicarFile>& Vicar_file, 
		   int Band_id = 1,
		   int Number_line_per_tile = 100, int Number_tile = 4);
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
  void set_rsm(const boost::shared_ptr<Rsm>& R,
	       VicarFile::rsm_file_type File_type = VicarFile::RSM_NITF_FILE);
  void set_igc_glas_gfm(const boost::shared_ptr<ImageGroundConnection>& Igc,
			VicarFile::glas_gfm_file_type File_type =
			VicarFile::GLAS_GFM_NITF_FILE);
  void set_map_info(const MapInfo& Mi);
  %python_attribute(has_igc_glas_gfm, bool);
  %python_attribute(igc_glas_gfm, boost::shared_ptr<ImageGroundConnection>);
  %python_attribute(number_band, int);
  %python_attribute(band_id, int);
  %python_attribute(rpc, Rpc);
  %python_attribute(map_info, MapInfo);
  %pickle_serialization();
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

boost::shared_ptr<RasterImage> 
  vicar_open(const std::string& Fname, int Band_id = 1,
	     VicarFile::access_type Access = VicarFile::READ,
	     bool Favor_memory_mapped = true,
	     int Number_line_per_tile = 100,
	     int Number_tile = 4,
	     bool Force_area_pixel = false);
}

// List of things "import *" will include
%python_export("VicarRasterImage", "vicar_open")
