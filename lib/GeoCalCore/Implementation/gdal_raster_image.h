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
    read_all(const std::string& Fname);

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
  }
  void set_map_info(const MapInfo& Mi);
  boost::shared_ptr<MapInfo> map_info_from_nitf_corner(bool Approx_ok = false) const;
  void set_rpc(const Rpc& R);
  std::vector<std::string> file_names() const 
  { return gdal_data_base().file_names(); }
  int band_id() const 
  { return const_cast<GDALRasterBand*>(&raster_band())->GetBand(); }
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
  }
};
}
#endif
