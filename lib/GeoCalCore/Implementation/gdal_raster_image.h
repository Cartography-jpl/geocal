#ifndef GDAL_RASTER_IMAGE_H
#define GDAL_RASTER_IMAGE_H
#include "raster_image_tiled_file.h"
#include "raster_image_multi_band.h"
#include "geocal_gdal.h"
#include "ostream_pad.h"

namespace GeoCal {
/****************************************************************//**
  This wraps a Gdal<T> to make a RasterImage.

  The type of T should be one we can convert to and from a int, since
  this is what a RasterImage works in.
*******************************************************************/

class GdalRasterImage : public RasterImageTiledFile {
public:
  // Enumerations primarily meant for use in SWIG
  enum {Byte = GDT_Byte, UInt16 = GDT_UInt16, Int16 = GDT_Int16,
	UInt32 = GDT_UInt32, Int32 = GDT_Int32, Float32 = GDT_Float32,
	Float64 = GDT_Float64 };
  GdalRasterImage(const boost::shared_ptr<GDALDataset>&
		  Data_set, int Band_id = 1, int Number_tile = 4,
		  int Tile_number_line = -1, int Tile_number_sample = -1);
  GdalRasterImage(const std::string& Fname, int Band_id = 1, int
		  Number_tile = 4, bool Update = false,
		  int Tile_number_line = -1, int Tile_number_sample = -1);
  GdalRasterImage(const std::string& Fname, int Band_id,
		  const std::string& Allowed_drivers,
		  const std::string& Open_options="", const std::string& Sibling_files="",
		  int Number_tile = 4, bool Update = false,
		  int Tile_number_line = -1, int Tile_number_sample = -1);
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

  virtual bool copy_needs_double() const
  { return (raster_data_type() == Float32 || raster_data_type() == Float64); }
  
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  template<class T> GdalRasterImage(const boost::shared_ptr<Gdal<T> >& Data)
  : RasterImageTiledFile(Data),
    gdal_data_base_(Data)
  {
    if(gdal_data_base_->has_map_info())
      map_info_.reset(new MapInfo(gdal_data_base_->map_info()));
    if(gdal_data_base_->has_rpc())
      rpc_.reset(new Rpc(gdal_data_base_->rpc()));
    // No RSM support yet
  }
  void set_map_info(const MapInfo& Mi);
  boost::shared_ptr<MapInfo> map_info_from_nitf_corner(bool Approx_ok = false) const;
  void set_rpc(const Rpc& R);
  std::vector<std::string> file_names() const 
  { return gdal_data_base().file_names(); }
  int band_id() const 
  { return const_cast<GDALRasterBand*>(&raster_band())->GetBand(); }
  int raster_data_type() const
  { return const_cast<GDALRasterBand*>(&raster_band())->GetRasterDataType(); }

//-----------------------------------------------------------------------
/// Set fill value
//-----------------------------------------------------------------------
  void set_fill_value(double Fill_value)
  { raster_band().SetNoDataValue(Fill_value); }

//-----------------------------------------------------------------------
/// Set scale value
//-----------------------------------------------------------------------
  void set_scale(double Scale_value)
  { raster_band().SetScale(Scale_value); }

//-----------------------------------------------------------------------
/// Set offset value
//-----------------------------------------------------------------------
  void set_offset(double Offset_value)
  { raster_band().SetOffset(Offset_value); }

//-----------------------------------------------------------------------
/// Write a GdalRasterImage to a file with a given driver name. This
/// helps handle drivers that only support create_copy (like COG). We
/// create GdalRasterImage with a different driver (often "MEM"), and
/// then use this function to create an output file
//-----------------------------------------------------------------------

  void write_gdal(const std::string& Fname, const std::string& Driver_name,
		  const std::string& Options="") const
  { gdal_create_copy(Fname, Driver_name, *data_set(), Options); }
  
//-----------------------------------------------------------------------
/// Return offset, if any to apply to data to get underlying values.
//-----------------------------------------------------------------------
  double offset() const
  { return const_cast<GDALRasterBand*>(&raster_band())->GetOffset();}

//-----------------------------------------------------------------------
/// Return scale, if any to apply to data to get underlying values.
//-----------------------------------------------------------------------
  double scale() const
  { return const_cast<GDALRasterBand*>(&raster_band())->GetScale();}

//-----------------------------------------------------------------------
/// Return unit type of underlying values.
//-----------------------------------------------------------------------
  std::string unit_type() const
  { return const_cast<GDALRasterBand*>(&raster_band())->GetUnitType();}

  bool update() const
  { return const_cast<GDALRasterBand*>(&raster_band())->GetAccess() == GA_Update; }

  virtual boost::shared_ptr<RasterImage> overview(int Min_number_sample) const;

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GdalRasterImage() {}

//-----------------------------------------------------------------------
// Close file. Normally you don't need to call this, the destructor
// does this automatically. But this is useful to have for use by
// Ruby.
//
// Note that this deletes the reference to the pointer to the
// GDALDataset. If this is the only reference, that closes the file.
// If there are other copies around the file isn't actually closed.
// The way this is used from Ruby this doesn't matter - we only have
// one copy of a GDALDataset at a time.
//-----------------------------------------------------------------------

  void close() { flush(); gdal_data_base_->close(); }

//-----------------------------------------------------------------------
// True if file is already closed.
//-----------------------------------------------------------------------

  inline bool is_closed() const {return gdal_data_base_->is_closed();}

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

  const GdalBase& gdal_data_base() const {return *gdal_data_base_;}
  boost::shared_ptr<GdalBase> gdal_data_base_ptr() const 
  {return gdal_data_base_;}

//-----------------------------------------------------------------------
/// The underlying dataset object.
//-----------------------------------------------------------------------

  inline const boost::shared_ptr<GDALDataset>& data_set() const 
  { return gdal_data_base_->data_set(); }


//-----------------------------------------------------------------------
/// Return true if we have GCPs.
//-----------------------------------------------------------------------

  bool has_gcps() const
  { return data_set()->GetGCPCount() > 0; }
  blitz::Array<double, 2> gcps() const;

//-----------------------------------------------------------------------
/// The underlying GDALRasterBand object.
//-----------------------------------------------------------------------

  inline GDALRasterBand& raster_band() { return gdal_data_base_->raster_band(); }

//-----------------------------------------------------------------------
/// The underlying GDALRasterBand object.
//-----------------------------------------------------------------------

  inline const GDALRasterBand& raster_band() const { return gdal_data_base_->raster_band(); }

//-----------------------------------------------------------------------
/// Flush data to disk
//-----------------------------------------------------------------------

  virtual void flush() const 
  { 
    if(!is_closed()) {
      RasterImageTiledFile::flush(); 
      gdal_data_base_->flush(); 
    }
  }

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

  GdalBase& gdal_data_base() {return *gdal_data_base_;}

//-----------------------------------------------------------------------
/// Return the list of metadata keys for a given domain
//-----------------------------------------------------------------------

  std::vector<std::string> metadata_list(const std::string& Domain = "") const
  { return gdal_data_base().metadata_list(Domain); }
  
//-----------------------------------------------------------------------
/// Indicates if a metadata item is found in the file
//-----------------------------------------------------------------------

  bool has_metadata(const std::string& Metadata_item,
		    const std::string& Domain = "") const
  { return gdal_data_base().has_metadata(Metadata_item, Domain); }

//-----------------------------------------------------------------------
/// Read a metadata item and cast it to the given type.
//-----------------------------------------------------------------------

  template<class S> S metadata(const std::string& Metadata_item,
			       const std::string& Domain = "") const
  { return gdal_data_base().metadata<S>(Metadata_item, Domain); }

//-----------------------------------------------------------------------
/// Write the requested metadata.
//-----------------------------------------------------------------------

  void set_metadata(const std::string& M, const std::string& Val,
		    const std::string& Domain = "")
  { gdal_data_base().set_metadata(M, Val, Domain); }

//-----------------------------------------------------------------------
/// Return Rpc for image.
//-----------------------------------------------------------------------

  Rpc rpc() const {return gdal_data_base_->rpc();}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  {
    OstreamPad opad(Os, "    ");
    Os << "GDAL Raster Image:\n"
       << "  File:          " << gdal_data_base() << "\n"
       << "  Band:          " << band_id() << "\n"
       << "  Number line:   " << number_line() << "\n"
       << "  Number sample: " << number_sample() << "\n";
    Os << "  Map Info:      ";
    if(has_map_info()) {
      Os << "\n";
      opad << map_info();
      opad.strict_sync();
    } else
      Os << "None\n";
    Os << "  RPC:           ";
    if(has_rpc()) {
      Os << "\n";
      opad << rpc();
      opad.strict_sync();
    } else
      Os << "None\n";
    Os << "  RSM:           ";
    if(has_rsm()) {
      Os << "\n";
      opad << *rsm();
      opad.strict_sync();
    } else
      Os << "None\n";
  }

  static void save_to_erdas(const std::string& Oname, 
			    const RasterImage& Mi, 
			    int Dt = GDT_Byte,
			    int Fill_value = 0);
  static void save_to_erdas(const std::string& Oname, 
			    const RasterImage& Mi_r, 
			    const RasterImage& Mi_g, 
			    const RasterImage& Mi_b, 
			    int Dt = GDT_Byte,
			    int Fill_value = 0);
  static void save_to_erdas(const std::string& Oname, 
			    const RasterImage& Mi_r, 
			    const RasterImage& Mi_g, 
			    const RasterImage& Mi_b, 
			    const RasterImage& Mi_ir, 
			    int Dt = GDT_Byte,
			    int Fill_value = 0);
  static void save_to_erdas(const std::string& Oname, 
	    const std::vector<boost::shared_ptr<RasterImage> >& d,
	    int Dt = GDT_Byte,
	    int Fill_value = 0);
  static void save_to_erdas(const std::string& Oname, 
	    const RasterImageMultiBand& d,
	    int Dt = GDT_Byte,
	    int Fill_value = 0);

//-----------------------------------------------------------------------
/// Copy the given RasterImage into a new file, using the given
/// driver and options. Some drivers only allow "CreateCopy" rather
/// than "Create", requiring us to use an intermediate memory version
/// of Gdal. In that case, set "require_copy" to true.
//-----------------------------------------------------------------------

  static void save(const std::string& Oname,
		   const std::string& Driver,
		   const RasterImage& Mi, 
		   int Dt = GDT_Byte,
		   const std::string& Option = "",
		   bool Require_copy = false, int Fill_value = -1000)
  {
    std::vector<const RasterImage*> d;
    d.push_back(&Mi);
    save(Oname, Driver, d, Dt, Option, Require_copy, Fill_value);
  }
  
//-----------------------------------------------------------------------
/// Copy the given RasterImages into a new file, using the given
/// driver and options. Some drivers only allow "CreateCopy" rather
/// than "Create", requiring us to use an intermediate memory version
/// of Gdal. In that case, set "require_copy" to true.
//-----------------------------------------------------------------------

  static void save(const std::string& Oname,
		   const std::string& Driver,
		   const RasterImage& Mi_r, 
		   const RasterImage& Mi_g, 
		   const RasterImage& Mi_b, 
		   int Dt = GDT_Byte,
		   const std::string& Option = "",
		   bool Require_copy = false, int Fill_value = -1000)
  {
    std::vector<const RasterImage*> d;
    d.push_back(&Mi_r);
    d.push_back(&Mi_g);
    d.push_back(&Mi_b);
    save(Oname, Driver, d, Dt, Option, Require_copy, Fill_value);
  }

  static void save(const std::string& Oname,
		   const std::string& Driver,
		   const std::vector<const RasterImage*>& d,
		   int Dt = GDT_Byte,
		   const std::string& Option = "",
		   bool Require_copy = false,
		   int Fill_value = -1000);
protected:
  GdalRasterImage(const boost::shared_ptr<GDALDataset>&
		  Data_set, GDALRasterBand* Rb, int Number_tile = 4,
		  int Tile_number_line = -1, int Tile_number_sample = -1);
private:
  boost::shared_ptr<GdalBase> gdal_data_base_; 
				///< Underlying data.

//-----------------------------------------------------------------------
/// Type dispatched initializization
//-----------------------------------------------------------------------

  template<class T> void initialize(const boost::shared_ptr<GDALDataset>&
				    Data_set, int Band_id, 
				    int Number_tile, int Tile_number_line,
				    int Tile_number_sample)
  {
    boost::shared_ptr<Gdal<T> > t(new Gdal<T>(Data_set, Band_id, Number_tile, 
					      Tile_number_sample, 
					      Tile_number_sample));
    RasterImageTiledFile::initialize(t);
    gdal_data_base_ = t;
    if(gdal_data_base_->has_map_info())
      map_info_.reset(new MapInfo(gdal_data_base_->map_info()));
    if(gdal_data_base_->has_rpc())
      rpc_.reset(new Rpc(gdal_data_base_->rpc()));
    // No RSM support yet
  }

  template<class T> void initialize(const boost::shared_ptr<GDALDataset>&
				    Data_set, GDALRasterBand* Rb,
				    int Number_tile, int Tile_number_line,
				    int Tile_number_sample)
  {
    boost::shared_ptr<Gdal<T> > t(new Gdal<T>(Data_set, Rb, Number_tile, 
					      Tile_number_sample, 
					      Tile_number_sample));
    RasterImageTiledFile::initialize(t);
    gdal_data_base_ = t;
    if(gdal_data_base_->has_map_info())
      map_info_.reset(new MapInfo(gdal_data_base_->map_info()));
    if(gdal_data_base_->has_rpc())
      rpc_.reset(new Rpc(gdal_data_base_->rpc()));
    // No RSM support yet
  }

  template<class T> void initialize(const std::string& Fname,
				    int Band_id, 
				    int Number_tile, bool Update,
				    int Tile_number_line, 
				    int Tile_number_sample)
  {
    boost::shared_ptr<Gdal<T> > t(new Gdal<T>(Fname, Band_id, Number_tile, 
					      Update, Tile_number_line,
					      Tile_number_sample));
    RasterImageTiledFile::initialize(t);
    gdal_data_base_ = t;
    if(gdal_data_base_->has_map_info())
      map_info_.reset(new MapInfo(gdal_data_base_->map_info()));
    if(gdal_data_base_->has_rpc())
      rpc_.reset(new Rpc(gdal_data_base_->rpc()));
    // No RSM support yet
  }
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

// This is a little more complicated, because we can't really
// construct a object using a default constructor. So we need to
// directly handle the object construction.
namespace boost { namespace serialization {
template<class Archive> 
void save_construct_data(Archive & ar, const GeoCal::GdalRasterImage* d, 
				const unsigned int version);
template<class Archive>
void load_construct_data(Archive & ar, GeoCal::GdalRasterImage* d,
			 const unsigned int version);
  }
}

GEOCAL_EXPORT_KEY(GdalRasterImage);
#endif
