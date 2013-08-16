//===========================================================================
//                                                                         
//                              L1B2File                                   
//                                                                         
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef L1B2_FILE_H
#define L1B2_FILE_H

#include <HE5_HdfEosDef.h>	  // Definition of hid_t

#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include "MSPI-Shared/Support/src/projection_information.h"
				  // Definition of ProjectionInformation
#include "MSPI-Shared/Support/src/map_information.h"
				  // Definition of MapInformation
#include "MSPI-Shared/File/Table/src/band_table.h"
				  // Definition of BandTable
#include "MSPI-Shared/File/Table/src/polygon_table.h"
				  // Definition of polygon_table_write
#include "MSPI-Shared/File/FieldMap/src/field_map.h"
				  // Definition of FieldMap
#include "MSPI-Shared/File/FileHandle/src/hdfeos5_file_handle.h"
				  // Definition of Hdfeos5FileHandle
#include "MSPI-Shared/File/Meta/src/hdf5_meta.h"
				  // Definition of Hdf5Meta

namespace MSPI {
  namespace Shared {
    class L1B2File;
    class BoxRegionCoor;          // Forward declaration.
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief File access interface for MSPI L1B2 files.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::L1B2File {
public:
  typedef enum {
    CREATE = 0,
    WRITE = 1,
    READ = 2
  } AccessType;

  typedef enum {
    XMAJOR = 0,
    YMAJOR = 1
  } DimensionOrder;

  L1B2File(const std::string& Filename, AccessType Access,
	   const FieldMap& Field_map);
  L1B2File(const std::string& Filename);

  ~L1B2File();
  
  ProjectionInformation projection_info(int Band) const;

  MapInformation map_info(int Band) const;
  
  void align_region(int Band, const BoxRegionCoor& Region_in,
		    BoxRegionCoor& Region_out, int& Start_x, int& Start_y, 
		    int& Size_x, int& Size_y) const;

  template <class T>
  boost::multi_array<T, 2> read(int Band, const std::string& Field_name, 
				int Start_x, int Start_y, 
				int Size_x, int Size_y) const;

  template <class T>
  boost::multi_array<T, 2> read(const std::string& Grid_name, 
				const std::string& Field_name, 
				int Start_x, int Start_y, 
				int Size_x, int Size_y) const;

  template <class T>
  void write(int Band, const std::string& Field_name, int Start_x, int Start_y, 
	     const boost::multi_array<T, 2>& Data);

  template <class T>
  void write(const std::string& Grid_name, 
	     const std::string& Field_name, int Start_x, int Start_y, 
	     const boost::multi_array<T, 2>& Data);

  void add_band(int Band_number, float Wavelength, double E0,
		const ProjectionInformation& Projection_info,
		const MapInformation& Map_info,
		DimensionOrder Dimension_order,
		size_t Tile_size_x = 64, size_t Tile_size_y = 64);

  void create_grid(const std::string& Grid_name,
		   const ProjectionInformation& Projection_info,
		   const MapInformation& Map_info,
		   DimensionOrder Dimension_order,
		   size_t Tile_size_x = 64, size_t Tile_size_y = 64);

  DimensionOrder dimension_order(int Band, const std::string& Field_name) const;
  DimensionOrder dimension_order(const std::string& Grid_name, 
				 const std::string& Field_name) const;

  float wavelength(int Band) const;
  
  std::string grid_name(int Band) const;

  std::vector<int> band_numbers() const;

  std::vector<std::string> field_names(int Row_number) const;
  std::vector<std::string> field_names(const std::string& Grid_name) const;

  std::string epoch() const;

  void epoch(const std::string& Epoch);

  void add_meta(const Meta& Meta);
  const Meta& meta() const;
  std::string granule_id() const;
  void polygon_table_write(const std::vector<double>& Latitude,
			   const std::vector<double>& Longtude);
  void polygon_table_read(std::vector<double>& Latitude,
			  std::vector<double>& Longitude) const;

protected:

private:
  template <class T> 
  void create_field(hid_t Gid,				     
		    const std::string& Field_name, 
		    DimensionOrder Dimension_order,
		    size_t Tile_size_x,
		    size_t Tile_size_y,
		    T Fill_value);
  template <class T> hid_t datatype() const;
  Hdfeos5FileHandle file_;
  Hdf5Meta meta_;
  FieldMap field_map_;             ///< Contains swath name and field names 
                                   ///< for each row.
  BandTable band_table_;
  bool read_only_;
};

/////////////////////////////////////////////////////////////////////////////
/// Write GeoPolygon table.
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::L1B2File::polygon_table_write(
  const std::vector<double>& Latitude,
  const std::vector<double>& Longitude
)
{
  MSPI::Shared::polygon_table_write(file_.file_attr_group_id(), Latitude, Longitude);
}

/////////////////////////////////////////////////////////////////////////////
/// Read GeoPolygon table.
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::L1B2File::polygon_table_read(
  std::vector<double>& Latitude,
  std::vector<double>& Longitude
) const
{
  MSPI::Shared::polygon_table_read(file_.file_attr_group_id(), Latitude, Longitude);
}

/////////////////////////////////////////////////////////////////////////////
/// Return granule identifier.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::L1B2File::granule_id() const
{
  return meta_.get_value<std::string>("granule_id");
}

/////////////////////////////////////////////////////////////////////////////
/// Add the metadata to the current collection.  Throws an exception if
/// existing metadata does not match.
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::L1B2File::add_meta(const Meta& Meta)
{
  return meta_.add(Meta);
}

/////////////////////////////////////////////////////////////////////////////
/// Return metadata.
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::Meta&
MSPI::Shared::L1B2File::meta() const
{
  return meta_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return HDF datatype identifier
/// Note: template specializations must be inside a namespace block.
/////////////////////////////////////////////////////////////////////////////

namespace MSPI {
  namespace Shared {
    template <> inline hid_t L1B2File::datatype<int>() const { return H5T_NATIVE_INT; }
    template <> inline hid_t L1B2File::datatype<float>() const { return H5T_NATIVE_FLOAT; }
    template <> inline hid_t L1B2File::datatype<double>() const { return H5T_NATIVE_DOUBLE; }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Read a rectangular region of data for the given band and field name.
/// Returned data will be in the grid dimension order.
///
/// X-MAJOR : c_storage_order, Data[x][y]
/// Y-MAJOR : c_storage_order, Data[y][x]
/////////////////////////////////////////////////////////////////////////////

template <class T>
boost::multi_array<T, 2> 
MSPI::Shared::L1B2File::read(
  int Band, 
  const std::string& Field_name, 
  int Start_x, 
  int Start_y, 
  int Size_x, 
  int Size_y) const
{
  return read<T>(grid_name(Band), Field_name, Start_x, Start_y, Size_x, Size_y);
}

/////////////////////////////////////////////////////////////////////////////
/// Read a rectangular region of data for the given band and field name.
/// Returned data will be in the grid dimension order.
///
/// X-MAJOR : c_storage_order, Data[x][y]
/// Y-MAJOR : c_storage_order, Data[y][x]
/////////////////////////////////////////////////////////////////////////////

template <class T>
boost::multi_array<T, 2> 
MSPI::Shared::L1B2File::read(
  const std::string& Grid_name, 
  const std::string& Field_name, 
  int Start_x, 
  int Start_y, 
  int Size_x, 
  int Size_y) const
{
  herr_t status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Get the dimension order for this band.
//---------------------------------------------------------------------------

  DimensionOrder dimension_order_band = dimension_order(Grid_name, Field_name);

//---------------------------------------------------------------------------
// Allocate space for data.
//---------------------------------------------------------------------------

  std::vector<hsize_t> shape(2);
  shape[0] = (dimension_order_band == XMAJOR ? Size_x : Size_y);
  shape[1] = (dimension_order_band == XMAJOR ? Size_y : Size_x);
  boost::multi_array<T,2> data(shape);

//---------------------------------------------------------------------------
// Attach to grid for this band.
//---------------------------------------------------------------------------

  hid_t gid = HE5_GDattach(file_.hdfeos_fid(), Grid_name.c_str());
  if (gid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with GDattach, grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Read data.
//---------------------------------------------------------------------------

  hssize_t start[2];
  start[0] = (dimension_order_band == XMAJOR ? Start_x : Start_y);
  start[1] = (dimension_order_band == XMAJOR ? Start_y : Start_x);

  status = HE5_GDreadfield(gid, Field_name.c_str(), start, NULL, &shape[0],
			   data.data());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDreadfield, grid: " + Grid_name + 
		    ", field: " + Field_name);
  }
  
//---------------------------------------------------------------------------
// Detach grid.
//---------------------------------------------------------------------------

  status = HE5_GDdetach(gid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdetach, grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Return.
//---------------------------------------------------------------------------

  return data;

}

/////////////////////////////////////////////////////////////////////////////
/// Write a rectangular region of data for the given band and field name. 
/// Memory storage order of input data must match the grid dimension order
/// 
/// X-MAJOR : c_storage_order, Data[x][y]
/// Y-MAJOR : c_storage_order, Data[y][x]
/////////////////////////////////////////////////////////////////////////////

template <class T>
inline void 
MSPI::Shared::L1B2File::write(
  int Band, 
  const std::string& Field_name, 
  int Start_x, 
  int Start_y, 
  const boost::multi_array<T, 2>& Data)
{
  write(grid_name(Band), Field_name, Start_x, Start_y, Data);
}

/////////////////////////////////////////////////////////////////////////////
/// Write a rectangular region of data for the given band and field name. 
/// Memory storage order of input data must match the grid dimension order
/// 
/// X-MAJOR : c_storage_order, Data[x][y]
/// Y-MAJOR : c_storage_order, Data[y][x]
/////////////////////////////////////////////////////////////////////////////

template <class T>
inline void 
MSPI::Shared::L1B2File::write(
  const std::string& Grid_name,
  const std::string& Field_name, 
  int Start_x, 
  int Start_y, 
  const boost::multi_array<T, 2>& Data)
{
  herr_t status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Get the dimension order for this band.
//---------------------------------------------------------------------------

  DimensionOrder dimension_order_grid = dimension_order(Grid_name, Field_name);

//---------------------------------------------------------------------------
// Attach to grid for this band.
//---------------------------------------------------------------------------

  hid_t gid = HE5_GDattach(file_.hdfeos_fid(), Grid_name.c_str());
  if (gid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with GDattach, grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Write data.
//---------------------------------------------------------------------------

  hssize_t start[2];
  start[0] = (dimension_order_grid == XMAJOR ? Start_x : Start_y);
  start[1] = (dimension_order_grid == XMAJOR ? Start_y : Start_x);
  hsize_t edge[2];
  edge[0] = Data.shape()[0];
  edge[1] = Data.shape()[1];

  status = HE5_GDwritefield(gid, Field_name.c_str(), start, NULL, edge, 
			    (T *)Data.data());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDwritefield, grid: " + Grid_name + 
		    ", field: " + Field_name);
  }
  
//---------------------------------------------------------------------------
// Detach grid.
//---------------------------------------------------------------------------

  status = HE5_GDdetach(gid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdetach, grid: " + Grid_name);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Create field.
/////////////////////////////////////////////////////////////////////////////

template <class T>
inline void
MSPI::Shared::L1B2File::create_field(
  hid_t Gid,				     
  const std::string& Field_name, 
  DimensionOrder Dimension_order,
  size_t Tile_size_x,
  size_t Tile_size_y,
  T Fill_value
)
{
//---------------------------------------------------------------------------
// Set the order of XDim and YDim.
//---------------------------------------------------------------------------

  const char *dimlist;
  if (Dimension_order == YMAJOR) {
    dimlist = "YDim,XDim";
  } else {
    dimlist = "XDim,YDim";
  }

//---------------------------------------------------------------------------
// Set tiling for the next call to GDdeffield.  This has to be called
// before *each* call to GDdeffield.  It only applies to the *next* call to
// GDdeffield; not all subsequent calls to HE5_GDdeffield.
// ---------------------------------------------------------------------------
    
  int tile_rank = 2;
  hsize_t tile_dims[2];
  tile_dims[0] = (Dimension_order == XMAJOR ? Tile_size_x : Tile_size_y);
  tile_dims[1] = (Dimension_order == XMAJOR ? Tile_size_y : Tile_size_x);
  herr_t status;
  status = HE5_GDdeftile(Gid, HE5_HDFE_TILE, tile_rank, tile_dims);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdeftile");
  }

//---------------------------------------------------------------------------
// Set compression for the next call to GDdeffield.  This has to be
// called before *each* call to GDdeffield.  It only applies to the
// *next* call to GDdeffield; not all subsequent calls to HE5_GDdeffield.
// ---------------------------------------------------------------------------

  int comp_code = HE5_HDFE_COMP_DEFLATE;
  int comp_level[1] = {5};  // 1 to 9, 1=faster, 9=greater compression
  status = HE5_GDdefcomp(Gid, comp_code, comp_level);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdefcomp");
  }
  
//---------------------------------------------------------------------------
// Set fill value for this field.   This must be done *before* the call to
// HE5_GDdeffield.
//---------------------------------------------------------------------------

  status = HE5_GDsetfillvalue(Gid, Field_name.c_str(), datatype<T>(), &Fill_value);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDsetfillvalue, field: " + Field_name);
  }

//---------------------------------------------------------------------------
// Create this field.
//---------------------------------------------------------------------------

  status = HE5_GDdeffield(Gid, Field_name.c_str(), (char *)dimlist, NULL, 
			  datatype<T>(), HE5_HDFE_NOMERGE);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdeffield, field: " + Field_name);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Return wavelength for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::L1B2File::wavelength(int Band) const
{
  return band_table_.wavelength(Band);
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return grid name for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline std::string 
MSPI::Shared::L1B2File::grid_name(int Band) const
{
  return band_table_.grid_name(Band);
}

/////////////////////////////////////////////////////////////////////////////
/// Returns the set of band numbers in the Band Table.  This is equivalently
/// the set of bands for which there exists a grid structure in the
/// file; and may be a subset of the band numbers defined in the Field
/// Map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<int> 
MSPI::Shared::L1B2File::band_numbers() const
{
  return band_table_.band_numbers();
}

/////////////////////////////////////////////////////////////////////////////
/// Returns the vector of field names for the given band number, as
/// defined by the Field Map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string> 
MSPI::Shared::L1B2File::field_names(int Row_number) const
{
  return field_map_.field_names(Row_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Returns the vector of field names for the given grid name, as
/// defined by the Field Map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string> 
MSPI::Shared::L1B2File::field_names(const std::string& Grid_name) const
{
  return field_map_.field_names(Grid_name);
}

/////////////////////////////////////////////////////////////////////////////
/// Return epoch value.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::L1B2File::epoch() const
{
  return meta_.get_value<std::string>("Epoch (UTC)");
}

/////////////////////////////////////////////////////////////////////////////
/// Set epoch value.  This should be a CCSDS ASCII UTC time string.
/// Example: 1993-01-01T00:00:00.000000Z
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::L1B2File::epoch(const std::string& Epoch)
{
  meta_.add<std::string>("Epoch (UTC)",Epoch);
}

/////////////////////////////////////////////////////////////////////////////
/// Return dimension order for the given band.
/////////////////////////////////////////////////////////////////////////////

inline MSPI::Shared::L1B2File::DimensionOrder 
MSPI::Shared::L1B2File::dimension_order(
  int Band,
  const std::string& Field_name
) const
{
  return dimension_order(grid_name(Band), Field_name);
}

#endif
