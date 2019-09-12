// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "gdal_raster_image.h"
#include "ecr.h"
#include "image_ground_connection.h"
#include "raster_image_multi_band_variable.h"
%}
%base_import(raster_image_tiled_file)
%import "raster_image_multi_band.i"
%import "geocal_gdal.i"
%geocal_shared_ptr(GeoCal::GdalRasterImage);
namespace GeoCal {
template<class T> class Gdal;

class GdalRasterImage : public RasterImageTiledFile {
public:
  enum { Byte, UInt16, Int16, UInt32, Int32, Float32,
         Float64 };
  // See note in geocal_gdal on GDALDataset
  // GdalRasterImage(const boost::shared_ptr<GDALDataset>&
  //    Data_set, int Band_id = 1, int Number_tile = 4,
  //    int Tile_number_line = -1, int Tile_number_sample = -1);
  GdalRasterImage(const std::string& Fname, const std::string& 
		  Driver_name, int Number_line, int Number_sample,
		  int Number_band,
    		  int Dt, const std::string& Options="", int Number_tile = 4,
		  int Tile_number_line = -1, int Tile_number_sample = -1);
  GdalRasterImage(const std::string& Fname, const std::string& 
		  Driver_name, const MapInfo& Mi, int Number_band,
		  int Dt, const std::string& Options = "", 
		  int Number_tile = 4,
		  int Tile_number_line = -1, int Tile_number_sample = -1);
  GdalRasterImage(const boost::shared_ptr<Gdal<GByte> >& Data);
  GdalRasterImage(const boost::shared_ptr<Gdal<GUInt16> >& Data);
  GdalRasterImage(const boost::shared_ptr<Gdal<GInt16> >& Data);
  GdalRasterImage(const boost::shared_ptr<Gdal<GUInt32> >& Data);
  GdalRasterImage(const boost::shared_ptr<Gdal<GInt32> >& Data);
  GdalRasterImage(const boost::shared_ptr<Gdal<float> >& Data);
  GdalRasterImage(const boost::shared_ptr<Gdal<double> >& Data);
  GdalRasterImage(const std::string& Fname, int Band_id = 1, int
     Number_tile = 4, bool Update = false,
     int Tile_number_line = -1, int Tile_number_sample = -1);
  GdalRasterImage(const std::string& Fname,
		  const std::string& Driver_name,
		  const GdalRasterImage& Source,
		  const std::string& Options = "", 
		  bool Log_progress = false,
		  int Band_id = 1,
		  int Number_tile = 4,
		  int Tile_number_line = -1, int Tile_number_sample = -1);
  static std::vector<boost::shared_ptr<RasterImage> > 
    read_all_bands(const std::string& Fname);
  %python_attribute2(gdal_data_base, gdal_data_base_ptr, 
		     boost::shared_ptr<GdalBase>)
  // See note in geocal_gdal on GDALDataset
   // %python_attribute(data_set, boost::shared_ptr<GDALDataset>)
  virtual void flush() const;
  void close();
  %python_attribute(is_closed, bool)
  void set_map_info(const MapInfo& Mi);
  void set_rpc(const Rpc& R);
  std::vector<std::string> metadata_list(const std::string& Domain = "") const;
  bool has_metadata(const std::string& M,
		    const std::string& Domain = "");
  void set_metadata(const std::string& M, const std::string& Val,
		    const std::string& Domain = "");
  %python_attribute(file_names, std::vector<std::string>)
  boost::shared_ptr<MapInfo> map_info_from_nitf_corner(bool Approx_ok = false) const;
  %python_attribute(band_id, int)
  %python_attribute(offset, double)
  %python_attribute(scale, double)
  %python_attribute(unit_type, std::string)
  %python_attribute(raster_data_type, int)
  %python_attribute(update, bool)
  %python_attribute(rpc, Rpc)
  %python_attribute(map_info, MapInfo)
  %python_attribute(has_gcps, bool)
  %python_attribute(gcps, blitz::Array<double, 2>)
  %pythoncode {
@rpc.setter
def rpc(self, val):
   self.set_rpc(val)

@map_info.setter
def map_info(self, val):
   self.set_map_info(val)

def __getitem__(self, key):
    if(isinstance(key, list) or isinstance(key, tuple)):
      domain, ky = key
      return self.metadata(ky, domain)
    else:
      return self.metadata(key)

def __setitem__(self, key, v):
    if(isinstance(key, list) or isinstance(key, tuple)):
      domain, ky = key
      self.set_metadata(ky, v, domain)
    else:
      self.set_metadata(key, v)

def __contains__(self, key):
    if(isinstance(key, list) or isinstance(key, tuple)):
      domain, ky = key
      return self.has_metadata(ky, domain)
    else:
      return self.has_metadata(key)
    
  }
 %extend {
     std::string metadata(const std::string& Metadata_item) const
     {
        return $self->metadata<std::string>(Metadata_item);
     }
     std::string metadata(const std::string& Metadata_item, const std::string& D) const
     {
        return $self->metadata<std::string>(Metadata_item, D);
     }
     static void save_to_file(const char* Oname, const RasterImage& Ri, 
			      const char* Type, int Out_type = GDT_UInt16)
     {
        GDALDriver* driver = (GDALDriver*) GDALGetDriverByName(Type);
	if(!driver) {
	  GeoCal::Exception e("Unrecognized type ");
	  e << Type;
	  throw e;
        }
        boost::shared_ptr<GDALDataset> gd(
	    driver->Create(Oname, Ri.number_sample(),
			   Ri.number_line(), 1, GDALDataType(Out_type), 0));
        if(!gd.get()) {
            GeoCal::Exception e;
            e << "Creating GDAL file " << Oname << " failed";
            throw e;
        }
        GeoCal::GdalRasterImage out_img(gd);
	if(Ri.has_map_info())
	  out_img.set_map_info(Ri.map_info());
	if(Ri.has_rpc())
	  out_img.set_rpc(Ri.rpc());
	// Just leave RSM off, we don't have to write out. This
	// should perhaps be treated as an error, but for now just
	// drop RSM
	// if(Ri.has_rsm())
	//   throw GeoCal::Exception("Don't support RSM yet");
        copy(Ri, out_img);
     }
  }
  static void save_to_erdas(const std::string& Oname, 
   const RasterImage& Mi, int Dt = GDT_Byte, int Fill_value = 0);
  static void save_to_erdas(const std::string& Oname, 
   const RasterImage& Mi_r, 
   const RasterImage& Mi_g, 
   const RasterImage& Mi_b, 
   int Dt = GDT_Byte, int Fill_value = 0);
  static void save_to_erdas(const std::string& Oname, 
   const RasterImage& Mi_r, 
   const RasterImage& Mi_g, 
   const RasterImage& Mi_b, 
   const RasterImage& Mi_ir, 
   int Dt = GDT_Byte, int Fill_value = 0);
  static void save_to_erdas(const std::string& Oname, 
	    const std::vector<boost::shared_ptr<GeoCal::RasterImage> >& d,
	    int Dt = GDT_Byte,
	    int Fill_value = 0);
  static void save_to_erdas(const std::string& Oname, 
	    const RasterImageMultiBand& d,
	    int Dt = GDT_Byte,
	    int Fill_value = 0);
  static void save(const std::string& Oname,
		   const std::string& Driver,
		   const RasterImage& Mi, 
		   int Dt = GDT_Byte,
		   const std::string& Option = "",
		   bool Require_copy = false,
		   int Fill_value = -1000);
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("GdalRasterImage")
