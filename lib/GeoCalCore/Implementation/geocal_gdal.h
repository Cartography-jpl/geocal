#ifndef GEOCAL_GDAL_H
#define GEOCAL_GDAL_H
#include "tiled_file.h"
#include "map_info.h"
#include "geocal_rpc.h"
// These are defined in the build, and result in a warning when they
// conflict with the values in gdal_priv. To keep from getting this
// warning, we just undef them before including the file.
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#include <gdal_priv.h>
#include <ogr_spatialref.h>	
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

namespace GeoCal {
  namespace GdalRegister {
    void gdal_register();
  }

  namespace GdalType {
    template<class T> int gdal_data_type();
    template<> inline int gdal_data_type<GByte>() { return GDT_Byte; }
    template<> inline int gdal_data_type<GUInt16>() {return GDT_UInt16;}
    template<> inline int gdal_data_type<GInt16>() {return GDT_Int16;}
    template<> inline int gdal_data_type<GUInt32>() {return GDT_UInt32;}
    template<> inline int gdal_data_type<GInt32>() {return GDT_Int32;}
    template<> inline int gdal_data_type<float>() {return GDT_Float32;}
    template<> inline int gdal_data_type<double>() {return GDT_Float64;}

// Not sure how to handle complex data. For now, we don't actually
// need this, so I'll just leave this commented out.

//     template<> inline int gdal_data_type<GDT_CInt16>() {return GDT_CInt16;}
//     template<> inline int gdal_data_type<GDT_CInt32>() {return GDT_CInt32;}
//     template<> inline int gdal_data_type<GDT_CFloat32>() {return GDT_CFloat32;}
//     template<> inline int gdal_data_type<GDT_CFloat64>() {return GDT_CFloat64;}

    template<class T> struct Conv {};
    template<> struct Conv<int> {
      static inline int conv(const char* M) {
	if(M[0] =='(')		// Skip leading "("
	  return atoi(M + 1);
	return atoi(M);
      }
    };
    template<> struct Conv<double> {
      static inline double conv(const char* M) {
	if(M[0] =='(')		// Skip leading "("
	  return atof(M + 1);
	return atof(M);
      }
    };
    template<> struct Conv<std::string> {
      static inline std::string conv(const char* M) {return M;}
    };
    template<class T, std::size_t D> struct Conv<boost::array<T, D> > {
      static inline boost::array<T, D> conv(const char* M)
      {
	std::istringstream is(M);
	boost::array<T, D> res;
	for(std::size_t i = 0; i < D; ++i)
	  is >> res[i];
	return res;
      }
    };
  }

boost::shared_ptr<GDALDataset> gdal_openex(const std::string& Fname,
   bool Update=false,					   
   const std::string& Allowed_drivers="", const std::string& Open_options="",
   const std::string& Sibling_files="");
GDALDataType gdal_band_type(const std::string& Fname, int Band_id = 1);
std::string gdal_driver_name(const std::string& Fname);
bool gdal_has_map_info(const std::string& Fname);
bool gdal_has_rpc(const GDALDataset& D);
Rpc gdal_rpc(const std::string& Fname);
Rpc gdal_rpc(const GDALDataset& D);
void gdal_rpc(GDALDataset& D, const Rpc& R);
// No RSM support yet
void gdal_map_info(GDALDataset& D, const MapInfo& M);
boost::shared_ptr<GDALDataset> gdal_create_dataset(const std::string& Fname, 
 const std::string& Driver_name, int Number_line, 
 int Number_sample, int Number_band,
 GDALDataType Dt, const std::string& Options);
boost::shared_ptr<GDALDataset> gdal_create_copy(const std::string& Fname, 
 const std::string& Driver_name, 
 const GDALDataset& Source_dataset,
 const std::string& Options, bool Log_progress = false);
boost::shared_ptr<GDALDataset> gdal_create_erdas(const std::string& Fname, 
	 const GDALDataset& Source_dataset, bool Log_progress = false);

/****************************************************************//**
  Base class of Gdal<T>. This contains the portion that doesn't depend
  on T.
*******************************************************************/

class GdalBase : public Printable<GdalBase> {
public:
  typedef TiledFileBase<2>::index index; ///<< Index type

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GdalBase() { }

//-----------------------------------------------------------------------
// Close a file. Normally you don't need to directly call this since
// this is automatically done by the destructor. But this is useful to
// have for use by Ruby.
//
// Note that this delete the reference to the pointer to the
// GDALDataset. If this is the only reference, that closes the file.
// If there are other copies around the file isn't actually closed.
// The way this is used from Ruby this doesn't matter - we only have
// one copy of a GDALDataset at a time.
//-----------------------------------------------------------------------

  inline void close() { 
    if(!is_closed()) {
      data_set_.reset((GDALDataset*) 0);
    }
  }

//-----------------------------------------------------------------------
// True if we are already closed.
//-----------------------------------------------------------------------

  inline bool is_closed() const { return data_set_.get() == 0; }

//-----------------------------------------------------------------------
/// The underlying dataset object.
//-----------------------------------------------------------------------

  inline const boost::shared_ptr<GDALDataset>& data_set() const 
  { return data_set_; }

//-----------------------------------------------------------------------
/// Flush data to disk
//-----------------------------------------------------------------------
  
  void flush() const 
  { 
    if(!is_closed())
      data_set_->FlushCache(); 
  }

  std::vector<std::string> file_names() const;
  virtual bool has_map_info() const;
  virtual bool has_rpc() const
  {
    return gdal_has_rpc(*data_set());
  }
  virtual MapInfo map_info() const;
  virtual void map_info(const MapInfo& M)
  {
    gdal_map_info(*data_set(), M);
  }
  std::string linear_unit_name() const;
  double linear_unit_scale() const;
  double no_data_value() const {return raster_band_->GetNoDataValue();}
  
//-----------------------------------------------------------------------
/// The underlying GDALRasterBand object.
//-----------------------------------------------------------------------

  inline GDALRasterBand& raster_band() { return *raster_band_; }
  inline GDALRasterBand* raster_band_ptr() { return raster_band_; }

//-----------------------------------------------------------------------
/// The underlying GDALRasterBand object.
//-----------------------------------------------------------------------

  inline const GDALRasterBand& raster_band() const { return *raster_band_; }

//-----------------------------------------------------------------------
/// Get a list of metadata keys for a given domain.
//-----------------------------------------------------------------------

  std::vector<std::string> metadata_list(const std::string& Domain = "") const;
  
//-----------------------------------------------------------------------
/// This is a thin wrapper around GDALGetMetadataItem(). We determine if
/// requested metadata can be read.
//-----------------------------------------------------------------------

  bool has_metadata(const std::string& M,
		    const std::string& Domain = "") const
  {
    if(is_closed())
      throw Exception("File has already been closed");
    const char* v = data_set()->GetMetadataItem(M.c_str(),Domain.c_str());
    return (!v ? false : true);
  }

//-----------------------------------------------------------------------
/// This is a thin wrapper around GDALGetMetadataItem(). We read the
/// requested metadata, and cast to the desired type. A
/// MetadataMissing is thrown if we don't find the requested metadata.
//-----------------------------------------------------------------------

  template<class S> S metadata(const std::string& M,
			   const std::string& Domain = "") const
  {
    if(is_closed())
      throw Exception("File has already been closed");
    const char* v = data_set()->GetMetadataItem(M.c_str(),Domain.c_str());
    if(!v)
      throw MetadataMissing("Metadata " + M + " isn't found in the file.");
    return GdalType::Conv<S>::conv(v);
  }

//-----------------------------------------------------------------------
/// This is a thin wrapper around GDALSetMetadataItem(). We write
/// the requested metadata.
//-----------------------------------------------------------------------

  void set_metadata(const std::string& M, const std::string& Val,
		    const std::string& Domain = "")
  {
    if(is_closed())
      throw Exception("File has already been closed");
    data_set()->SetMetadataItem(M.c_str(), Val.c_str(), Domain.c_str());
  }

//-----------------------------------------------------------------------
/// Read metadata to determine Rpc. 
//-----------------------------------------------------------------------

  inline Rpc rpc() const
  {
    if(is_closed())
      throw Exception("File has already been closed");
    return gdal_rpc(*data_set());
  }
  void rpc(const Rpc& R)
  {
    if(is_closed())
      throw Exception("File has already been closed");
    gdal_rpc(*data_set(), R);
  }
  virtual void print(std::ostream& Os) const;
protected:
  std::vector<std::string> fnames_; ///< We'll replace this with
				    ///directly getting this from GDAL
				    ///once the GDAL library supports
				    ///this. 
//-----------------------------------------------------------------------
/// Underlying dataset.
//-----------------------------------------------------------------------

  mutable boost::shared_ptr<GDALDataset> data_set_;

//-----------------------------------------------------------------------
/// Underlying raster band. Note that although we have a pointer, the
/// GDALDataset "owns" this - we don't do anything to handle the
/// cleanup.
//-----------------------------------------------------------------------

  mutable GDALRasterBand* raster_band_;

//-----------------------------------------------------------------------
/// Size of file.
//-----------------------------------------------------------------------

};
  
/****************************************************************//**
  Read and write files supported by GDAL library.

  The GDAL library is a library that supports a number of raster
  formats. It is the library used by GRASS, although it is separate
  and you don't need to use GRASS or even have it installed to use
  GDAL. There is a large number of formats supported, including ERDAS
  imagine, Arc/Info, TIFF and GeoTIFF, NITF, HDF, netCDF, and many
  others. Documentation can be found at http://www.gdal.org.

  This class provides a generic TiledFile interface. Other classes can
  read additional metadata to support additional features (e.g.,
  create a Dem or RasterImage).

  Note that the GDAL library reads each band separate. So if you have
  multiple bands, you'll need to create multiple Gdals.

  Note when using MapInfo, we have X (e.g., longitude) going in the
  sample direction, and Y (e.g., latitude) going in the line
  direction. This means that when reading data which is in line,
  sample order you need to swap these - e.g., we have (Y_index,
  X_index).
*******************************************************************/

template<class T> class Gdal: public TiledFile<T, 2>,
  public GdalBase {
public:
  typedef GdalBase::index index;

//-----------------------------------------------------------------------
/// Constructor to read and/or write a new file (or an existing file where
/// you want to share the GDALDataset with some other object). This
/// constructor doesn't try to actually create the file, that is done
/// outside of this class. Normally, you just call GDALDriver::Create for
/// whatever driver you are using (see the GDAL documentation at the web
/// site listed above for details)
//-----------------------------------------------------------------------

  Gdal(const boost::shared_ptr<GDALDataset>& Data_set, int Band_id =
       1, int Number_tile = 4, int Tile_number_line = -1, int 
       Tile_number_sample = -1)
  { data_set_ = Data_set; initialize(Band_id, Number_tile, Tile_number_line,
				     Tile_number_sample); }


  Gdal(const boost::shared_ptr<GDALDataset>& Data_set, GDALRasterBand* Rb, 
       int Number_tile = 4, int Tile_number_line = -1, int 
       Tile_number_sample = -1)
  { data_set_ = Data_set; initialize(Rb, Number_tile, Tile_number_line,
				     Tile_number_sample); }

//-----------------------------------------------------------------------
/// Constructor to create a new file. This is a thin wrapper around
/// the GDAL library that gets the requested driver and creates the
/// file with the given options. 
///
/// Because the option interface of GDAL is a bit awkward to use, we
/// supply an interface that takes a single string, with the options
/// separated by " ", so for example for a TIFF file we might have
/// "TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024"
//-----------------------------------------------------------------------

  Gdal(const std::string& Fname, 
    const std::string& Driver_name, int Number_line, 
       int Number_sample, int Number_band,
       const std::string& Options, int Number_tile = 4, 
       int Tile_number_line = -1, int Tile_number_sample = -1)
  {
    data_set_ = gdal_create_dataset(Fname, Driver_name, Number_line,
				    Number_sample, Number_band,
				    (GDALDataType) GdalType::gdal_data_type<T>(),
				    Options);
    initialize(1, Number_tile, Tile_number_line,
	       Tile_number_sample);
  }

//-----------------------------------------------------------------------
/// Constructor to read and/or update an existing file.
//-----------------------------------------------------------------------

  Gdal(const std::string& Fname, int Band_id = 1, int Number_tile = 4, 
       bool Update = false, int Tile_number_line = -1, 
       int Tile_number_sample = -1)
  {
    fnames_.push_back(Fname);
    GdalRegister::gdal_register();
    data_set_.reset((GDALDataset *) GDALOpen(Fname.c_str(), (Update ? GA_Update : GA_ReadOnly) ));
    if(!data_set_.get())
      throw Exception("Trouble opening file " + Fname);
    initialize(Band_id, Number_tile, Tile_number_line, Tile_number_sample);
  }

//-----------------------------------------------------------------------
/// Variation of constructor where we specify more options, using
/// GDALOpenEx instead of GDALOpen.
//-----------------------------------------------------------------------

  Gdal(const std::string& Fname, int Band_id, const std::string& Allowed_drivers,
       const std::string& Open_options="", const std::string& Sibling_files="",
       int Number_tile = 4, 
       bool Update = false, int Tile_number_line = -1, 
       int Tile_number_sample = -1)
  {
    fnames_.push_back(Fname);
    GdalRegister::gdal_register();
    data_set = gdal_openex(Fname, Update, Allowed_drivers, Open_options, Sibling_files);
    initialize(Band_id, Number_tile, Tile_number_line, Tile_number_sample);
  }
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Gdal() { flush(); }

//-----------------------------------------------------------------------
/// Flush data to disk
//-----------------------------------------------------------------------

  virtual void flush() const {
    if(!is_closed()) {
      TiledFile<T, 2>::flush();
      data_set_->FlushCache();
    }
  }

//-----------------------------------------------------------------------
/// True if we have map info.
//-----------------------------------------------------------------------

  virtual bool has_map_info() const
  { return GdalBase::has_map_info(); }

//-----------------------------------------------------------------------
/// True if we have RPC.
//-----------------------------------------------------------------------

  virtual bool has_rpc() const
  { return GdalBase::has_rpc(); }

//-----------------------------------------------------------------------
/// We can set MapInfo for this kind of file.
//-----------------------------------------------------------------------

  virtual bool can_set_map_info() const { return true; }

//-----------------------------------------------------------------------
/// Return MapInfo. 
//-----------------------------------------------------------------------

  virtual MapInfo map_info() const {return GdalBase::map_info(); }

//-----------------------------------------------------------------------
/// Set MapInfo. 
//-----------------------------------------------------------------------

  virtual void map_info(const MapInfo& M)
  { GdalBase::map_info(M); }
protected:

//-----------------------------------------------------------------------
/// Constructor that defers initialization. Derived classes should fill in
/// data_set_ and call initialize before exiting.
//-----------------------------------------------------------------------

  inline Gdal() {}

//-----------------------------------------------------------------------
/// Initialize object. This should only be called after data_set_ has been
/// filled in.
//-----------------------------------------------------------------------

  void initialize(int Band_id, int Number_tile, int Tile_number_line,
		  int Tile_number_sample)
  {
    range_check(Band_id, 1, data_set_->GetRasterCount() + 1);
    raster_band_ = data_set_->GetRasterBand(Band_id);
    if(raster_band_->GetRasterDataType() !=
       (GDALDataType) GdalType::gdal_data_type<T>())
      throw Exception("GetRasterDataType doesn't match type T");
    int block_x, block_y;
    raster_band().GetBlockSize(&block_x, &block_y);
    boost::array<index, 2> size = {{raster_band().GetYSize(),
				    raster_band().GetXSize()}};
    boost::array<index, 2> tile_size = {{block_y, block_x}};
    if(Tile_number_line > 0)
      tile_size[0] = Tile_number_line;
    if(Tile_number_sample > 0)
      tile_size[1] = Tile_number_sample;
    TiledFile<T, 2>::initialize(size, tile_size, Number_tile);
  }

  void initialize(GDALRasterBand* Rb, int Number_tile, int Tile_number_line,
		  int Tile_number_sample)
  {
    raster_band_ = Rb;
    if(raster_band_->GetRasterDataType() !=
       (GDALDataType) GdalType::gdal_data_type<T>())
      throw Exception("GetRasterDataType doesn't match type T");
    int block_x, block_y;
    raster_band().GetBlockSize(&block_x, &block_y);
    boost::array<index, 2> size = {{raster_band().GetYSize(),
				    raster_band().GetXSize()}};
    boost::array<index, 2> tile_size = {{block_y, block_x}};
    if(Tile_number_line > 0)
      tile_size[0] = Tile_number_line;
    if(Tile_number_sample > 0)
      tile_size[1] = Tile_number_sample;
    TiledFile<T, 2>::initialize(size, tile_size, Number_tile);
  }

//-----------------------------------------------------------------------
/// Read a tile of data.
//-----------------------------------------------------------------------

  virtual void read_tile(const boost::array<index, 2>& Min_index, 
			 const boost::array<index, 2>& Max_index, 
			 T* Res) const
  {
    if(is_closed())
      throw Exception("File is already closed");
    CPLErr status =
      raster_band_->RasterIO(GF_Read, Min_index[1], Min_index[0], 
			     Max_index[1] - Min_index[1],
			     Max_index[0] - Min_index[0],
			     (void*) Res,
			     Max_index[1] - Min_index[1],
			     Max_index[0] - Min_index[0],
			     (GDALDataType) GdalType::gdal_data_type<T>(), 
			     0, 0);
    if(status ==CE_Failure)
      throw Exception("Read failed for GDAL");
  }

//-----------------------------------------------------------------------
/// Write a tile of data.
//-----------------------------------------------------------------------

  virtual void write_tile(const boost::array<index, 2>& Min_index, 
			  const boost::array<index, 2>& Max_index, 
			  const T* V) const
  {
    if(is_closed())
      throw Exception("File is already closed");
    CPLErr status =
      raster_band_->RasterIO(GF_Write, Min_index[1], Min_index[0], 
			     Max_index[1] - Min_index[1],
			     Max_index[0] - Min_index[0],
			     (void *) V,
			     Max_index[1] - Min_index[1],
			     Max_index[0] - Min_index[0],
			     (GDALDataType) GdalType::gdal_data_type<T>(), 
			     0, 0);
    if(status ==CE_Failure)
      throw Exception("Read failed for GDAL");
  }
};

//-----------------------------------------------------------------------
/// \ingroup Gdal
/// This is a thin wrapper around GDALGetMetadataItem(). We read the
/// requested metadata, and cast to the desired type. A
/// MetadataMissing is thrown if we don't find the requested metadata.
//-----------------------------------------------------------------------

  template<class S> S gdal_metadata(const GDALDataset& D, const std::string& M, const std::string& Domain = "")
{
  const char* v = const_cast<GDALDataset&>(D).GetMetadataItem(M.c_str(), Domain.c_str());
  if(!v)
    throw MetadataMissing("Metadata " + M + " isn't found in the file.");
  return GdalType::Conv<S>::conv(v);
}

}
#endif
