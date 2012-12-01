// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "vicar_raster_image.h"
%}

%geocal_shared_ptr(VicarRasterImage);
namespace GeoCal {
class VicarRasterImage : public RasterImageTiledFile {
public:
  typedef VicarFile::access_type access_type;
  typedef VicarFile::compression compression;
  VicarRasterImage(const std::string& Fname, 
		   access_type Access = VicarFile::READ,
		   int Number_line_per_tile = 100, int Number_tile = 4);
  VicarRasterImage(const std::string& Fname, 
		   const std::string& Type,
		   int Number_line, int Number_sample,
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE);
  VicarRasterImage(const std::string& Fname, 
		   const MapInfo& M,
		   const std::string& Type = "BYTE",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE);
  VicarRasterImage(int Instance, access_type Access = VicarFile::READ, 
		   const std::string& Name = "INP",
		   int Number_line_per_tile = 100, int Number_tile = 4);
  VicarRasterImage(int Instance, 
		   const std::string& Type,
		   int Number_line, int Number_sample,
		   const std::string& Name = "OUT",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE);
  VicarRasterImage(int Instance, const MapInfo& M,
		   const std::string& Type = "BYTE",
		   const std::string& Name = "OUT",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE);
  %python_attribute2(vicar_file, vicar_file_ptr, boost::shared_ptr<VicarFile>)
  virtual void flush() const;
  void close();
  void set_rpc(const Rpc& R);
  void set_map_info(const MapInfo& Mi);
  %python_attribute(rpc, Rpc)
  %python_attribute(map_info, MapInfo)
  %pickle_init(self.vicar_file.file_name, self.vicar_file.access,
	       self.number_tile_line, self.number_tile)
  %pythoncode {
@rpc.setter
def rpc(self, val):
   self.set_rpc(val)

@map_info.setter
def map_info(self, val):
   self.set_map_info(val)

def __getitem__(self, key):
    if(isinstance(key, list) or isinstance(key, tuple)):
      prop, ky = key
      return self.vicar_file.label_string(ky, prop)
    else:
      return self.vicar_file.label_string(key)

def __setitem__(self, key, v):
    if(isinstance(key, list) or isinstance(key, tuple)):
      prop, ky = key
      self.vicar_file.label_set(ky, v, prop)
    else:
      self.vicar_file.label_set(key, v)
    
  }
};
}
