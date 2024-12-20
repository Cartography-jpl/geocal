// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "vicar_lite_file.h"
#include "ecr.h"
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%base_import(raster_image)
%base_import(dem_map_info)
%import "map_info.i"
%import "geocal_rpc.i"
%import "rsm.i"
%geocal_shared_ptr(GeoCal::VicarLiteFile);
%geocal_shared_ptr(GeoCal::VicarLiteRasterImage);
%geocal_shared_ptr(GeoCal::VicarLiteDem);

namespace GeoCal {
class VicarLiteFile: public GenericObject {
public:
  enum data_type {VICAR_BYTE, VICAR_HALF, VICAR_FULL, VICAR_FLOAT,
		  VICAR_DOUBLE};
  enum access_type {READ, WRITE, UPDATE};
  VicarLiteFile(const std::string& Fname, access_type Access = READ,
		bool Force_area_pixel = false);
  // Because SWIG squashes some types together, it can't tell the
  // difference between this constructor and the read
  // constructor. Remove the default Type, and it can then tell the
  // difference
  // VicarLiteFile(const std::string& Fname, int Number_line, int Number_sample,
  //	    const std::string& Type = "BYTE");
  VicarLiteFile(const std::string& Fname, int Number_line, int Number_sample,
		const std::string& Type);
  ~VicarLiteFile();
  %python_attribute(access, access_type)
  %python_attribute(force_area_pixel, bool)
  %python_attribute(data_offset, int)
  %python_attribute(file_name, std::string)
  static bool is_vicar_file(const std::string& Fname);
  %python_attribute(has_igc_glas_gfm, bool)
  %python_attribute(igc_glas_gfm, boost::shared_ptr<ImageGroundConnection>);
  %python_attribute(number_line, int)
  %python_attribute(number_sample, int)
  %python_attribute(number_band, int)
  %python_attribute(number_line_binary, int)
  %python_attribute(number_byte_binary, int)
  %python_attribute(type, data_type)
  %python_attribute_with_set(map_info, MapInfo)
  %python_attribute_with_set(rpc, Rpc)
  %python_attribute_with_set(rsm, boost::shared_ptr<Rsm>)
  std::string print_to_string() const;
  %python_attribute(is_compressed, bool)
  %extend {
     std::string label_string(const std::string& N)
	{return $self->label<std::string>(N);}
     std::vector<std::string> label_list()
     { std::vector<std::string>  k;
       for(std::map<std::string, std::string>::const_iterator i = $self->label_map().begin(); i != $self->label_map().end(); ++i)
         k.push_back((*i).first);
       return k;
    }
  }
  %pickle_serialization();
};

class VicarLiteRasterImage : public RasterImage {
public:
  typedef VicarLiteFile::access_type access_type;
  VicarLiteRasterImage(const std::string& Fname, int Band_id = 1,
		       access_type Access = VicarLiteFile::READ,
		       int Number_tile_line = -1,
		       int Number_tile_sample = -1,
		       bool Force_area_pixel = false);
  VicarLiteRasterImage(const std::string& Fname, 
		       const MapInfo& Mi,
		       int Band_id = 1,
		       access_type Access = VicarLiteFile::READ,
		       int Number_tile_line = -1,
		       int Number_tile_sample = -1,
		       bool Force_area_pixel = false);
  %python_attribute2(file, file_ptr, boost::shared_ptr<VicarLiteFile>)
  %python_attribute(is_compressed, bool)
  %python_attribute(band_id, int)
  %python_attribute(force_map_info, bool)
  %python_attribute(has_igc_glas_gfm, bool)
  %python_attribute(igc_glas_gfm, boost::shared_ptr<ImageGroundConnection>);
  %pickle_serialization();
};

class VicarLiteDem : public DemMapInfo {
public:
  VicarLiteDem(const std::string& Fname, 
	       bool Outside_dem_is_error = false,
	       const boost::shared_ptr<Datum>& D = 
	       boost::shared_ptr<Datum>(new NoDatum()),
	       int Band = 0);
  %python_attribute2(file, file_ptr, boost::shared_ptr<VicarLiteFile>)
  virtual double elevation(int Y_index, int X_index) const;
  %python_attribute(band, int)
  %pickle_serialization();
};

}

// List of things "import *" will include
%python_export("VicarLiteFile", "VicarLiteRasterImage", "VicarLiteDem")
