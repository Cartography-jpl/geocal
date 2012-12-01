// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "geocal_gdal.h"
%}

%geocal_shared_ptr(GdalBase);
%shared_ptr(GDALDataset);
%nodefaultctor GDALDataset;
class GDALDataset {
  virtual ~GDALDataset();
  int GetRasterXSize();
  int GetRasterYSize();
  int GetRasterCount();
};

namespace GeoCal {
#define GDT_BYTE GDT_Byte
#define GDT_UINT16 GDT_UInt16 
#define GDT_INT16 GDT_Int16,
#define GDT_UINT32 GDT_UInt32
#define GDT_INT32 GDT_Int32
#define GDT_FLOAT32 GDT_Float32
#define GDT_FLOAT64 GDT_Float64
class GdalBase {
public:
  enum {};
  %python_attribute(data_set, boost::shared_ptr<GDALDataset>)
  %python_attribute_with_set(map_info, MapInfo)
  %python_attribute_nonconst(raster_band, GDALRasterBand&)
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

boost::shared_ptr<GDALDataset> gdal_create_copy(const std::string& Fname, 
 const std::string& Driver_name, 
 const GDALDataset& Source_dataset,
 const std::string& Options, bool Log_progress = false);

boost::shared_ptr<GDALDataset> 
GeoCal::gdal_create_erdas(const std::string& Fname, 
			  const GDALDataset& Source_dataset,
			  bool Log_progress = false);

}
