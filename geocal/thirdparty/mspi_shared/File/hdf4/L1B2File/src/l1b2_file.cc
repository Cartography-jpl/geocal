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

#include "l1b2_file.h"		// Definition of class.
#include <HdfEosDef.h>		// Definition of FAIL.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "MSPI-Shared/Support/src/box_region_coor.h" 
				// Definition of BoxRegionCoor.
#include "MSPI-Shared/File/FieldMap/src/hdf4_l1b2_field_map.h" 
				// Definition of L1B2FieldMap.
#include <sstream>  		// Definition of std::ostringstream
#include <cmath> 		// Definition of std::floor
#include "mfhdf.h"		// Definition of SDsetattr
#include <boost/filesystem.hpp>	// Definition of boost::filesystem::path

/////////////////////////////////////////////////////////////////////////////
/// Translate AccessType to HDF access mode.
/////////////////////////////////////////////////////////////////////////////

namespace {
  uintn hdfeos2_access_mode(MSPI::Shared::HDF4::L1B2File::AccessType Access)
  {
    uintn hdf_access;
    switch(Access) {
    case MSPI::Shared::HDF4::L1B2File::CREATE: hdf_access = DFACC_CREATE; break;
    case MSPI::Shared::HDF4::L1B2File::READ: hdf_access = DFACC_READ; break;
    case MSPI::Shared::HDF4::L1B2File::WRITE: hdf_access = DFACC_RDWR; break;
    default: 
      std::ostringstream msg;
      msg << "Unexpected value: Access=" << Access;
      throw MSPI_EXCEPTIONm(msg.str());
    }
    return hdf_access;
  }
}

/////////////////////////////////////////////////////////////////////////////
///  Create or open file.
///
///  Field map defines grid name and field names for each band
///  number.  When opening an existing file, consistency checks are
///  performed to verify that the file is compatible with Field_map.
///  If the file is not compatible, an exception is thrown.
///
///  Access codes:
///
///  - READ: Open file for reading only.  If file doesn't exist, 
///      throw an exception.
///  - WRITE: Open file for read/write.  If the file doesn't exist, 
///      throw an exception.
///  - CREATE: Create a new file and open for read/write access.  
///      Delete any existing file.
///
/// 
///  Field Map compatibility requirements:
///  -# Band names in Field_map must match corresponding swath names,
///     for all bands in the Band Table.
///  -# Field names in Field_map must match corresponding field names in 
///     the file, for all bands in the Band Table.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::HDF4::L1B2File::L1B2File(
  const std::string& Filename,
  AccessType Access,
  const FieldMap& Field_map
)
  : file_(Filename, hdfeos2_access_mode(Access)),
    meta_(Hdf4Meta::hdf4_import(file_.file_attr_group_id())),
    field_map_(Field_map),
    band_table_(file_.hdf_fid()),
    read_only_(Access == READ)
{
  intn status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// For each band number in the Band Table...
//---------------------------------------------------------------------------

  std::vector<int> band_set = band_numbers();

  for (std::vector<int>::const_iterator iband = band_set.begin() ; 
       iband != band_set.end() ; 
       ++iband) {
    int band_number = (*iband);

//---------------------------------------------------------------------------
// Get swath name for this band number from the Band Table.
//---------------------------------------------------------------------------

    std::string grid_name_band = grid_name(band_number);

//---------------------------------------------------------------------------
// Check that the band name in the Field Map matches the grid name for
// this band number.
// ---------------------------------------------------------------------------

    if (grid_name_band != field_map_.grid_name(band_number)) {
      std::ostringstream message;
      message << "Grid name '"<<band_table_.grid_name(band_number)
	      <<"', in file, does not match band name '"
	      <<field_map_.grid_name(band_number)<<"', in field map.  Band_number: "
	      << band_number << "   Filename: "<< file_.filename();
      throw MSPI_EXCEPTIONm(message.str());
    }

//---------------------------------------------------------------------------
// Attach to grid for this band number.
//---------------------------------------------------------------------------

    int32 sid = GDattach(file_.hdfeos_fid(), (char *)grid_name_band.c_str());
    if (sid == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with GDattach, grid: " + grid_name_band);
    }

//---------------------------------------------------------------------------
// For each required field name from the Field Map, for this band number...  
// ---------------------------------------------------------------------------

    std::vector<std::string> required_field_names = field_map_.field_names(band_number);

    for (size_t ifield = 0 ; ifield < required_field_names.size() ; ++ifield) {

//---------------------------------------------------------------------------
// Get dimensions and buffer sizes required for calling GDfieldinfo.
// NOTE: GDnentries call does not account for XDim, YDim dimensions.
//---------------------------------------------------------------------------

      int32 strbufsize;
      int32 ndims = GDnentries(sid, HDFE_NENTDIM, &strbufsize);
      if (ndims == FAIL) {
	throw MSPI_EXCEPTIONm("Trouble with GDnentries, grid: " + grid_name_band);
      }

//---------------------------------------------------------------------------
// Try querying the file for information about this field.  If the
// query fails, then assume the field is not present, and the file is
// therefore not compatible with the Field Map.
// ---------------------------------------------------------------------------

      int rank;
      std::vector<int32> dims(ndims+2);  // Add 2 to account for XDim, YDim
      int32 datatype;

      status = GDfieldinfo(sid, (char *)required_field_names[ifield].c_str(),
			       &rank, &(dims[0]), &datatype, NULL);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with GDfieldinfo for required field name: " +
			required_field_names[ifield] + ",  grid: " + 
			grid_name_band + ",  Filename: " + file_.filename());
      }
      
//---------------------------------------------------------------------------
// End loop for each required field name from the Field Map
//---------------------------------------------------------------------------

    }

//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

    status = GDdetach(sid);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with GDdetach, grid: " + grid_name_band);
    }

//---------------------------------------------------------------------------
// End loop for each band in Band Table
//---------------------------------------------------------------------------

  }

//---------------------------------------------------------------------------
// Create initial metadata.
//---------------------------------------------------------------------------

  if (Access == CREATE) {
    meta_.add<std::string>("granule_id",boost::filesystem::path(file_.filename()).filename());
  }
}

/////////////////////////////////////////////////////////////////////////////
///  Open file for reading only.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::HDF4::L1B2File::L1B2File(
  const std::string& Filename
)
  : file_(Filename, DFACC_READ),
    meta_(Hdf4Meta::hdf4_import(file_.file_attr_group_id())),
    field_map_(L1B2FieldMap(Filename)),
    band_table_(file_.hdf_fid()),
    read_only_(true)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Destructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::HDF4::L1B2File::~L1B2File()
{
  if (not read_only_) {
    meta_.hdf4_export(file_.file_attr_group_id());
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Align the given region to the nearest pixel boundaries, and calculate
/// the corresponding pixel start location and array dimension sizes.
/// The resulting region will be truncated to the boundaries of the map.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::HDF4::L1B2File::align_region(
  int Band, 
  const BoxRegionCoor& Region_in,
  BoxRegionCoor& Region_out, 
  int& Start_x, 
  int& Start_y, 
  int& Size_x,
  int& Size_y) const 
{

//---------------------------------------------------------------------------
// Get map information for this band.
//---------------------------------------------------------------------------

  MapInformation map_info_band = map_info(Band);

//---------------------------------------------------------------------------
// Calculate the intersection of the map with the region.
//---------------------------------------------------------------------------

  BoxRegionCoor region_n_map = 
    Region_in.intersect(BoxRegionCoor(map_info_band.min_x(), 
				      map_info_band.min_y(),  
				      map_info_band.max_x(), 
				      map_info_band.max_y()));

//---------------------------------------------------------------------------
// Aligns the region boundary to the nearest pixel boundary, and select the
// minimum size region containing all the pixel centers within the area
// defined by the pixel coordinates.
//---------------------------------------------------------------------------

  map_info_band.map_coor_to_pixel_coor(region_n_map,
				       Start_x,
				       Start_y,
				       Size_x,
				       Size_y);

//---------------------------------------------------------------------------
// Calculate Region_out
//---------------------------------------------------------------------------

  int pixel_index_min_x = Start_x;
  int pixel_index_min_y = Start_y;
  int pixel_index_max_x = Start_x + Size_x - 1;
  int pixel_index_max_y = Start_y + Size_y - 1;

  Region_out = map_info_band.
    pixel_coor_to_map_coor(BoxRegionCoor(pixel_index_min_x - 0.5,
					 pixel_index_min_y - 0.5,
					 pixel_index_max_x + 0.5,
					 pixel_index_max_y + 0.5));
}


/////////////////////////////////////////////////////////////////////////////
/// Add band with the given parameters.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::HDF4::L1B2File::add_band(
  int Band_number, 
  float Wavelength,
  double E0,
  const ProjectionInformation& Projection_info,
  const MapInformation& Map_info,
  DimensionOrder Dimension_order,
  size_t Tile_size_x,
  size_t Tile_size_y
)
{

//---------------------------------------------------------------------------
// Get grid name for this band from the field map.
//---------------------------------------------------------------------------

  std::string grid_name = field_map_.grid_name(Band_number);

//---------------------------------------------------------------------------
// Add band to band table.
//---------------------------------------------------------------------------

  band_table_.add(Band_number, Wavelength, E0, grid_name);

//---------------------------------------------------------------------------
// Create grid for this band.
//---------------------------------------------------------------------------

  create_grid(grid_name, Projection_info, Map_info, Dimension_order, Tile_size_x, 
	      Tile_size_y);
}

/////////////////////////////////////////////////////////////////////////////
/// Create grid with the given parameters.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::HDF4::L1B2File::create_grid(
  const std::string& Grid_name,
  const ProjectionInformation& Projection_info,
  const MapInformation& Map_info,
  DimensionOrder Dimension_order,
  size_t Tile_size_x,
  size_t Tile_size_y
)
{
  intn status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Create grid.
//---------------------------------------------------------------------------

  double upleft[2];
  double lowright[2];
    
  upleft[0] = Map_info.min_x();
  upleft[1] = Map_info.max_y();
  lowright[0] = Map_info.max_x();
  lowright[1] = Map_info.min_y();

  int32 gid = GDcreate(file_.hdfeos_fid(), (char *)Grid_name.c_str(), 
			   Map_info.grid_size_x(), 
			   Map_info.grid_size_y(), upleft, lowright);
  if (gid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with GDcreate, grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Define projection information for this grid.
//---------------------------------------------------------------------------

  status = GDdefproj(gid, 
			 Projection_info.proj_code(), 
			 Projection_info.zone_code(), 
			 Projection_info.sphere_code(), 
			 (double *)
			 (&(*(Projection_info.proj_param().begin()))));
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdefproj, grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Define origin for this grid.
//---------------------------------------------------------------------------

  status = GDdeforigin(gid, Map_info.origin_code());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdeforigin, grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Define pixel registration for this grid.
//---------------------------------------------------------------------------

  status = GDdefpixreg(gid, Map_info.pix_reg_code());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdefpixreg, grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Get field names and number of fields for this row.
//---------------------------------------------------------------------------
  
  std::vector<std::string> field_names_band = field_names(Grid_name);
  int number_field = field_names_band.size();

//---------------------------------------------------------------------------
// Create fields.
//---------------------------------------------------------------------------

  for (int ifield = 0; ifield < number_field ; ifield++) {
    std::string datatype = field_map_.type(Grid_name, ifield);
    if (datatype == "int") {
      float fill_value = field_map_.fill<int>(Grid_name, ifield);
      create_field<int>(gid, field_names_band[ifield], Dimension_order, 
			Tile_size_x, Tile_size_y, fill_value);
    } else if (datatype == "float") {
      float fill_value = field_map_.fill<float>(Grid_name, ifield);
      create_field<float>(gid, field_names_band[ifield], Dimension_order, 
			  Tile_size_x, Tile_size_y, fill_value);
    } else if (datatype == "double") {
      double fill_value = field_map_.fill<double>(Grid_name, ifield);
      create_field<double>(gid, field_names_band[ifield], Dimension_order, 
			  Tile_size_x, Tile_size_y, fill_value);
    }
  }

//---------------------------------------------------------------------------
// Detach grid.
//---------------------------------------------------------------------------

  status = GDdetach(gid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdetach, grid: " + Grid_name);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Map information for the given band.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::MapInformation
MSPI::Shared::HDF4::L1B2File::map_info(int Band) const
{
  intn status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Get grid name for this band.
//---------------------------------------------------------------------------

  std::string grid_name_band = grid_name(Band);

//---------------------------------------------------------------------------
// Attach to grid for this band.
//---------------------------------------------------------------------------

  int32 gid = GDattach(file_.hdfeos_fid(), (char *)grid_name_band.c_str());
  if (gid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with GDcreate, grid: " + grid_name_band);
  }

//---------------------------------------------------------------------------
// Read map information for this band.
//---------------------------------------------------------------------------

  int32 xdimsize;
  int32 ydimsize;
  double upleft[2];
  double lowright[2];
  
  status = GDgridinfo(gid, &xdimsize, &ydimsize, upleft, lowright);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDgridinfo, file: " + file_.filename() + 
		    ", grid: " + grid_name_band);
  }
  
  double min_x = upleft[0];
  double max_y = upleft[1];
  double max_x = lowright[0];
  double min_y = lowright[1];

  int origincode;
  status = GDorigininfo(gid, &origincode);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDorigininfo, file: " + file_.filename() + 
		    ", grid: " + grid_name_band);
  }
  
  int pixregcode;
  status = GDpixreginfo(gid, &pixregcode);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDpixreginfo, file: " + file_.filename() + 
		    ", grid: " + grid_name_band);
  }
  
//---------------------------------------------------------------------------
// Detach grid.
//---------------------------------------------------------------------------

  status = GDdetach(gid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdetach, grid: " + grid_name_band);
  }

//---------------------------------------------------------------------------
// Return
//---------------------------------------------------------------------------

  return MapInformation(min_x, min_y, max_x, max_y, (int)xdimsize, (int)ydimsize,
			(MapInformation::OriginCode)origincode, 
			(MapInformation::PixRegCode)pixregcode);
}

/////////////////////////////////////////////////////////////////////////////
/// Projection information for the given band.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::ProjectionInformation
MSPI::Shared::HDF4::L1B2File::projection_info(int Band) const
{
  intn status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Get grid name for this band.
//---------------------------------------------------------------------------

  std::string grid_name_band = grid_name(Band);

//---------------------------------------------------------------------------
// Attach to grid for this band.
//---------------------------------------------------------------------------

  int32 gid = GDattach(file_.hdfeos_fid(), (char *)grid_name_band.c_str());
  if (gid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with GDcreate, grid: " + grid_name_band);
  }

//---------------------------------------------------------------------------
// Read projection information for this band.
//---------------------------------------------------------------------------

  int projcode;
  int zonecode;
  int spherecode;
  double projparam[ProjectionInformation::number_proj_param];
    
  status = GDprojinfo(gid, &projcode, &zonecode, &spherecode, projparam);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDprojinfo, file: " + file_.filename() + 
		    ", grid: " + grid_name_band);
  }
    
//---------------------------------------------------------------------------
// Detach grid.
//---------------------------------------------------------------------------

  status = GDdetach(gid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdetach, grid: " + grid_name_band);
  }

//---------------------------------------------------------------------------
// Return
//---------------------------------------------------------------------------

  return ProjectionInformation(projcode, spherecode, zonecode,
			       std::vector<double>(&projparam[0],
						   &projparam[ProjectionInformation::number_proj_param]));

}

MSPI::Shared::HDF4::L1B2File::DimensionOrder 
MSPI::Shared::HDF4::L1B2File::dimension_order(
  const std::string& Grid_name,
  const std::string& Field_name
) const
{

  DimensionOrder dimension_order_grid;

//---------------------------------------------------------------------------
// Attach to grid for this band.
//---------------------------------------------------------------------------

  int32 gid = GDattach(file_.hdfeos_fid(), (char *)Grid_name.c_str());
  if (gid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with GDcreate, grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Get dimension info.
//---------------------------------------------------------------------------

  int32 rank;
  int32 dims[100];
  int32 number_type;
  char dimlist[1000];
  memset(dimlist, 0, 1000);
  
  intn status =  GDfieldinfo(gid, (char *)Field_name.c_str(), &rank, dims, &number_type,
			     dimlist);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDfieldinfo, grid: " + Grid_name+
		    ", field: " + Field_name);
  }
  
  if (!strcmp(dimlist,"XDim,YDim")) {
    dimension_order_grid = XMAJOR;
  } else if (!strcmp(dimlist,"YDim,XDim")) {
    dimension_order_grid = YMAJOR;
  } else {
    throw MSPI_EXCEPTIONm("Unsupported dimension order: "+std::string(dimlist)+
		    ", grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Detach grid.
//---------------------------------------------------------------------------

  status = GDdetach(gid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with GDdetach, grid: " + Grid_name);
  }

//---------------------------------------------------------------------------
// Return 
//---------------------------------------------------------------------------

  return dimension_order_grid;
  
}
