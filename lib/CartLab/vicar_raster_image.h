#ifndef VICAR_RASTER_IMAGE_H
#define VICAR_RASTER_IMAGE_H
#include "raster_image_tiled_file.h"
#include "vicar_tiled_file.h"

namespace GeoCal {
/****************************************************************//**
  This wraps a VicarTiledFile<T> to make a RasterImage.

  The type of T should be one we can convert to and from a int, since
  this is what a RasterImage works in.
*******************************************************************/

class VicarRasterImage : public RasterImageTiledFile {
public:
  typedef VicarFile::access_type access_type;
  typedef VicarFile::compression compression;

//-----------------------------------------------------------------------
/// Open an existing VICAR file for reading or update.
///
/// The Force_area_pixel forces the file to be treated as
/// "pixel as area" rather than "pixel as point". This is really just
/// meant as a work around for the SRTM data, which incorrectly labels
/// the data as "point" rather than "area". Since this is a 15 meter
/// difference, it matters for may applications. Most users should
/// just ignore this value.
//-----------------------------------------------------------------------

  VicarRasterImage(const std::string& Fname, int Band_id = 1,
		   access_type Access = VicarFile::READ,
		   int Number_line_per_tile = 100, int Number_tile = 4,
		   bool Force_area_pixel = false)
    : vicar_file_(new VicarFile(Fname, Access, Force_area_pixel))
  {
    initialize(Band_id, Number_line_per_tile, Number_tile);
  }

//-----------------------------------------------------------------------
/// Open a VicarRasterImage from an already open VicarFile
//-----------------------------------------------------------------------

  VicarRasterImage(const boost::shared_ptr<VicarFile>& Vicar_file, 
		   int Band_id = 1,
		   int Number_line_per_tile = 100, int Number_tile = 4)
    : vicar_file_(Vicar_file)
  {
    initialize(Band_id, Number_line_per_tile, Number_tile);
  }
  
//-----------------------------------------------------------------------
/// Create a new VICAR file with the given size.
//-----------------------------------------------------------------------

  VicarRasterImage(const std::string& Fname, 
		   const std::string& Type,
		   int Number_line, int Number_sample,
		   int Number_band = 1,
		   const std::string& Org = "BSQ",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE)
    : vicar_file_(new VicarFile(Fname, Number_line, Number_sample, 
				Number_band, Type, Org, C))
  {
// I believe the number of tiles must be 1 for writing, since it seems
// like VICAR is restricted to writing out the output sequentially.
    initialize(1, Number_line_per_tile, 1);
  }

//-----------------------------------------------------------------------
/// Create a new VICAR file with the given size.
//-----------------------------------------------------------------------

  VicarRasterImage(const std::string& Fname, 
		   const MapInfo& M,
		   const std::string& Type = "BYTE",
		   int Number_band = 1,
		   const std::string& Org = "BSQ",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE)
    : vicar_file_(new VicarFile(Fname, M.number_y_pixel(), 
				M.number_x_pixel(), Number_band, Type, Org, C))
  {
    vicar_file_->map_info(M);
    
// I believe the number of tiles must be 1 for writing, since it seems
// like VICAR is restricted to writing out the output sequentially.
    initialize(1, Number_line_per_tile, 1);
  }

//-----------------------------------------------------------------------
/// Open a file, using the VICAR Name and Instance input (so for
/// example, "INP" and 2 is the second INP file passed to a VICAR program.
//-----------------------------------------------------------------------

  VicarRasterImage(int Instance, int Band_id = 1,
		   access_type Access = VicarFile::READ, 
		   const std::string& Name = "INP",
		   int Number_line_per_tile = 100, int Number_tile = 4)
    : vicar_file_(new VicarFile(Instance, Access, Name))
  {
    initialize(Band_id, Number_line_per_tile, Number_tile);
  }

//-----------------------------------------------------------------------
/// Create a new VICAR file with the given size. Use the VICAR Name
/// and Instance input (so for example, "INP" and 2 is the second INP
/// file passed to a VICAR program. 
//-----------------------------------------------------------------------

  VicarRasterImage(int Instance, 
		   const std::string& Type,
		   int Number_line, int Number_sample,
		   int Number_band = 1,
		   const std::string& Org = "BSQ",
		   const std::string& Name = "OUT",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE)
    : vicar_file_(new VicarFile(Instance, Number_line, Number_sample, 
				Number_band, Type, Name, Org, C))
  {
// I believe the number of tiles must be 1 for writing, since it seems
// like VICAR is restricted to writing out the output sequentially.
    initialize(1, Number_line_per_tile, 1);
  }

//-----------------------------------------------------------------------
/// Create a new VICAR file with the given size. Use the VICAR Name
/// and Instance input (so for example, "INP" and 2 is the second INP
/// file passed to a VICAR program. 
//-----------------------------------------------------------------------

  VicarRasterImage(int Instance, const MapInfo& M,
		   const std::string& Type = "BYTE",
		   int Number_band = 1,
		   const std::string& Org = "BSQ",
		   const std::string& Name = "OUT",
		   int Number_line_per_tile = 100,
		   compression C = VicarFile::NONE)
    : vicar_file_(new VicarFile(Instance, M.number_y_pixel(), 
				M.number_x_pixel(), Number_band,
				Type, Name, "BSQ", C))
  {
    vicar_file_->map_info(M);

// I believe the number of tiles must be 1 for writing, since it seems
// like VICAR is restricted to writing out the output sequentially.

    initialize(1, Number_line_per_tile, 1);
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~VicarRasterImage() {}

//-----------------------------------------------------------------------
/// Close a file. You don't normally need to call this directly, it
/// is done by the destructor. But it is useful to have for use by Ruby.
//-----------------------------------------------------------------------

  void close()
  { RasterImageTiledFile::flush(); vicar_file().close(); }

//-----------------------------------------------------------------------
/// Flush data to disk
//-----------------------------------------------------------------------

  virtual void flush() const 
  { RasterImageTiledFile::flush(); vicar_file().flush(); }

//-----------------------------------------------------------------------
/// Underlying file.
//-----------------------------------------------------------------------

  const VicarFile& vicar_file() const {return *vicar_file_;}
  
//-----------------------------------------------------------------------
/// Underlying file.
//-----------------------------------------------------------------------

  VicarFile& vicar_file() {return *vicar_file_;}
  boost::shared_ptr<VicarFile> vicar_file_ptr() const {return vicar_file_;}

//-----------------------------------------------------------------------
/// Number of bands in file.
//-----------------------------------------------------------------------

  int number_band() const {return vicar_file_->number_band(); }

//-----------------------------------------------------------------------
/// Band id we are reading. Note that following the GDAL convention, this
/// is 1 based.
//-----------------------------------------------------------------------

  int band_id() const {return band_id_;}

//-----------------------------------------------------------------------
/// Set the RPC.
//-----------------------------------------------------------------------

  void set_rpc(const Rpc& R) { 
    vicar_file_->rpc(R); rpc_.reset(new Rpc(R));
  }

//-----------------------------------------------------------------------
/// Set the Map Info.
//-----------------------------------------------------------------------

  void set_map_info(const MapInfo& Mi) { 
    vicar_file_->map_info(Mi); 
    map_info_.reset(new MapInfo(vicar_file_->map_info()));
  }
private:
  boost::shared_ptr<VicarFile> vicar_file_; 
				///< Underlying data.
  int band_id_;
  void initialize(int Band_id, int Number_line_per_tile, int Number_tile);

//-----------------------------------------------------------------------
/// Type dispatched initialization
//-----------------------------------------------------------------------

  template<class T> void initialize_t(int Band_id,
				      int Number_line_per_tile, 
				      int Number_tile)
  {
    range_check(Band_id, 1, number_band() + 1);
    band_id_ = Band_id;
    boost::shared_ptr<VicarTiledFile<T> > 
      t(new VicarTiledFile<T>(vicar_file_, band_id_,
			      Number_line_per_tile, Number_tile));
    RasterImageTiledFile::initialize(t);
    if(vicar_file_->has_map_info())
      map_info_.reset(new MapInfo(vicar_file_->map_info()));
    if(vicar_file_->has_rpc())
      rpc_.reset(new Rpc(vicar_file_->rpc()));
  }

#ifdef USE_BOOST_SERIALIZATON
  VicarRasterImage() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const
  {
    using boost::serialization::make_nvp;
    ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImage);
    int ntile = number_tile();
    ar << GEOCAL_NVP_(vicar_file)
       << GEOCAL_NVP_(band_id)
       << GEOCAL_NVP_(number_tile_line)
       << make_nvp("number_tile", ntile);
  }
  template<class Archive>
  void load(Archive & ar, const unsigned int version)
  {
    using boost::serialization::make_nvp;
    ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImage);
    int ntile;
    ar >> GEOCAL_NVP_(vicar_file) 
       >> GEOCAL_NVP_(band_id)
       >> GEOCAL_NVP_(number_tile_line)
       >> make_nvp("number_tile", ntile);;
    initialize(band_id_, number_tile_line_, ntile);
  }
  BOOST_SERIALIZATION_SPLIT_MEMBER();
#endif  
};

boost::shared_ptr<RasterImage> 
  vicar_open(const std::string& Fname, int Band_id = 1,
	     VicarFile::access_type Access = VicarFile::READ,
	     bool Favor_memory_mapped = true,
	     int Number_line_per_tile = -1,
	     int Number_tile = 4,
	     bool Force_area_pixel = false);
}

#endif
