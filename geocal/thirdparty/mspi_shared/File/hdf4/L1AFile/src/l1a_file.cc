//===========================================================================
//                                                                          
//                                  L1AFile                                 
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "l1a_file.h"		// Definition of class.
#include <HdfEosDef.h>		// Definition of FAIL.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream>  		// Definition of std::ostringstream
#include <map>			// Definition of std::map
#include "mfhdf.h"		// Definition of SDsetattr
#include <stdint.h> 		// Definition of uint32_t
#include <boost/filesystem.hpp>	// Definition of boost::filesystem::path

/////////////////////////////////////////////////////////////////////////////
/// Translate AccessType to HDF access mode.
/////////////////////////////////////////////////////////////////////////////

namespace {
  uintn hdfeos2_access_mode(MSPI::Shared::HDF4::L1AFile::AccessType Access)
  {
    uintn hdf_access;
    switch(Access) {
    case MSPI::Shared::HDF4::L1AFile::CREATE: hdf_access = DFACC_CREATE; break;
    case MSPI::Shared::HDF4::L1AFile::READ: hdf_access = DFACC_READ; break;
    case MSPI::Shared::HDF4::L1AFile::WRITE: hdf_access = DFACC_RDWR; break;
    default: 
      std::ostringstream msg;
      msg << "Unexpected value: Access=" << Access;
      throw MSPI_EXCEPTIONm(msg.str());
    }
    return hdf_access;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Create or open file.
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

MSPI::Shared::HDF4::L1AFile::L1AFile(
  const std::string& Filename,
  AccessType Access,
  const FieldMap& Field_map
)
  : number_pixel_(1536),
    max_number_subframe_(35),
    max_number_frame_(10000),
    file_(Filename, hdfeos2_access_mode(Access)),
    meta_(Hdf4Meta::hdf4_import(file_.file_attr_group_id())),
    field_map_(Field_map),
    row_table_(file_.hdf_fid())
{
  intn status;		// Status code returned by HDF-EOS

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

    int32 sid = sw_sid_[row_number] = SWattach(file_.hdfeos_fid(), (char *)swath_name_row.c_str());

    if (sid == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + swath_name_row);
    }

    {
      int32          sds_id;         /* SDS identifier. */
      int32          sds_index;	   /* SDS index. */

      std::string field_name_dn = field_name(row_number, FIELD_INDEX_DN);

      sds_index = SDnametoindex(file_.sid(), field_name_dn.c_str());
      if (sds_index == FAIL) {
	throw MSPI_EXCEPTIONm("Trouble with SDnametoindex, field: "+field_name_dn);
      }
      
      sds_id = dn_sds_id_[row_number] = SDselect(file_.sid(), sds_index);

      if (sds_id == FAIL) {
	throw MSPI_EXCEPTIONm("Trouble with SDselect, field: "+field_name_dn);
      }
    }

//---------------------------------------------------------------------------
// For each required field name from the Field Map, for this row number...  
// Include "SubframeCount" field as well.
// ---------------------------------------------------------------------------

    std::vector<std::string> required_field_names = field_map_.field_names(row_number);
    required_field_names.push_back("SubframeCount");

//---------------------------------------------------------------------------
// Get dimensions and buffer sizes required for calling SWfieldinfo.
//---------------------------------------------------------------------------

    int32 strbufsize;
    int32 ndims = SWnentries(sid, HDFE_NENTDIM, &strbufsize);
    if (ndims == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWnentries, swath: " + swath_name_row);
    }

//---------------------------------------------------------------------------
// Loop over required fields.
//---------------------------------------------------------------------------

    for (size_t ifield = 0 ; ifield < required_field_names.size() ; ++ifield) {

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
// Read FrameCount attribute for this row.
//---------------------------------------------------------------------------

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

//---------------------------------------------------------------------------
// Initialize number of frames for this row.
//---------------------------------------------------------------------------

    number_frame_[row_number] = frame_count;

//---------------------------------------------------------------------------
// End loop for each row in Row Table
//---------------------------------------------------------------------------

  }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Create or check FrameInfo swath.
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

  std::string frameinfo_swath_name("FrameInfo");

  std::vector<std::string> frameinfo_field_names;
  frameinfo_field_names.push_back("tframe_nominal");
  frameinfo_field_names.push_back("omega_bar");
  frameinfo_field_names.push_back("omega_beat");
  frameinfo_field_names.push_back("T");
  frameinfo_field_names.push_back("tframe_measured");
  frameinfo_field_names.push_back("timetag");

//---------------------------------------------------------------------------
// Check if FrameInfo swath exists, if not create it.
//---------------------------------------------------------------------------

  if (Access == CREATE) {

//---------------------------------------------------------------------------
// Create FrameInfo swath.
//---------------------------------------------------------------------------

    int32 sid = SWcreate(file_.hdfeos_fid(), (char *)frameinfo_swath_name.c_str());
    if (sid == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWcreate, swath: " + frameinfo_swath_name);
    }

//---------------------------------------------------------------------------
// Define swath dimensions.
// NOTE: Initial dimension size for "Frame" is 1 because
// HDF-EOS does not allow a dimension size of 0.
//---------------------------------------------------------------------------

    status = SWdefdim(sid,(char *)"Frame",0);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdefdim, dimension: Frame, swath: " + frameinfo_swath_name);
    }

//---------------------------------------------------------------------------
// Iterate over FrameInfo fields creating each one.
//---------------------------------------------------------------------------

    for ( std::vector<std::string>::const_iterator ifield = frameinfo_field_names.begin();
	  ifield != frameinfo_field_names.end(); 
	  ++ifield) {
      std::string field_name = (*ifield);

//---------------------------------------------------------------------------
// Create field.
//---------------------------------------------------------------------------

      status = SWdefdatafield(sid, (char *)field_name.c_str(),
				  (char *)"Frame",
			      DFNT_FLOAT64, HDFE_NOMERGE);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, swath: " + 
			frameinfo_swath_name + ", field: " + field_name);
      }

//---------------------------------------------------------------------------
// Set fill value for this field.
//---------------------------------------------------------------------------

      double fill_value = -9999.0;
      status = SWsetfillvalue(sid, (char *)field_name.c_str(),
			      &fill_value);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
			frameinfo_swath_name + ", field: " + field_name);
      }

    } // End of field loop.

//---------------------------------------------------------------------------
// Initialize number of frames for FrameInfo swath.
//---------------------------------------------------------------------------

    number_frame_[FRAME_INFO_IDX] = 0;

//---------------------------------------------------------------------------
// Initialize FrameCount attribute for FrameInfo swath.
//---------------------------------------------------------------------------

    uint32_t frame_count = number_frame(FRAME_INFO_IDX);
    int32 size = 1;
    status = SWwriteattr(sid, (char *)"FrameCount", DFNT_UINT32, size,
			     &frame_count);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " + frameinfo_swath_name);
    }

//---------------------------------------------------------------------------
// Detach FrameInfo swath.
//---------------------------------------------------------------------------

    status = SWdetach(sid);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + frameinfo_swath_name);
    }

  } else { // End of FrameInfo creation.

//---------------------------------------------------------------------------
// Attach to FrameInfo swath.
//---------------------------------------------------------------------------

    int32 sid = SWattach(file_.hdfeos_fid(), (char *)frameinfo_swath_name.c_str());
    if (sid == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + frameinfo_swath_name);
    }

//---------------------------------------------------------------------------
// Get dimensions and buffer sizes required for calling SWfieldinfo.
//---------------------------------------------------------------------------

    int32 strbufsize;
    int32 ndims = SWnentries(sid, HDFE_NENTDIM, &strbufsize);
    if (ndims == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWnentries, swath: " + frameinfo_swath_name);
    }

//---------------------------------------------------------------------------
// Iterate over FrameInfo fields checking each one.
//---------------------------------------------------------------------------

    for ( std::vector<std::string>::const_iterator ifield = frameinfo_field_names.begin();
	  ifield != frameinfo_field_names.end(); 
	  ++ifield) {
      std::string field_name = (*ifield);

//---------------------------------------------------------------------------
// Try querying the file for information about this field.  If the
// query fails, then assume the field is not present, and the file is
// therefore not a L1A File.
// ---------------------------------------------------------------------------

      int rank;
      std::vector<int32> dims(ndims);
      int32 datatype;

      status = SWfieldinfo(sid, (char *)field_name.c_str(),
			       &rank, &(dims[0]), &datatype, NULL);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWfieldinfo for required field name: " +
			field_name + ",  swath: " + frameinfo_swath_name +
			",  Filename: " + file_.filename());
      }
    } // End of field loop.

//---------------------------------------------------------------------------
// Read FrameCount attribute for FrameInfo swath.
//---------------------------------------------------------------------------

    int32 datatype;
    int32 size_bytes;
    
    status = SWattrinfo(sid, (char *)"FrameCount", &datatype, &size_bytes);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWattrinfo: FrameCount, swath: " + 
		      frameinfo_swath_name + ",  Filename: " + file_.filename());
    }

    if (datatype != DFNT_UINT32) {
      std::ostringstream message;
      message << "Unexpected datatype ("<<datatype<<") for FrameCount attribute, "
	      << " expected: "<<DFNT_UINT32<< "  swath: " << frameinfo_swath_name 
	      << ",  Filename: " << file_.filename();
      throw MSPI_EXCEPTIONm(message.str());
    }

    if (size_bytes != DFKNTsize(DFNT_UINT32)) {
      std::ostringstream message;
      message << "Unexpected size ("<<size_bytes<<") for FrameCount attribute, "
	      << "  swath: " << frameinfo_swath_name << ",  Filename: " << file_.filename();
      throw MSPI_EXCEPTIONm(message.str());
    }

    uint32_t frame_count;
    status = SWreadattr(sid, (char *)"FrameCount", &frame_count);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadattr: FrameCount, swath: " +
		      frameinfo_swath_name + ",  Filename: " + file_.filename());
    }

//---------------------------------------------------------------------------
// Initialize number of frames for FrameInfo swath.
//---------------------------------------------------------------------------

    number_frame_[FRAME_INFO_IDX] = frame_count;

//---------------------------------------------------------------------------
// Detach FrameInfo swath.
//---------------------------------------------------------------------------

    status = SWdetach(sid);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + frameinfo_swath_name);
    }

  } // End of FrameInfo check.

//---------------------------------------------------------------------------
// Write Max_number_subframe attribute only if this is a new file.
//---------------------------------------------------------------------------

  if (Access == CREATE) {

    int32 max_number_subframe_local = max_number_subframe();
    status = SDsetattr(file_.sid(), "Max_number_subframe", DFNT_INT32, 1, 
		       &max_number_subframe_local);
    
  } // End of Max_number_subframe attribute write.

//---------------------------------------------------------------------------
// Create initial metadata.
//---------------------------------------------------------------------------

  if (Access == CREATE) {
    meta_.add<std::string>("Epoch (UTC)","1993-01-01T00:00:00.000000Z");
    meta_.add<std::string>("granule_id",boost::filesystem::path(file_.filename()).filename());
  }
}



/////////////////////////////////////////////////////////////////////////////
/// Close the file
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::HDF4::L1AFile::~L1AFile()
{
  intn status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Close sds_id
// Detach swath.
//---------------------------------------------------------------------------

  std::vector<int> row_set = row_numbers();

  for (std::vector<int>::const_iterator irow = row_set.begin() ; 
       irow != row_set.end() ; 
       ++irow) {
    int row_number = (*irow);

    status = SDendaccess(dn_sds_id_[row_number]);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SDendaccess");
    }

    status = SWdetach(sw_sid_[row_number]);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath");
    }
  }
}



/////////////////////////////////////////////////////////////////////////////
/// Returns an array of data dimensioned by pixel and subframe for a
/// given row and frame number.
/////////////////////////////////////////////////////////////////////////////

boost::multi_array<unsigned short, 2>
MSPI::Shared::HDF4::L1AFile::read_data(
  int Row_number,
  int Frame_number
) const
{
  intn status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  int32 sid = sw_sid(Row_number);

//---------------------------------------------------------------------------
// Read SubframeCount field to number of subframes
//---------------------------------------------------------------------------

  int number_subframe = 0;	// Number of subframes in this row/frame

  {
    int32 start[1];
    start[0] = Frame_number;
    int32 edge[1];
    edge[0] = std::min(1,number_frame(Row_number)-Frame_number);

    if (edge[0] > 0) {

      status = SWreadfield(sid, (char *)"SubframeCount", start, NULL, edge,
			   &(number_subframe));
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
			", field: SubframeCount");
      }
    }
  }

//---------------------------------------------------------------------------
// Allocate space for data.
//---------------------------------------------------------------------------

  boost::multi_array<unsigned short, 2> data(boost::extents[number_subframe][number_pixel()]);

//---------------------------------------------------------------------------
// Get field name.
//---------------------------------------------------------------------------
  
  std::string field_name_dn = field_name(Row_number,FIELD_INDEX_DN);

//---------------------------------------------------------------------------
// Check datatype is compatible with unsigned short.
//---------------------------------------------------------------------------

  {
    int rank;
    std::vector<int32> dims(H4_MAX_VAR_DIMS);
    int32 datatype;

    status = SWfieldinfo(sid, (char *)field_name_dn.c_str(),
			 &rank, &(dims[0]), &datatype, NULL);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWfieldinfo, field name: " +
		      field_name_dn + ",  swath: " + swath_name_row +
		      ",  Filename: " + file_.filename());
    }

    if (datatype != DFNT_UINT16) {
      throw MSPI_EXCEPTIONm("Unexpected datatype for field: " +
		      field_name_dn + ",  swath: " + swath_name_row +
		      ",  Filename: " + file_.filename());
    }
  }

//---------------------------------------------------------------------------
// Read dn data.
//---------------------------------------------------------------------------

  if (number_subframe > 0) {
    int32 start[3];
    start[0] = Frame_number;
    start[1] = 0;
    start[2] = 0;
    int32 edge[3];
    edge[0] = 1;
    edge[1] = number_subframe;
    edge[2] = number_pixel();

    int32 sds_id = dn_sds_id(Row_number);

    status = SDreaddata(sds_id, start, NULL, edge, data.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
		      ", field: " + field_name_dn);
    }
  }

//---------------------------------------------------------------------------
// Return.
//---------------------------------------------------------------------------

  return data;

}

/////////////////////////////////////////////////////////////////////////////
/// Returns a vector of float values for a given row, field index, and 
/// frame number.
/////////////////////////////////////////////////////////////////////////////

std::vector<float>
MSPI::Shared::HDF4::L1AFile::read_float(
  int Row_number,
  int Field_index,
  int Frame_number
) const
{
  intn status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  int32 sid = sw_sid(Row_number);

//---------------------------------------------------------------------------
// Read SubframeCount field to number of subframes
//---------------------------------------------------------------------------

  int number_subframe = 0;    // Number of subframes in this row/frame

  {
    int32 start[1];
    start[0] = Frame_number;
    int32 edge[1];
    edge[0] = std::min(1,number_frame(Row_number)-Frame_number);

    if (edge[0] > 0) {
      status = SWreadfield(sid, (char *)"SubframeCount", start, NULL, edge,
			   &(number_subframe));
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
			", field: SubframeCount");
      }
    }
  }

//---------------------------------------------------------------------------
// Allocate space for data.
//---------------------------------------------------------------------------

  std::vector<float> data(number_subframe,0.0);

//---------------------------------------------------------------------------
// Get field name.
//---------------------------------------------------------------------------
  
  std::string field_name_float = field_name(Row_number, Field_index);

//---------------------------------------------------------------------------
// Check datatype is compatible with float.
//---------------------------------------------------------------------------

  {
    int rank;
    std::vector<int32> dims(H4_MAX_VAR_DIMS);
    int32 datatype;

    status = SWfieldinfo(sid, (char *)field_name_float.c_str(),
			 &rank, &(dims[0]), &datatype, NULL);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWfieldinfo, field name: " +
		      field_name_float + ",  swath: " + swath_name_row +
		      ",  Filename: " + file_.filename());
    }

    if (datatype != DFNT_FLOAT32) {
      throw MSPI_EXCEPTIONm("Unexpected datatype for field: " +
		      field_name_float + ",  swath: " + swath_name_row +
		      ",  Filename: " + file_.filename());
    }
  }

//---------------------------------------------------------------------------
// Read data.
//---------------------------------------------------------------------------

  if (number_subframe > 0) {
    int32 start[2];
    start[0] = Frame_number;
    start[1] = 0;
    int32 edge[2];
    edge[0] = 1;
    edge[1] = number_subframe;

    status = SWreadfield(sid, (char *)field_name_float.c_str(),
			     start, NULL, edge, &(data[0]));
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
		      ", field: " + field_name_float);
    }
  }

//---------------------------------------------------------------------------
// Return.
//---------------------------------------------------------------------------

  return data;

}


/////////////////////////////////////////////////////////////////////////////
/// Write frame time and data ("xn" and "dn") for a given row and frame.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::HDF4::L1AFile::write_frame(
  int Row_number,
  int Frame_number,
  const std::vector<float>& P1,
  const std::vector<float>& P2,
  const std::vector<float>& Xn,
  const boost::multi_array<unsigned short, 2>& Dn
)
{
  intn status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Argument check: Frame_number != number_frame_[Row_number] 
//---------------------------------------------------------------------------

  if (Frame_number != number_frame(Row_number)) {
    std::ostringstream message;
    message << "Argument check: Frame_number != number_frame(Row_number), "
            << "Frame_number = "<<Frame_number << ", number_frame("<<Row_number
            << ") = "<<number_frame(Row_number);
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Get number_subframe in this frame.
//---------------------------------------------------------------------------

  size_t number_subframe = Xn.size();

//---------------------------------------------------------------------------
// Argument check: P1.size() != number_subframe
//---------------------------------------------------------------------------

  if (P1.size() != number_subframe) {
    std::ostringstream message;
    message << "Argument check: P1.size() != number_subframe, "
            << "P1.size() = "<<P1.size()<< ", number_subframe = "
	    << number_subframe;
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Argument check: P2.size() != number_subframe
//---------------------------------------------------------------------------

  if (P2.size() != number_subframe) {
    std::ostringstream message;
    message << "Argument check: P2.size() != number_subframe, "
            << "P2.size() = "<<P2.size()<< ", number_subframe = "
	    << number_subframe;
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Argument check: Xn.size() > max_number_subframe()
//---------------------------------------------------------------------------

  if (number_subframe > max_number_subframe()) {
    std::ostringstream message;
    message << "Argument check: number_subframe > max_number_subframe(), "
            << "number_subframe = "<<number_subframe << ", max_number_subframe = "
	    <<max_number_subframe();
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Argument check: Dn number_subframe != Xn number_subframe
//---------------------------------------------------------------------------

  size_t number_subframe_dn = Dn.shape()[0];

  if (number_subframe_dn != number_subframe) {
    std::ostringstream message;
    message << "Argument check: Dn number_subframe != Xn number_subframe, "
            << "Dn number_subframe = "<<number_subframe_dn << ", Xn number_subframe = "
	    <<number_subframe;
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Argument check: Dn number_pixel != number_pixel()
//---------------------------------------------------------------------------

  size_t number_pixel_dn = Dn.shape()[1];

  if (number_pixel_dn != number_pixel()) {
    std::ostringstream message;
    message << "Argument check: Dn number_pixel != number_pixel, "
            << "Dn number_pixel = "<<number_pixel_dn << ", number_pixel = "
	    <<number_pixel();
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  int32 sid = sw_sid(Row_number);

//---------------------------------------------------------------------------
// Get field names.
//---------------------------------------------------------------------------
  
  std::string field_name_p1 = field_name(Row_number, FIELD_INDEX_P1);
  std::string field_name_p2 = field_name(Row_number, FIELD_INDEX_P2);
  std::string field_name_time = field_name(Row_number, FIELD_INDEX_XN);
  std::string field_name_dn = field_name(Row_number, FIELD_INDEX_DN);

//---------------------------------------------------------------------------
// Write P1 data.
//---------------------------------------------------------------------------

  {
    int32 start[2];
    start[0] = Frame_number;
    start[1] = 0;
    int32 edge[2];
    edge[0] = 1;
    edge[1] = number_subframe;

    status = SWwritefield(sid, (char *)field_name_p1.c_str(),
			      start, NULL, edge, (float *)&(P1[0]));
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
                      ", field: " + field_name_p1);
    }
  }

//---------------------------------------------------------------------------
// Write P2 data.
//---------------------------------------------------------------------------

  {
    int32 start[2];
    start[0] = Frame_number;
    start[1] = 0;
    int32 edge[2];
    edge[0] = 1;
    edge[1] = number_subframe;

    status = SWwritefield(sid, (char *)field_name_p2.c_str(),
			      start, NULL, edge, (float *)&(P2[0]));
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
                      ", field: " + field_name_p2);
    }
  }

//---------------------------------------------------------------------------
// Write time data.
//---------------------------------------------------------------------------

  {
    int32 start[2];
    start[0] = Frame_number;
    start[1] = 0;
    int32 edge[2];
    edge[0] = 1;
    edge[1] = number_subframe;

    status = SWwritefield(sid, (char *)field_name_time.c_str(),
			      start, NULL, edge, (float *)&(Xn[0]));
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
                      ", field: " + field_name_time);
    }
  }

//---------------------------------------------------------------------------
// Write dn data.
//---------------------------------------------------------------------------

  {
    int32 start[3];
    start[0] = Frame_number;
    start[1] = 0;
    start[2] = 0;
    int32 edge[3];
    edge[0] = 1;
    edge[1] = number_subframe;
    edge[2] = number_pixel();

    int32 sds_id = dn_sds_id(Row_number);
    
    status = SDwritedata(sds_id, start, NULL, edge, 
			 (unsigned short *)&(Dn[0][0]));
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SDwritedata, swath: " + swath_name_row + 
                      ", field: " + field_name_dn);
    }
  }

//---------------------------------------------------------------------------
// Write the SubframeCount.
//---------------------------------------------------------------------------

  {
    int32 start[1];
    start[0] = Frame_number;
    int32 edge[1];
    edge[0] = 1;

    status = SWwritefield(sid, (char *)"SubframeCount",
			      start, NULL, edge, (unsigned int *)&number_subframe);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
                      ", field: SubframeCount");
    }
  }

//---------------------------------------------------------------------------
// Increment the number of frames for this row.
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
}



/////////////////////////////////////////////////////////////////////////////
/// Add row (channel) with the given attributes.  This creates a new swath 
/// structure in the file.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::HDF4::L1AFile::add_row(
  int Row_number,
  float Wavelength,
  float Polarization_angle
)
{
  intn status;		// Status code returned by HDF-EOS

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

  int32 sid = sw_sid_[Row_number] = 
    SWcreate(file_.hdfeos_fid(), (char *)swath_name_row.c_str());

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
  status = SWdefdim(sid,(char *)"Subframe",max_number_subframe());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdim, dimension: Subframe, swath: " + swath_name_row);
  }
  status = SWdefdim(sid,(char *)"Frame",max_number_frame_);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdim, dimension: Frame, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Get field names.
//---------------------------------------------------------------------------
  
  std::string field_name_p1 = field_name(Row_number, FIELD_INDEX_P1);
  std::string field_name_p2 = field_name(Row_number, FIELD_INDEX_P2);
  std::string field_name_time = field_name(Row_number, FIELD_INDEX_XN);
  std::string field_name_dn = field_name(Row_number, FIELD_INDEX_DN);
 
//---------------------------------------------------------------------------
// Create P1 field.
//---------------------------------------------------------------------------

  status = SWdefdatafield(sid, (char *)field_name_p1.c_str(),
			  (char *)"Frame,Subframe",
			  DFNT_FLOAT32, HDFE_NOMERGE);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, swath: " + 
		    swath_name_row + ", field: " + field_name_p1);
  }

//---------------------------------------------------------------------------
// Set fill value for P1 field.
//---------------------------------------------------------------------------

  {   
    float fill_value = -9999.0;
    status = SWsetfillvalue(sid, (char *)field_name_p1.c_str(),
			    &fill_value);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
		      swath_name_row + ", field: " + field_name_p1);
    }
  }

//---------------------------------------------------------------------------
// Create P2 field.
//---------------------------------------------------------------------------

  status = SWdefdatafield(sid, (char *)field_name_p2.c_str(),
			  (char *)"Frame,Subframe",
			  DFNT_FLOAT32, HDFE_NOMERGE);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, swath: " + 
		    swath_name_row + ", field: " + field_name_p2);
  }

//---------------------------------------------------------------------------
// Set fill value for P2 field.
//---------------------------------------------------------------------------

  {   
    float fill_value = -9999.0;
    status = SWsetfillvalue(sid, (char *)field_name_p2.c_str(),
			    &fill_value);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
		      swath_name_row + ", field: " + field_name_p2);
    }
  }

//---------------------------------------------------------------------------
// Create time field.
//---------------------------------------------------------------------------

  status = SWdefdatafield(sid, (char *)field_name_time.c_str(),
			  (char *)"Frame,Subframe",
			  DFNT_FLOAT32, HDFE_NOMERGE);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, swath: " + 
		    swath_name_row + ", field: " + field_name_time);
  }

//---------------------------------------------------------------------------
// Set fill value for time field.
//---------------------------------------------------------------------------

  {   
    float fill_value = -9999.0;
    status = SWsetfillvalue(sid, (char *)field_name_time.c_str(),
			    &fill_value);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
		      swath_name_row + ", field: " + field_name_time);
    }
  }

//---------------------------------------------------------------------------
// Create DN field.
//---------------------------------------------------------------------------

  status = SWdefdatafield(sid, (char *)field_name_dn.c_str(),
			  (char *)"Frame,Subframe,Pixel",
			  DFNT_UINT16, HDFE_NOMERGE);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, swath: " + 
		    swath_name_row + ", field: "+field_name_dn);
  }

//---------------------------------------------------------------------------
// Set fill value for DN field.
//---------------------------------------------------------------------------

  {
    unsigned short fill_value = 65533;
    status = SWsetfillvalue(sid, (char *)field_name_dn.c_str(), 
			    &fill_value);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
		      swath_name_row + ", field: "+field_name_dn);
    }
  }

//---------------------------------------------------------------------------
// Create "SubframeCount" field.
//---------------------------------------------------------------------------

  status = SWdefdatafield(sid, (char *)"SubframeCount",
			  (char *)"Frame",
			  DFNT_UINT32, HDFE_NOMERGE);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, swath: " + 
		    swath_name_row + ", field: SubframeCount");
  }

//---------------------------------------------------------------------------
// Set fill value for this field.
//---------------------------------------------------------------------------

  {
    unsigned int fill_value = 0;
    status = SWsetfillvalue(sid, (char *)"SubframeCount",
				&fill_value);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
		      swath_name_row + ", field: SubframeCount");
    }
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
// Set tiling/compression parameters for DN field.
//---------------------------------------------------------------------------

  {
    HDF_CHUNK_DEF  c_def;          /* Chunk definition */
				   /* HDF file identifier. */
    int32          flag;           /* Chunk flag. */
    int32          sds_id;         /* SDS identifier. */
    int32          sds_index;	   /* SDS index. */

    memset(&c_def,0,sizeof(HDF_CHUNK_DEF));

    flag = (HDF_CHUNK | HDF_COMP);
    c_def.comp.chunk_lengths[0] = 16;
    c_def.comp.chunk_lengths[1] = max_number_subframe_;
    c_def.comp.chunk_lengths[2] = number_pixel_;
    c_def.comp.comp_type = COMP_CODE_DEFLATE;
    c_def.comp.cinfo.deflate.level = 5;


    sds_index = SDnametoindex(file_.sid(), field_name_dn.c_str());
    if (sds_index == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SDnametoindex, swath: " + 
		      swath_name_row + ", field: "+field_name_dn);
    }

    sds_id = dn_sds_id_[Row_number] = SDselect(file_.sid(), sds_index);

    if (sds_id == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SDselect, swath: " + 
		      swath_name_row + ", field: "+field_name_dn);
    }

    status = SDsetchunk(sds_id, c_def, flag);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SDsetchunk, swath: " + 
		      swath_name_row + ", field: "+field_name_dn);
    }
  }
}



/////////////////////////////////////////////////////////////////////////////
/// Returns the frame info for the specified frame number.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::FrameInfo
MSPI::Shared::HDF4::L1AFile::frame_info(
  int Frame_number
) const
{
  intn status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// FrameInfo swath_name, field_names and data.
//---------------------------------------------------------------------------

  std::string frameinfo_swath_name("FrameInfo");

  std::map<std::string, double> frameinfo;
  frameinfo["tframe_nominal"] = 0.0;
  frameinfo["omega_bar"] = 0.0;
  frameinfo["omega_beat"] = 0.0;
  frameinfo["T"] = 0.0;
  frameinfo["tframe_measured"] = 0.0;
  frameinfo["timetag"] = 0.0;

//---------------------------------------------------------------------------
// Attach to FrameInfo swath.
//---------------------------------------------------------------------------

  int32 sid = SWattach(file_.hdfeos_fid(), (char *)frameinfo_swath_name.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + frameinfo_swath_name);
  }

//---------------------------------------------------------------------------
// Iterate over FrameInfo fields reading each one.
//---------------------------------------------------------------------------

  for (std::map<std::string,double>::const_iterator ifield = frameinfo.begin();
       ifield != frameinfo.end(); ++ifield) {
    std::string fieldname = ifield->first;
    double data = -9999.0;

    int32 start[1];
    start[0] = Frame_number;
    int32 edge[1];
    edge[0] = std::min(1,number_frame(FRAME_INFO_IDX)-Frame_number);

    if (edge[0] > 0) {
      status = SWreadfield(sid, (char *)fieldname.c_str(),
			   start, NULL, edge, &data);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + frameinfo_swath_name + 
			", field: " + fieldname);
      }
    }

    frameinfo[fieldname] = data;
  }

//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

  status = SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + frameinfo_swath_name);
  }

//---------------------------------------------------------------------------
// Return FrameInfo.
//---------------------------------------------------------------------------

  return FrameInfo(frameinfo["T"], frameinfo["tframe_nominal"], 
		   frameinfo["omega_bar"], frameinfo["omega_beat"],
		   frameinfo["tframe_measured"],frameinfo["timetag"]);
}



/////////////////////////////////////////////////////////////////////////////
/// Write the frame info for the specified frame number.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::HDF4::L1AFile::write_frame_info(
  int Frame_number,
  const FrameInfo& Frame_info
)
{
  intn status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Argument check: Frame_number != number_frame_[FRAME_INFO_IDX] 
//---------------------------------------------------------------------------

  if (Frame_number != number_frame(FRAME_INFO_IDX)) {
    std::ostringstream message;
    message << "Argument check: Frame_number != number_frame(FRAME_INFO_IDX), "
            << "Frame_number = "<<Frame_number << ", number_frame(FRAME_INFO_IDX) = "
	    <<number_frame(FRAME_INFO_IDX);
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// FrameInfo swath_name, field_names and data.
//---------------------------------------------------------------------------

  std::string frameinfo_swath_name("FrameInfo");

  std::map<std::string, double> frameinfo;
  frameinfo["tframe_nominal"] = Frame_info.tframe_nominal();
  frameinfo["omega_bar"] = Frame_info.omega_bar();
  frameinfo["omega_beat"] = Frame_info.omega_beat();
  frameinfo["T"] = Frame_info.T();
  frameinfo["tframe_measured"] = Frame_info.tframe_measured();
  frameinfo["timetag"] = Frame_info.timetag();

//---------------------------------------------------------------------------
// Attach to FrameInfo swath.
//---------------------------------------------------------------------------

  int32 sid = SWattach(file_.hdfeos_fid(), (char *)frameinfo_swath_name.c_str());
  if (sid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + frameinfo_swath_name);
  }

//---------------------------------------------------------------------------
// Iterate over FrameInfo fields writing each one.
//---------------------------------------------------------------------------

  for (std::map<std::string,double>::const_iterator ifield = frameinfo.begin();
       ifield != frameinfo.end(); ++ifield) {
    std::string fieldname = ifield->first;
    double data = ifield->second;

    int32 start[1];
    start[0] = Frame_number;
    int32 edge[1];
    edge[0] = 1;

    status = SWwritefield(sid, (char *)fieldname.c_str(), 
			      start, NULL, edge, (double *)&data);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + frameinfo_swath_name + 
                      ", field: " + fieldname);
    }
  }

//---------------------------------------------------------------------------
// Increment the number of frames for FrameInfo.
//---------------------------------------------------------------------------

  number_frame_[FRAME_INFO_IDX]++;

//---------------------------------------------------------------------------
// Update FrameCount attribute for FrameInfo.
//---------------------------------------------------------------------------

  uint32_t frame_count = number_frame(FRAME_INFO_IDX);
  int32 size = 1;
  status = SWwriteattr(sid, (char *)"FrameCount", DFNT_UINT32, size,
                           &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " 
		    + frameinfo_swath_name);
  }

//---------------------------------------------------------------------------
// Detach swath.
//---------------------------------------------------------------------------

  status = SWdetach(sid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + frameinfo_swath_name);
  }
}

