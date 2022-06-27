// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_gdal.h"
#include "ecr.h"
#include "image_ground_connection.h"
%}

%base_import(generic_object)
%import "map_info.i"
%import "geocal_rpc.i"
%import "ground_coordinate.i"
%import "ecr.i"
%geocal_shared_ptr(GeoCal::GdalBase);
// GDALDataset causes SWIG 2.09 to segment fault. Not sure why, this
// seems like a clear bug (SWIG should report an error if there is a
// problem). But I don't think we actually need or use GDALDataset, so
// as an easy workaround just remove all references to it.
//%shared_ptr(GDALDataset);
//%nodefaultctor GDALDataset;
// class GDALDataset {
//   virtual ~GDALDataset();
//   int GetRasterXSize();
//   int GetRasterYSize();
//   int GetRasterCount();
// };

namespace GeoCal {
#define GDT_BYTE GDT_Byte
#define GDT_UINT16 GDT_UInt16 
#define GDT_INT16 GDT_Int16,
#define GDT_UINT32 GDT_UInt32
#define GDT_INT32 GDT_Int32
#define GDT_FLOAT32 GDT_Float32
#define GDT_FLOAT64 GDT_Float64
class GdalBase : public GenericObject {
public:
  enum {};
  // %python_attribute(data_set, boost::shared_ptr<GDALDataset>)
  %python_attribute_with_set(map_info, MapInfo)
  %python_attribute_nonconst(raster_band, GDALRasterBand&)
  %python_attribute(linear_unit_name, std::string)
  %python_attribute(linear_unit_scale, double)
  void close();
  %python_attribute(is_closed, bool)
  void flush();
  %python_attribute(rpc, Rpc)
  std::string print_to_string() const;
  %extend {
     static int driver_count() { return GDALGetDriverCount();}
     static const char* driver_short_name(int i)
      { return GDALGetDriverShortName(GDALGetDriver(i));}
     static const char* driver_long_name(int i)
      { return GDALGetDriverLongName(GDALGetDriver(i));}
     static const char* driver_help_url(int i)
      { return GDALGetDriverHelpTopic(GDALGetDriver(i));}
     static const char* driver_option_list(int i)
      { return GDALGetDriverCreationOptionList(GDALGetDriver(i));}
  }
};

// boost::shared_ptr<GDALDataset> gdal_create_copy(const std::string& Fname, 
//  const std::string& Driver_name, 
//  const GDALDataset& Source_dataset,
//  const std::string& Options, bool Log_progress = false);

// boost::shared_ptr<GDALDataset> 
// GeoCal::gdal_create_erdas(const std::string& Fname, 
// 			  const GDALDataset& Source_dataset,
// 			  bool Log_progress = false);

  std::string gdal_driver_name(const std::string& Fname);
}


// List of things "import *" will include
%python_export("GdalBase", "gdal_driver_name")
