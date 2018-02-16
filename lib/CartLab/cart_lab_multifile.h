#ifndef CART_LAB_MULTI_FILE_H
#define CART_LAB_MULTI_FILE_H
#include "raster_multifile.h"
#include "location_to_file.h"
#include "ostream_pad.h"

namespace GeoCal {
/****************************************************************//**
  There are 2 kinds of multi-file databases that the cartlab
  produces. The first uses an IBIS file to describe the file
  structure, this is used by for example SrtmDem. This is handled by
  the class VicarMultiFile.

  The other uses a naming convention, with fixed size tiles in each
  file. Examples are the Landsat 7 data, or the USGS DEM. This class
  provides support for the common behavior of these types of
  databases. For right now we assume that the data needs to be read by
  GDAL (so the data is not VICAR format, but something like geotiff).
  We could relax that if needed.

  The file names are always of the form "n66e130_<extension>" or 
  "s9w62_<extension>". The coordinates in the name give something
  close to the upper left corner of the tile covered by the at
  file. In general this won't be the *exact* upper left corner, there
  may be some offset or border added to the files (this varies from 
  dataset to dataset). Derived classes should fill in a "reference map
  info", which is just the coordinates for a particular file (e.g.,
  use gdalinfo to find the information). We then determine the map
  info for every other file by finding the offset relative to this
  reference map info. We do this by the naming convention rather than
  just reading each file and getting the map info metadata because it
  is much quicker.  See for example UsgsDem for an example of filling
  in this information.

  Note that some databases have 0 padding in the file name (e.g, USGS
  DEM has files like "n47w087_10m.tif"). While others don't (e.g.,
  SRTM has file names like "n47w87_L2.hlf"). Either naming convention
  works with this file, we actually build up the files by reading the
  directory. 
*******************************************************************/
class CartLabMultifile : public RasterMultifile {
public:
  virtual ~CartLabMultifile() {}
//-----------------------------------------------------------------------
/// Database base directory
//-----------------------------------------------------------------------

  const std::string& directory_base() const {return dirbase;}
  void create_subset_file
  (const std::string& Oname, const std::string& Driver,
   const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt, 
   const std::string& Options = "",   
   int boundary = 0) const;
protected:
  CartLabMultifile() {}
  CartLabMultifile(int Number_tile,
		   bool No_coverage_is_error = true, 
		   int No_coverage_fill_value = -1) 
    : RasterMultifile(Number_tile, No_coverage_is_error, 
		      No_coverage_fill_value)
  {
  }

//-----------------------------------------------------------------------
/// Constructor that fills in same data commonly known by the derived
/// constructor. Note that setting Number_line_per_tile and
/// Number_sample_per_tile to -1 means that we replace this with the
/// full size of each file, so we read a tile file all into memory at
/// once. 
//-----------------------------------------------------------------------
  
  CartLabMultifile(const std::string& Dir,
		   const std::string& Dir_env_variable = "",
		   int Number_line_per_tile = -1,
		   int Number_sample_per_tile = -1, 
		   int Number_tile_each_file = 4,
		   int Number_tile = 4,
		   bool No_coverage_is_error = true, 
		   int No_coverage_fill_value = -1);

  // Derived classes should call init_loc_to_file() in their
  // constructor, after filling in information listed below.
  // Note that RasterMultifile also has a init function which may need
  // to be called.
  void init_loc_to_file();

  // This is stuff that the derived class should fill in before
  // calling init_loc_to_file.
  std::string dirbase;		///< Location of data we are reading
  int number_line_per_tile;	///< Number of lines we read in a tile
				/// for each file
  int number_sample_per_tile;	///< Number of lines we read in a tile
				/// for each file
  int number_tile_each_file;	///< Number of tiles in a single file.
  std::string file_name_end;	///< End of file name, e.g., _10m.tif
				///for UsgsDem or _L2.hlf for SRTM
  int lat_ref, lon_ref;		///< Coordinates of mi_ref, e.g., 
				/// 47, -87 for "n47w087_10m.tif"
  MapInfo mi_ref;		///< Mapinfo for a sample tile, e.g.,
				/// n47w087_10m.tif

  // This is the thing that gets filled in
  LocationToFile loc_to_file;	///< Map a pixel location to the file
				///that contains it.
private:
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};  

/****************************************************************//**
  This is a CartLabMultifile where we use GDAL to read each of the
  tiles, e.g., the tiles are geotiff files.
*******************************************************************/

class GdalCartLabMultifile: public CartLabMultifile {
public:
  virtual ~GdalCartLabMultifile() {}
protected:
  GdalCartLabMultifile() {}
  GdalCartLabMultifile(int Number_tile,
		       bool No_coverage_is_error = true, 
		       int No_coverage_fill_value = -1) 
    : CartLabMultifile(Number_tile, No_coverage_is_error, 
		       No_coverage_fill_value)
  {
  }
  GdalCartLabMultifile(const std::string& Dir,
		       const std::string& Dir_env_variable = "",
		       int Number_line_per_tile = -1,
		       int Number_sample_per_tile = -1, 
		       int Number_tile_each_file = 4,
		       int Number_tile = 4,
		       bool No_coverage_is_error = true, 
		       int No_coverage_fill_value = -1)
    : CartLabMultifile(Dir, Dir_env_variable, Number_line_per_tile, 
		       Number_sample_per_tile,
		       Number_tile_each_file, Number_tile,
		       No_coverage_is_error,
		       No_coverage_fill_value)
  {
  }
  virtual RasterMultifileTile get_file(int Line, int Sample) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a CartLabMultifile where we use Vicar to read each of the
  tiles. If desired we can favor doing memory mapping when possible 
  rather than using the VICAR RTL (i.e., we use
  VicarLiteRasterImage). 

  The force_area_pixel_ forces the files to be treated as
  "pixel as area" rather than "pixel as point". This is really just
  meant as a work around for the SRTM data, which incorrectly labels
  the data as "point" rather than "area". Since this is a 15 meter
  difference, it matters for many applications. Most users should
  just ignore this value.
*******************************************************************/
class VicarCartLabMultifile : public CartLabMultifile {
public:
  virtual ~VicarCartLabMultifile() {}
protected:
  VicarCartLabMultifile() {}
  VicarCartLabMultifile(int Number_tile,
		       bool No_coverage_is_error = true, 
		       int No_coverage_fill_value = -1) 
    : CartLabMultifile(Number_tile, No_coverage_is_error, 
		       No_coverage_fill_value)
  {
  }
  VicarCartLabMultifile(const std::string& Dir,
			const std::string& Dir_env_variable = "",
			int Number_line_per_tile = 100,
			int Number_sample_per_tile = -1, 
			int Number_tile_each_file = 4,
			int Number_tile = 4,
			bool Favor_memory_mapped = true,
			bool No_coverage_is_error = true, 
			int No_coverage_fill_value = -1,
			bool Force_area_pixel = false)
    : CartLabMultifile(Dir, Dir_env_variable, Number_line_per_tile, 
		       Number_sample_per_tile,
		       Number_tile_each_file, Number_tile,
		       No_coverage_is_error,
		       No_coverage_fill_value),
      favor_memory_mapped(Favor_memory_mapped),
      force_area_pixel(Force_area_pixel)
  {
  }
  virtual RasterMultifileTile get_file(int Line, int Sample) const;

  bool favor_memory_mapped;	///< Whether we use memory mapping or
				///not when reading an uncompressed
				///file. 
  bool force_area_pixel;	///< If true force map to have pixel
				///as area rather than point, meant as
				///a work around for the SRTM data.
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(CartLabMultifile);
GEOCAL_EXPORT_KEY(GdalCartLabMultifile);
GEOCAL_EXPORT_KEY(VicarCartLabMultifile);
#endif

