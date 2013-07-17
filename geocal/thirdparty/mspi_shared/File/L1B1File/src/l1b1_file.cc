//===========================================================================
//                                                                         
//                              L1B1File                                   
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "l1b1_file.h"		// Definition of class.
#include <HE5_HdfEosDef.h>	// Definition of FAIL.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "MSPI-Shared/File/FieldMap/src/hdf5_l1b1_field_map.h"
				// Definition of L1B1FieldMap.
#include <cmath> 		// Definition of std::floor
#include <sstream>              // Definition of std::ostringstream
#include <boost/filesystem.hpp>	// Definition of boost::filesystem::path

/////////////////////////////////////////////////////////////////////////////
/// Translate AccessType to HDF5 access mode.
/////////////////////////////////////////////////////////////////////////////

namespace {
  uintn hdfeos5_access_mode(MSPI::Shared::L1B1File::AccessType Access)
  {
    uintn hdf_access;
    switch(Access) {
    case MSPI::Shared::L1B1File::CREATE: hdf_access = H5F_ACC_TRUNC; break;
    case MSPI::Shared::L1B1File::READ: hdf_access = H5F_ACC_RDONLY; break;
    case MSPI::Shared::L1B1File::WRITE: hdf_access = H5F_ACC_RDWR; break;
    default: 
      std::ostringstream msg;
      msg << "Unexpected value: Access=" << Access;
      throw MSPI_EXCEPTIONm(msg.str());
    }
    return hdf_access;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Read frame count for the given row.
/////////////////////////////////////////////////////////////////////////////

int
MSPI::Shared::L1B1File::read_frame_count(int Row_number) const
{
  herr_t status; 		// Status code returned by HDF-EOS
  std::string swath_name_row = swath_name(Row_number);

  hid_t sid = file_.swath_id(swath_name_row);

  hid_t datatype;
  hsize_t size;
    
  status = HE5_SWattrinfo(sid, "FrameCount", &datatype, &size);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWattrinfo: FrameCount, swath: " + 
			  swath_name_row + ",  Filename: " + file_.filename());
  }

  if (datatype != HE5T_NATIVE_UINT) {
    std::ostringstream message;
    message << "Unexpected datatype ("<<datatype<<") for FrameCount attribute, "
	    << " expected: "<<H5T_NATIVE_UINT32<< "  swath: " << swath_name_row 
	    << ",  Filename: " << file_.filename();
    throw MSPI_EXCEPTIONm(message.str());
  }
  
  if (size != 1) {
    std::ostringstream message;
    message << "Unexpected size ("<<size<<") for FrameCount attribute, "
	    << "  swath: " << swath_name_row << ",  Filename: " << file_.filename();
    throw MSPI_EXCEPTIONm(message.str());
  }
  
  uint32_t frame_count;
  status = HE5_SWreadattr(sid, "FrameCount", &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWreadattr: FrameCount, swath: " +
			  swath_name_row + ",  Filename: " + file_.filename());
  }
  
  return frame_count;
}

/////////////////////////////////////////////////////////////////////////////
///  Create or open file.
///
///  Field map defines swath name and field names for each row
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
///  -# Row names in Field_map must match corresponding swath names,
///     for all rows in the Row Table.
///  -# Field names in Field_map must match corresponding field names in 
///     the file, for all rows in the Row Table.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::L1B1File::L1B1File(
  const std::string& Filename,
  AccessType Access,
  const FieldMap& Field_map
)
  : number_pixel_(1536),
    file_(Filename, hdfeos5_access_mode(Access)),
    meta_(Hdf5Meta::hdf5_import(file_.file_attr_group_id())),
    row_table_(file_.file_attr_group_id()),
    field_map_(Field_map),
    read_only_(Access == READ)
{
  herr_t status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// For each row number in the Row Table...
//---------------------------------------------------------------------------

  std::vector<int> row_set = row_numbers();

  for (std::vector<int>::const_iterator irow = row_set.begin() ; 
       irow != row_set.end() ; 
       ++irow) {
    int row_number = (*irow);

//---------------------------------------------------------------------------
// Get swath name for this row number from the Row Table.
//---------------------------------------------------------------------------

    std::string swath_name_row = swath_name(row_number);

//---------------------------------------------------------------------------
// Check that the row name in the Field Map matches the swath name for
// this row number.
// ---------------------------------------------------------------------------

    if (swath_name_row != field_map_.grid_name(row_number)) {
      std::ostringstream message;
      message << "Swath name '"<<row_table_.swath_name(row_number)
	      <<"', in file, does not match row name '"
	      <<field_map_.grid_name(row_number)<<"', in field map.  Row_number: "
	      << row_number << "   Filename: "<< file_.filename();
      throw MSPI_EXCEPTIONm(message.str());
    }

//---------------------------------------------------------------------------
// Attach to swath for this row number.
//---------------------------------------------------------------------------

    hid_t sid = file_.swath_id(swath_name_row);

//---------------------------------------------------------------------------
// For each required field name from the Field Map, for this row number...  
// ---------------------------------------------------------------------------

    std::vector<std::string> required_field_names = field_map_.field_names(row_number);

    for (size_t ifield = 0 ; ifield < required_field_names.size() ; ++ifield) {

//---------------------------------------------------------------------------
// Get dimensions and buffer sizes required for calling SWfieldinfo.
//---------------------------------------------------------------------------

      long strbufsize;
      long ndims = HE5_SWnentries(sid, HE5_HDFE_NENTDIM, &strbufsize);
      if (ndims == FAIL) {
	throw MSPI_EXCEPTIONm("Trouble with SWnentries, swath: " + swath_name_row);
      }

//---------------------------------------------------------------------------
// Try querying the file for information about this field.  If the
// query fails, then assume the field is not present, and the file is
// therefore not compatible with the Field Map.
// ---------------------------------------------------------------------------

      int rank;
      std::vector<hsize_t> dims(ndims);
      hid_t datatype;

      status = HE5_SWfieldinfo(sid, (char *)required_field_names[ifield].c_str(),
			       &rank, &(dims[0]), &datatype, NULL, NULL);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWfieldinfo for required field name: " +
			required_field_names[ifield] + ",  swath: " + 
			swath_name_row + ",  Filename: " + file_.filename());
      }
      
//---------------------------------------------------------------------------
// End loop for each required field name from the Field Map
//---------------------------------------------------------------------------

    }

//---------------------------------------------------------------------------
// End loop for each row in Row Table
//---------------------------------------------------------------------------

  }

//---------------------------------------------------------------------------
// Create initial metadata.
//---------------------------------------------------------------------------

  if (Access == CREATE) {
    meta_.add<std::string>("granule_id",boost::filesystem::path(file_.filename()).filename());
  }

//---------------------------------------------------------------------------
// Read number of frames for each row.
//---------------------------------------------------------------------------

  for (std::vector<int>::const_iterator irow = row_set.begin() ; 
       irow != row_set.end() ; 
       ++irow) {
    int row_number = (*irow);

    number_frame_[row_number] = read_frame_count(row_number);
  }
}

/////////////////////////////////////////////////////////////////////////////
///  Open file for reading only.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::L1B1File::L1B1File(
  const std::string& Filename
)
  : number_pixel_(1536),
    file_(Filename, H5F_ACC_RDONLY),
    meta_(Hdf5Meta::hdf5_import(file_.file_attr_group_id())),
    row_table_(file_.file_attr_group_id()),
    field_map_(MSPI::Shared::HDF5::L1B1FieldMap(Filename)),
    read_only_(true)
{

//---------------------------------------------------------------------------
// Read number of frames for each row.
//---------------------------------------------------------------------------

  std::vector<int> row_set = row_numbers();

  for (std::vector<int>::const_iterator irow = row_set.begin() ; 
       irow != row_set.end() ; 
       ++irow) {
    int row_number = (*irow);

    number_frame_[row_number] = read_frame_count(row_number);
  }
}


/////////////////////////////////////////////////////////////////////////////
/// Close the file
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::L1B1File::~L1B1File()
{
  if (not read_only_) {
    meta_.hdf5_export(file_.file_attr_group_id());
  }
}


/////////////////////////////////////////////////////////////////////////////
/// Read a segment of data for the given row number and field name.
/////////////////////////////////////////////////////////////////////////////

boost::multi_array<float, 2> 
MSPI::Shared::L1B1File::read_data(
  int Row_number, 
  const std::string& Field_name, 
  size_t Start_frame, 
  size_t Number_frame
) const
{
  herr_t status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Argument check: Number_frame < 1
//---------------------------------------------------------------------------

  if (Number_frame < 1) {
    throw MSPI_EXCEPTIONm("L1B1File::read_data: Number_frame < 1");
  }

//---------------------------------------------------------------------------
// Allocate space for data.
//---------------------------------------------------------------------------

  boost::multi_array<float,2> data(boost::extents[Number_frame][number_pixel()]);

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  hid_t sid = file_.swath_id(swath_name_row);

//---------------------------------------------------------------------------
// Read data.
//---------------------------------------------------------------------------

  hssize_t start[2];
  start[0] = Start_frame;
  start[1] = 0;
  hsize_t edge[2];
  edge[0] = Number_frame;
  edge[1] = number_pixel();

  status = HE5_SWreadfield(sid, (char *)Field_name.c_str(), start, NULL, edge, 
			   data.data());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
		    ", field: " + Field_name);
  }
  
//---------------------------------------------------------------------------
// Return.
//---------------------------------------------------------------------------

  return data;

}


/////////////////////////////////////////////////////////////////////////////
/// Write a segment of data for the given row number and field name.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::L1B1File::write_data(
  int Row_number, 
  const std::string& Field_name, 
  size_t Start_frame,
  const boost::multi_array<float, 2>& Data
) 
{

//---------------------------------------------------------------------------
// Get the number of frames.
//---------------------------------------------------------------------------
  
  size_t number_frame = Data.shape()[0];

//---------------------------------------------------------------------------
// Argument check: number_frame < 1
//---------------------------------------------------------------------------

  if (number_frame < 1) {
    throw MSPI_EXCEPTIONm("L1B1File::write_data: number_frame < 1");
  }

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  hid_t sid = file_.swath_id(swath_name_row);

//---------------------------------------------------------------------------
// Write data.
//---------------------------------------------------------------------------

  hssize_t start[2];
  start[0] = Start_frame;
  start[1] = 0;
  hsize_t edge[2];
  edge[0] = number_frame;
  edge[1] = number_pixel();

  herr_t status = HE5_SWwritefield(sid, (char *)Field_name.c_str(), 
				   start, NULL, edge, (void *)Data.data());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
		    ", field: " + Field_name);
  }

//---------------------------------------------------------------------------
// Update FrameCount attribute for this row.
//---------------------------------------------------------------------------

  number_frame_[Row_number] = std::max(number_frame_[Row_number],
				       Start_frame + number_frame);
  uint32_t frame_count = number_frame_[Row_number];
  hsize_t size = 1;
  status = HE5_SWwriteattr(sid, "FrameCount", H5T_NATIVE_UINT32, &size,
			   &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " + swath_name_row);
  }
}



/////////////////////////////////////////////////////////////////////////////
/// Read a segment of frame times for the given row number.
/////////////////////////////////////////////////////////////////////////////

std::vector<double> 
MSPI::Shared::L1B1File::read_time(
  int Row_number, 
  size_t Start_frame, 
  size_t Number_frame
) const
{
  herr_t status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Allocate space for data.
//---------------------------------------------------------------------------

  std::vector<double> data(Number_frame,0.0);

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  hid_t sid = file_.swath_id(swath_name_row);

//---------------------------------------------------------------------------
// Read data.
//---------------------------------------------------------------------------

  hssize_t start[1];
  start[0] = Start_frame;
  hsize_t edge[1];
  edge[0] = Number_frame;

  status = HE5_SWreadfield(sid, (char *)"Time", start, NULL, edge, &(data[0]));
  if (status) {
    std::ostringstream msg;
    msg << "Trouble with SWreadfield, swath: " << swath_name_row 
	<< ", field: Time, start: " << Start_frame << ", edge: " 
	<< Number_frame << "\n";
    throw MSPI_EXCEPTIONm(msg.str());
  }

//---------------------------------------------------------------------------
// Return.
//---------------------------------------------------------------------------

  return data;

}


/////////////////////////////////////////////////////////////////////////////
/// Write a segment of frame times for the given row number.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::L1B1File::write_time(
  int Row_number, 
  size_t Start_frame, 
  const std::vector<double>& Time
)
{

//---------------------------------------------------------------------------
// Get the number of frames.
//---------------------------------------------------------------------------
  
  size_t number_frame =  Time.size();

//---------------------------------------------------------------------------
// Argument check: number_frame < 1
//---------------------------------------------------------------------------

  if (number_frame < 1) {
    throw MSPI_EXCEPTIONm("L1B1File::write_data: number_frame < 1");
  }

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  hid_t sid = file_.swath_id(swath_name_row);

//---------------------------------------------------------------------------
// Write data.
//---------------------------------------------------------------------------

  hssize_t start[1];
  start[0] = Start_frame;
  hsize_t edge[1];
  edge[0] = number_frame;

  herr_t status = HE5_SWwritefield(sid, (char *)"Time", start, NULL, edge, 
				   (void *)&(Time[0]));
  if (status) {
    std::ostringstream msg;
    msg << "Trouble with SWwritefield, swath: " << swath_name_row 
	<< ", field: Time, start: " << Start_frame << ", edge: " 
	<< number_frame << "\n";
    throw MSPI_EXCEPTIONm(msg.str());
  }
}





/////////////////////////////////////////////////////////////////////////////
/// Append a frame for the specified row.  Data is vector of
/// parameters for each field associated with the row.  Data[0] is the
/// first field, Data[1] is the second field, and so on.  Each element
/// of Data is a vector of size number_pixel().
///
/// The list of fields and the order in which they are required is
/// determined by the field map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::L1B1File::append(
  int Row_number, 
  double Time,
  const std::vector<std::vector<float> >& Data
)
{
  herr_t status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Get field names and number of fields for this row.
//---------------------------------------------------------------------------
  
  std::vector<std::string> field_names_row = field_names(Row_number);
  size_t number_field = field_names_row.size();

//---------------------------------------------------------------------------
// Argument check: Data.size() != number_field
//---------------------------------------------------------------------------

  if (Data.size() != number_field) {
    std::ostringstream message;
    message << "Argument check: Data.size() != number_field, "
	    << "Data.size() = "<<Data.size() << ", number_field = "<<number_field;
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Argument check: Data[x].size() != number_pixel()
//---------------------------------------------------------------------------

  for (size_t ifield = 0 ; ifield < Data.size() ; ++ifield ) {
    if (Data[ifield].size() != number_pixel()) {
      throw MSPI_EXCEPTIONm("Argument check: Data[x].size() != number_pixel()");
    }
  }

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  hid_t sid = file_.swath_id(swath_name_row);

//---------------------------------------------------------------------------
// Write time.
//---------------------------------------------------------------------------

  {
    hssize_t start[1];
    start[0] = number_frame(Row_number);
    hsize_t edge[1];
    edge[0] = 1;

    status = HE5_SWwritefield(sid, (char *)"Time", start, NULL, edge, &Time);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
		      ", field: Time");
    }
  }
  
//---------------------------------------------------------------------------
// Write data for each field.
//---------------------------------------------------------------------------

  for (size_t ifield = 0 ; ifield < number_field ; ++ifield ) {
    hssize_t start[2];
    start[0] = number_frame(Row_number);
    start[1] = 0;
    hsize_t edge[2];
    edge[0] = 1;
    edge[1] = Data[ifield].size();

    status = HE5_SWwritefield(sid, (char *)field_names_row[ifield].c_str(), 
			      start, NULL, edge, (float *)&(Data[ifield][0]));
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
		      ", field: "+field_names_row[ifield]);
    }
  }
  
//---------------------------------------------------------------------------
// Increment the number of frames for this field.
//---------------------------------------------------------------------------

  number_frame_[Row_number]++;

//---------------------------------------------------------------------------
// Update FrameCount attribute for this row.
//---------------------------------------------------------------------------

  uint32_t frame_count = number_frame(Row_number);
  hsize_t size = 1;
  status = HE5_SWwriteattr(sid, "FrameCount", H5T_NATIVE_UINT32, &size,
			   &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " + swath_name_row);
  }

}




/////////////////////////////////////////////////////////////////////////////
/// Add row (channel) with the given attributes.  This creates a new swath 
/// structure in the file.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::L1B1File::add_row(
  int Row_number, 
  float Wavelength,
  float Polarization_angle,
  size_t Max_number_frame
)
{
  herr_t status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Determine the size of the frame dimension and the chunk size.
// The frame dimension must be an even multiple of the chunk size.
//---------------------------------------------------------------------------

  int chunk_size_frame = Max_number_frame;
  int frame_dim = Max_number_frame;
  if (chunk_size_frame > 128) {
    chunk_size_frame = 128;
    frame_dim = 
      ((int)std::ceil(Max_number_frame / (double)chunk_size_frame)) * chunk_size_frame;
  }

//---------------------------------------------------------------------------
// Get swath name for this row from the field map.
//---------------------------------------------------------------------------

  std::string swath_name_row = field_map_.grid_name(Row_number);

//---------------------------------------------------------------------------
// Add row to row table.
//---------------------------------------------------------------------------

  row_table_.add(Row_number, Wavelength, Polarization_angle, swath_name_row);

//---------------------------------------------------------------------------
// Create swath.
//---------------------------------------------------------------------------

  hid_t sid = HE5_SWcreate(file_.hdfeos_fid(), swath_name_row.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWcreate, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Define swath dimensions.
// NOTE: Initial dimension size for "Frame" is 1 because HDF-EOS does not
// allow a dimension size of 0.
//---------------------------------------------------------------------------

  status = HE5_SWdefdim(sid,(char *)"Pixel",number_pixel());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdim, dimension: Pixel, swath: " + swath_name_row);
  }
  status = HE5_SWdefdim(sid,(char *)"Frame",frame_dim);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdim, dimension: Frame, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Get field names and number of fields for this row.
//---------------------------------------------------------------------------
  
  std::vector<std::string> field_names_row = field_names(Row_number);
  int number_field = field_names_row.size();

//---------------------------------------------------------------------------
// For each field...
//---------------------------------------------------------------------------

  for (int ifield = 0; ifield < number_field ; ifield++) {

//---------------------------------------------------------------------------
// Set chunking for the next field defined by HE5_SWdefdatafield.  
//---------------------------------------------------------------------------
    
    int chunk_rank = 2;
    hsize_t chunk_dims[2];
    chunk_dims[0] = chunk_size_frame;
    chunk_dims[1] = number_pixel();
    status = HE5_SWdefchunk(sid, chunk_rank, chunk_dims);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdeftile, swath: " + swath_name_row);
    }

//---------------------------------------------------------------------------
// Set compression for the next field defined by HE5_SWdefdatafield.
//---------------------------------------------------------------------------

    int comp_code = HE5_HDFE_COMP_DEFLATE;
    int comp_level[1] = {5};  // 1 to 9, 1=faster, 9=better compression
    status = HE5_SWdefcomp(sid, comp_code, comp_level);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdefcomp, swath: " + swath_name_row);
    }

//---------------------------------------------------------------------------
// Set fill value for this field.   This must be done *before* the call to
// HE5_SWdefdatafield.
//---------------------------------------------------------------------------

    float fill_value = -9999.0;
    status = HE5_SWsetfillvalue(sid, (char *)field_names_row[ifield].c_str(), 
				H5T_NATIVE_FLOAT, &fill_value);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
		      swath_name_row + ", field: "+field_names_row[ifield].c_str());
    }

//---------------------------------------------------------------------------
// Create this field.
//---------------------------------------------------------------------------

    status = HE5_SWdefdatafield(sid, field_names_row[ifield].c_str(), (char *)"Frame,Pixel",
				NULL,
				H5T_NATIVE_FLOAT, HE5_HDFE_NOMERGE);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, swath: " + 
		      swath_name_row + ", field: "+field_names_row[ifield].c_str());
    }

//---------------------------------------------------------------------------
// End loop for each field.
//---------------------------------------------------------------------------

  }

//---------------------------------------------------------------------------
// Set chunking for time field.
//---------------------------------------------------------------------------
    
  int chunk_rank = 1;
  hsize_t chunk_dims[1];
  chunk_dims[0] = chunk_size_frame;
  status = HE5_SWdefchunk(sid, chunk_rank, chunk_dims);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdeftile, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Set fill value for this field.   This must be done *before* the call to
// HE5_SWdefdatafield.
//---------------------------------------------------------------------------

  double fill_value = -9999.0;
  status = HE5_SWsetfillvalue(sid, (char *)"Time", H5T_NATIVE_DOUBLE,
			      &fill_value);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
		    swath_name_row + ", field: Time");
  }

//---------------------------------------------------------------------------
// Create time field.
//---------------------------------------------------------------------------

  status = HE5_SWdefgeofield(sid, (char *)"Time", (char *)"Frame", NULL, 
			      H5T_NATIVE_DOUBLE, HE5_HDFE_NOMERGE);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefgeofield, swath: " + 
		    swath_name_row + ", field: Time");
  }

//---------------------------------------------------------------------------
// Initialize number of frames for this row.
//---------------------------------------------------------------------------

  number_frame_[Row_number] = 0;

//---------------------------------------------------------------------------
// Initialize FrameCount attribute for this row.
//---------------------------------------------------------------------------

  uint32_t frame_count = number_frame(Row_number);
  hsize_t size = 1;
  status = HE5_SWwriteattr(sid, "FrameCount", H5T_NATIVE_UINT32, &size,
			   &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

  status = HE5_SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name_row);
  }

}

