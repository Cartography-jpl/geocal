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
#include <HdfEosDef.h>		// Definition of FAIL.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "MSPI-Shared/File/FieldMap/src/hdf4_l1b1_field_map.h"
				// Definition of L1B1FieldMap.
#include <cmath> 		// Definition of std::floor
#include <sstream>              // Definition of std::ostringstream
#include "mfhdf.h"		// Definition of SDsetattr
#include <stdint.h> 		// Definition of uint32_t
#include <boost/filesystem.hpp>	// Definition of boost::filesystem::path

/////////////////////////////////////////////////////////////////////////////
/// Translate AccessType to HDF access mode.
/////////////////////////////////////////////////////////////////////////////

namespace {
  uintn hdfeos2_access_mode(MSPI::Shared::HDF4::L1B1File::AccessType Access)
  {
    uintn hdf_access;
    switch(Access) {
    case MSPI::Shared::HDF4::L1B1File::CREATE: hdf_access = DFACC_CREATE; break;
    case MSPI::Shared::HDF4::L1B1File::READ: hdf_access = DFACC_READ; break;
    case MSPI::Shared::HDF4::L1B1File::WRITE: hdf_access = DFACC_RDWR; break;
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
MSPI::Shared::HDF4::L1B1File::read_frame_count(int Row_number) const
{
  intn status; 		// Status code returned by HDF-EOS
  std::string swath_name_row = swath_name(Row_number);

  int32 sid = SWattach(file_.hdfeos_fid(), (char *)swath_name_row.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + swath_name_row);
  }

  int32 datatype;
  int32 size_bytes;
  
  status = SWattrinfo(sid, (char *)"FrameCount", &datatype, &size_bytes);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWattrinfo: FrameCount, swath: " + 
		      swath_name_row + ",  Filename: " + file_.filename());
  }

  if (datatype != DFNT_UINT32) {
    std::ostringstream message;
    message << "Unexpected datatype ("<<datatype<<") for FrameCount attribute, "
	      << " expected: "<<DFNT_UINT32<< "  swath: " << swath_name_row 
	      << ",  Filename: " << file_.filename();
    throw MSPI_EXCEPTIONm(message.str());
  }

  if (size_bytes != DFKNTsize(DFNT_UINT32)) {
    std::ostringstream message;
    message << "Unexpected size ("<<size_bytes<<") for FrameCount attribute, "
	      << "  swath: " << swath_name_row << ",  Filename: " << file_.filename();
    throw MSPI_EXCEPTIONm(message.str());
  }

  uint32_t frame_count;
  status = SWreadattr(sid, (char *)"FrameCount", &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWreadattr: FrameCount, swath: " +
		      swath_name_row + ",  Filename: " + file_.filename());
  }
  
  status = SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name_row);
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

MSPI::Shared::HDF4::L1B1File::L1B1File(
  const std::string& Filename,
  AccessType Access,
  const FieldMap& Field_map
)
  : number_pixel_(1536),
    file_(Filename, hdfeos2_access_mode(Access)),
    meta_(Hdf4Meta::hdf4_import(file_.file_attr_group_id())),
    row_table_(file_.hdf_fid()),
    field_map_(Field_map),
    read_only_(Access == READ)
{
  intn status; 		// Status code returned by HDF-EOS

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

    int32 sid = SWattach(file_.hdfeos_fid(), (char *)swath_name_row.c_str());
    if (sid == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + swath_name_row);
    }

//---------------------------------------------------------------------------
// For each required field name from the Field Map, for this row number...  
// Include "Time" field as well.
// ---------------------------------------------------------------------------

    std::vector<std::string> required_field_names = field_map_.field_names(row_number);
    required_field_names.push_back("Time");

    for (size_t ifield = 0 ; ifield < required_field_names.size() ; ++ifield) {

//---------------------------------------------------------------------------
// Get dimensions and buffer sizes required for calling SWfieldinfo.
//---------------------------------------------------------------------------

      int32 strbufsize;
      int32 ndims = SWnentries(sid, HDFE_NENTDIM, &strbufsize);
      if (ndims == FAIL) {
	throw MSPI_EXCEPTIONm("Trouble with SWnentries, swath: " + swath_name_row);
      }

//---------------------------------------------------------------------------
// Try querying the file for information about this field.  If the
// query fails, then assume the field is not present, and the file is
// therefore not compatible with the Field Map.
// ---------------------------------------------------------------------------

      int rank;
      std::vector<int32> dims(ndims);
      int32 datatype;

      status = SWfieldinfo(sid, (char *)required_field_names[ifield].c_str(),
			       &rank, &(dims[0]), &datatype, NULL);
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
// Detach swath.
//---------------------------------------------------------------------------

    status = SWdetach(sid);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name_row);
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

MSPI::Shared::HDF4::L1B1File::L1B1File(
  const std::string& Filename
)
  : number_pixel_(1536),
    file_(Filename, DFACC_READ),
    meta_(Hdf4Meta::hdf4_import(file_.file_attr_group_id())),
    row_table_(file_.hdf_fid()),
    field_map_(MSPI::Shared::HDF4::L1B1FieldMap(Filename)),
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

MSPI::Shared::HDF4::L1B1File::~L1B1File()
{
  if (not read_only_) {
    meta_.hdf4_export(file_.file_attr_group_id());
  }
}






/////////////////////////////////////////////////////////////////////////////
/// Read a segment of data for the given row number and field name.
/////////////////////////////////////////////////////////////////////////////

boost::multi_array<float, 2> 
MSPI::Shared::HDF4::L1B1File::read_data(
  int Row_number, 
  const std::string& Field_name, 
  size_t Start_frame, 
  size_t Number_frame
) const
{
  intn status; 		// Status code returned by HDF-EOS

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
// Initialize returned data to fill value.
//---------------------------------------------------------------------------

  std::fill(data.data(),data.data()+data.num_elements(),-9999.0);

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  int32 sid = SWattach(file_.hdfeos_fid(), (char *)swath_name_row.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Read data.
//---------------------------------------------------------------------------

  int32 start[2];
  start[0] = Start_frame;
  start[1] = 0;
  int32 edge[2];
  edge[0] = std::min(Number_frame, number_frame(Row_number) - Start_frame);
  edge[1] = number_pixel();

  if (edge[0] > 0) {

    status = SWreadfield(sid, (char *)Field_name.c_str(), start, NULL, edge, 
			   data.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
		      ", field: " + Field_name);
    }
  }
  
//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

  status = SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name_row);
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
MSPI::Shared::HDF4::L1B1File::write_data(
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

  int32 sid = SWattach(file_.hdfeos_fid(), (char *)swath_name_row.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Write data.
//---------------------------------------------------------------------------

  int32 start[2];
  start[0] = Start_frame;
  start[1] = 0;
  int32 edge[2];
  edge[0] = number_frame;
  edge[1] = number_pixel();

  intn status = SWwritefield(sid, (char *)Field_name.c_str(), 
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
  int32 size = 1;
  status = SWwriteattr(sid, (char *)"FrameCount", DFNT_UINT32, size,
			   &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

  status = SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name_row);
  }

}




/////////////////////////////////////////////////////////////////////////////
/// Read a segment of frame times for the given row number.
/////////////////////////////////////////////////////////////////////////////

std::vector<double> 
MSPI::Shared::HDF4::L1B1File::read_time(
  int Row_number, 
  size_t Start_frame, 
  size_t Number_frame
) const
{
  intn status; 		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Allocate space for data.
//---------------------------------------------------------------------------

  std::vector<double> data(Number_frame,-9999.0);

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  int32 sid = SWattach(file_.hdfeos_fid(), (char *)swath_name_row.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Read data.
//---------------------------------------------------------------------------

  int32 start[1];
  start[0] = Start_frame;
  int32 edge[1];
  edge[0] = std::min(Number_frame, number_frame(Row_number) - Start_frame);

  status = SWreadfield(sid, (char *)"Time", start, NULL, edge, &(data[0]));
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
		    ", field: Time");
  }

//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

  status = SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name_row);
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
MSPI::Shared::HDF4::L1B1File::write_time(
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

  int32 sid = SWattach(file_.hdfeos_fid(), (char *)swath_name_row.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Write data.
//---------------------------------------------------------------------------

  int32 start[1];
  start[0] = Start_frame;
  int32 edge[1];
  edge[0] = number_frame;

  intn status = SWwritefield(sid, (char *)"Time", start, NULL, edge, 
			     (void *)&(Time[0]));
  if (status) {
    std::ostringstream msg;
    msg << "Trouble with SWwritefield, swath: " << swath_name_row 
	<< ", field: Time, start: " << Start_frame << ", edge: " 
	<< number_frame << "\n";
    throw MSPI_EXCEPTIONm(msg.str());
  }

//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

  status = SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name_row);
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
MSPI::Shared::HDF4::L1B1File::append(
  int Row_number, 
  double Time,
  const std::vector<std::vector<float> >& Data
)
{
  intn status; 		// Status code returned by HDF-EOS

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

  for ( size_t ifield = 0 ; ifield < Data.size() ; ++ifield ) {
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

  int32 sid = SWattach(file_.hdfeos_fid(), (char *)swath_name_row.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Write time.
//---------------------------------------------------------------------------

  {
    int32 start[1];
    start[0] = number_frame(Row_number);
    int32 edge[1];
    edge[0] = 1;

    status = SWwritefield(sid, (char *)"Time", start, NULL, edge, &Time);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
		      ", field: Time");
    }
  }
  
//---------------------------------------------------------------------------
// Write data for each field.
//---------------------------------------------------------------------------

  for ( size_t ifield = 0 ; ifield < number_field ; ++ifield ) {
    int32 start[2];
    start[0] = number_frame(Row_number);
    start[1] = 0;
    int32 edge[2];
    edge[0] = 1;
    edge[1] = Data[ifield].size();

    status = SWwritefield(sid, (char *)field_names_row[ifield].c_str(), 
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
  int32 size = 1;
  status = SWwriteattr(sid, (char *)"FrameCount", DFNT_UINT32, size,
			   &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

  status = SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name_row);
  }

}




/////////////////////////////////////////////////////////////////////////////
/// Add row (channel) with the given attributes.  This creates a new swath 
/// structure in the file.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::HDF4::L1B1File::add_row(
  int Row_number, 
  float Wavelength,
  float Polarization_angle,
  size_t Max_number_frame
)
{
  intn status; 		// Status code returned by HDF-EOS

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

  int32 sid = SWcreate(file_.hdfeos_fid(), (char *)swath_name_row.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWcreate, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Define swath dimensions.
//---------------------------------------------------------------------------

  status = SWdefdim(sid,(char *)"Pixel",number_pixel());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdim, dimension: Pixel, swath: " + swath_name_row);
  }
  status = SWdefdim(sid,(char *)"Frame",Max_number_frame);
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
// Create this field.
//---------------------------------------------------------------------------

    status = SWdefdatafield(sid, (char *)field_names_row[ifield].c_str(), 
			    (char *)"Frame,Pixel", DFNT_FLOAT32, HDFE_NOMERGE);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, swath: " + 
		      swath_name_row + ", field: "+field_names_row[ifield].c_str());
    }

//---------------------------------------------------------------------------
// Set fill value for this field.
//---------------------------------------------------------------------------

    float fill_value = -9999.0;
    status = SWsetfillvalue(sid, (char *)field_names_row[ifield].c_str(), 
				&fill_value);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
		      swath_name_row + ", field: "+field_names_row[ifield].c_str());
    }

//---------------------------------------------------------------------------
// End loop for each field.
//---------------------------------------------------------------------------

  }

//---------------------------------------------------------------------------
// Create time field.
//---------------------------------------------------------------------------

  status = SWdefgeofield(sid, (char *)"Time", (char *)"Frame", 
			      DFNT_FLOAT64, HDFE_NOMERGE);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefgeofield, swath: " + 
		    swath_name_row + ", field: Time");
  }

//---------------------------------------------------------------------------
// Set fill value for this field.
//---------------------------------------------------------------------------

  double fill_value = -9999.0;
  status = SWsetfillvalue(sid, (char *)"Time",
			      &fill_value);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
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
  int32 size = 1;
  status = SWwriteattr(sid, (char *)"FrameCount", DFNT_UINT32, size,
			   &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

  status = SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// ABANDONED ATTEMPT TO ENABLE COMPRESSION USING NATIVE HDF
//
// HDF-EOS (for HDF4) does not directly support compression for swath data.
//
// Complicated by the fact that multiple scientific datasets can have
// the same name. 
//
// NOTE: HDF-EOS does not support tiling for the Time field because it
// is internally converted to Vdata within the HDF-EOS library.
//---------------------------------------------------------------------------

//
//  for (int ifield = 0; ifield < number_field ; ifield++) {
//
//    for 
//    SDnametoindices(...);
//    for (int id = 
//	   DFTAG_SD, 
//    int32 sds_index = SDnametoindex(sd_id, (char *)field_names_row[ifield].c_str());
//    if (sds_index == FAIL) {
//      throw MSPI_EXCEPTIONm("Trouble with SDnametoindex, file: " + file_.filename() +
//			    ", field: " + field_names_row[ifield].c_str());
//    }
//
//    int32 sds_id = SDselect(sd_id, sds_index);
//    if (sds_id == FAIL) {
//      throw MSPI_EXCEPTIONm("Trouble with SDselect, file: " + file_.filename());
//    }
//
//      SDidtoref(sds_id);
//	 Vinqtagref(
//
//    {
//      HDF_CHUNK_DEF  c_def;          /* Chunk definition */
//      int32          flag;           /* Chunk flag. */
//
//      memset(&c_def,0,sizeof(HDF_CHUNK_DEF));
//      flag = (HDF_CHUNK | HDF_COMP);
//      c_def.comp.chunk_lengths[0] = chunk_size_frame;
//      c_def.comp.chunk_lengths[1] = number_pixel();
//      c_def.comp.comp_type = COMP_CODE_DEFLATE;
//      c_def.comp.cinfo.deflate.level = 5;
//      status = SDsetchunk(sds_id, c_def, flag);
//      if (status) {
//	throw MSPI_EXCEPTIONm("Trouble with SDsetchunk, file: " + file_.filename() +
//			      ", field: " + field_names_row[ifield].c_str());
//      }
//    }
//
//    status = SDendaccess(sds_id);
//    if (status) {
//      throw MSPI_EXCEPTIONm("Trouble with SDendaccess, file: " + file_.filename());
//    }
//  }
}
