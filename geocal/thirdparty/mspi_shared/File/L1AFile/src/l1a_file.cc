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
#include <HE5_HdfEosDef.h>	// Definition of FAIL.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream>  		// Definition of std::ostringstream
#include <map>			// Definition of std::map
#include <boost/filesystem.hpp>	// Definition of boost::filesystem::path

/////////////////////////////////////////////////////////////////////////////
/// Translate AccessType to HDF5 access mode.
/////////////////////////////////////////////////////////////////////////////

namespace {
  uintn hdfeos5_access_mode(MSPI::Shared::L1AFile::AccessType Access)
  {
    uintn hdf_access;
    switch(Access) {
    case MSPI::Shared::L1AFile::CREATE: hdf_access = H5F_ACC_TRUNC; break;
    case MSPI::Shared::L1AFile::READ: hdf_access = H5F_ACC_RDONLY; break;
    case MSPI::Shared::L1AFile::WRITE: hdf_access = H5F_ACC_RDWR; break;
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

MSPI::Shared::L1AFile::L1AFile(
  const std::string& Filename,
  AccessType Access,
  const FieldMap& Field_map,
  size_t Max_number_frame,
  size_t Max_number_subframe
)
  : number_pixel_(1536),
    max_number_frame_(Max_number_frame),
    max_number_subframe_(Max_number_subframe),
    file_(Filename, hdfeos5_access_mode(Access)),
    meta_(Hdf5Meta::hdf5_import(file_.file_attr_group_id())),
    field_map_(Field_map),
    read_only_(Access == READ)
{
  herr_t status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Determine the size of the frame dimension and the chunk size.
// The frame dimension must be an even multiple of the chunk size.
//---------------------------------------------------------------------------

  frame_chunk_size_ = Max_number_frame;
  frame_dim_size_ = Max_number_frame;
  if (frame_chunk_size_ > 3) {
    frame_chunk_size_ = 3;
    frame_dim_size_ = 
      ((int)std::ceil(Max_number_frame / 
		      (double)frame_chunk_size_)) * frame_chunk_size_;
  }
  if (max_number_frame_ == 0) {
    max_number_frame_ = meta_.get_value<unsigned>("Max_number_frame");
  }
  if (max_number_subframe_ == 0) {
    max_number_subframe_ = meta_.get_value<unsigned>("Max_number_subframe");
  }

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Check swath rows according to row_table.
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Create/open Row Table.
//---------------------------------------------------------------------------

    row_table_ = RowTable(file_.file_attr_group_id());

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
// Include "SubframeCount" field as well.
// ---------------------------------------------------------------------------

    std::vector<std::string> required_field_names = field_map_.field_names(row_number);
    required_field_names.push_back("SubframeCount");

//---------------------------------------------------------------------------
// Get dimensions and buffer sizes required for calling SWfieldinfo.
//---------------------------------------------------------------------------

    long strbufsize;
    long ndims = HE5_SWnentries(sid, HE5_HDFE_NENTDIM, &strbufsize);
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
// Read FrameCount attribute for this row.
//---------------------------------------------------------------------------

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

    hid_t sid = HE5_SWcreate(file_.hdfeos_fid(), frameinfo_swath_name.c_str());
    if (sid == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWcreate, swath: " + frameinfo_swath_name);
    }

//---------------------------------------------------------------------------
// Define swath dimensions.
// NOTE: Initial dimension size for "Frame" is 1 because
// HDF-EOS does not allow a dimension size of 0.
//---------------------------------------------------------------------------

    status = HE5_SWdefdim(sid,(char *)"Frame",frame_dim_size_);
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
// Set chunking for field.
//---------------------------------------------------------------------------
 
      int chunk_rank = 1;
      hsize_t chunk_dims[1];
      chunk_dims[0] = frame_dim_size_;
      status = HE5_SWdefchunk(sid, chunk_rank, chunk_dims);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWdefchunk, swath: " + frameinfo_swath_name);
      }

//---------------------------------------------------------------------------
// Set fill value for this field.   This must be done *before* the call to
// HE5_SWdefdatafield.
//---------------------------------------------------------------------------

      double fill_value = -9999.0;
      status = HE5_SWsetfillvalue(sid, (char *)field_name.c_str(),
				  H5T_NATIVE_DOUBLE, &fill_value);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, swath: " + 
			frameinfo_swath_name + ", field: " + field_name);
      }

//---------------------------------------------------------------------------
// Create field.
//---------------------------------------------------------------------------

      status = HE5_SWdefdatafield(sid, (char *)field_name.c_str(),
				  (char *)"Frame",
				  NULL, 
				  H5T_NATIVE_DOUBLE, HE5_HDFE_NOMERGE);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, swath: " + 
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
    hsize_t size = 1;
    status = HE5_SWwriteattr(sid, "FrameCount", H5T_NATIVE_UINT32, &size,
			     &frame_count);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " + frameinfo_swath_name);
    }

//---------------------------------------------------------------------------
// Detach FrameInfo swath.
//---------------------------------------------------------------------------

    status = HE5_SWdetach(sid);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + frameinfo_swath_name);
    }

  } else { // End of FrameInfo creation.

//---------------------------------------------------------------------------
// Attach to FrameInfo swath.
//---------------------------------------------------------------------------

    hid_t sid = file_.swath_id(frameinfo_swath_name);

//---------------------------------------------------------------------------
// Get dimensions and buffer sizes required for calling SWfieldinfo.
//---------------------------------------------------------------------------

    long strbufsize;
    long ndims = HE5_SWnentries(sid, HE5_HDFE_NENTDIM, &strbufsize);
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
      std::vector<hsize_t> dims(ndims);
      hid_t datatype;

      status = HE5_SWfieldinfo(sid, (char *)field_name.c_str(),
			       &rank, &(dims[0]), &datatype, NULL, NULL);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with SWfieldinfo for required field name: " +
			field_name + ",  swath: " + frameinfo_swath_name +
			",  Filename: " + file_.filename());
      }
    } // End of field loop.

//---------------------------------------------------------------------------
// Read FrameCount attribute for FrameInfo swath.
//---------------------------------------------------------------------------

    hid_t datatype;
    hsize_t size;
    
    status = HE5_SWattrinfo(sid, "FrameCount", &datatype, &size);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWattrinfo: FrameCount, swath: " + 
		      frameinfo_swath_name + ",  Filename: " + file_.filename());
    }

    if (datatype != HE5T_NATIVE_UINT) {
      std::ostringstream message;
      message << "Unexpected datatype ("<<datatype<<") for FrameCount attribute, "
	      << " expected: "<<H5T_NATIVE_UINT32<< "  swath: " << frameinfo_swath_name 
	      << ",  Filename: " << file_.filename();
      throw MSPI_EXCEPTIONm(message.str());
    }

    if (size != 1) {
      std::ostringstream message;
      message << "Unexpected size ("<<size<<") for FrameCount attribute, "
	      << "  swath: " << frameinfo_swath_name << ",  Filename: " << file_.filename();
      throw MSPI_EXCEPTIONm(message.str());
    }

    uint32_t frame_count;
    status = HE5_SWreadattr(sid, "FrameCount", &frame_count);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadattr: FrameCount, swath: " +
		      frameinfo_swath_name + ",  Filename: " + file_.filename());
    }

//---------------------------------------------------------------------------
// Initialize number of frames for FrameInfo swath.
//---------------------------------------------------------------------------

    number_frame_[FRAME_INFO_IDX] = frame_count;

  } // End of FrameInfo check.

//---------------------------------------------------------------------------
// Create initial metadata.
//---------------------------------------------------------------------------

  if (Access == CREATE) {
    meta_.add<unsigned>("Max_number_frame", max_number_frame_);
    meta_.add<unsigned>("Max_number_subframe", max_number_subframe_);
    meta_.add<std::string>("Epoch (UTC)","1993-01-01T00:00:00.000000Z");
    meta_.add<std::string>("granule_id",boost::filesystem::path(file_.filename()).filename());
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Returns an array of data dimensioned by pixel and subframe for a
/// given row and frame number.
/////////////////////////////////////////////////////////////////////////////

boost::multi_array<unsigned short, 2>
MSPI::Shared::L1AFile::read_data(
  int Row_number,
  int Frame_number
) const
{
  herr_t status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  hid_t sid = file_.swath_id(swath_name_row);

//---------------------------------------------------------------------------
// Read SubframeCount field to number of subframes
//---------------------------------------------------------------------------

  int number_subframe;		// Number of subframes in this row/frame

  {
    hssize_t start[1];
    start[0] = Frame_number;
    hsize_t edge[1];
    edge[0] = 1;

    status = HE5_SWreadfield(sid, (char *)"SubframeCount", start, NULL, edge,
			     &(number_subframe));
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
		      ", field: SubframeCount");
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
    long strbufsize;
    long ndims = HE5_SWnentries(sid, HE5_HDFE_NENTDIM, &strbufsize);
    if (ndims == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWnentries, swath: " + swath_name_row);
    }

    int rank;
    std::vector<hsize_t> dims(ndims);
    hid_t datatype;

    status = HE5_SWfieldinfo(sid, (char *)field_name_dn.c_str(),
			     &rank, &(dims[0]), &datatype, NULL,NULL);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWfieldinfo, field name: " +
		      field_name_dn + ",  swath: " + swath_name_row +
		      ",  Filename: " + file_.filename());
    }

    if (datatype != HE5T_NATIVE_USHORT) {
      throw MSPI_EXCEPTIONm("Unexpected datatype for field: " +
		      field_name_dn + ",  swath: " + swath_name_row +
		      ",  Filename: " + file_.filename());
    }
  }

//---------------------------------------------------------------------------
// Read dn data.
//---------------------------------------------------------------------------

  if (number_subframe > 0) {
    hssize_t start[3];
    start[0] = Frame_number;
    start[1] = 0;
    start[2] = 0;
    hsize_t edge[3];
    edge[0] = 1;
    edge[1] = number_subframe;
    edge[2] = number_pixel();

    status = HE5_SWreadfield(sid, (char *)field_name_dn.c_str(),
			     start, NULL, edge, data.data());
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
MSPI::Shared::L1AFile::read_float(
  int Row_number,
  int Field_index,
  int Frame_number
) const
{
  herr_t status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  hid_t sid = file_.swath_id(swath_name_row);

//---------------------------------------------------------------------------
// Read SubframeCount field to number of subframes
//---------------------------------------------------------------------------

  int number_subframe;		// Number of subframes in this row/frame

  {
    hssize_t start[1];
    start[0] = Frame_number;
    hsize_t edge[1];
    edge[0] = 1;

    status = HE5_SWreadfield(sid, (char *)"SubframeCount", start, NULL, edge,
			     &(number_subframe));
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
		      ", field: SubframeCount");
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
    long strbufsize;
    long ndims = HE5_SWnentries(sid, HE5_HDFE_NENTDIM, &strbufsize);
    if (ndims == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWnentries, swath: " + swath_name_row);
    }

    int rank;
    std::vector<hsize_t> dims(ndims);
    hid_t datatype;

    status = HE5_SWfieldinfo(sid, (char *)field_name_float.c_str(),
			 &rank, &(dims[0]), &datatype, NULL, NULL);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWfieldinfo, field name: " +
		      field_name_float + ",  swath: " + swath_name_row +
		      ",  Filename: " + file_.filename());
    }

    if (datatype != HE5T_NATIVE_FLOAT) {
      throw MSPI_EXCEPTIONm("Unexpected datatype for field: " +
		      field_name_float + ",  swath: " + swath_name_row +
		      ",  Filename: " + file_.filename());
    }
  }

//---------------------------------------------------------------------------
// Read data.
//---------------------------------------------------------------------------

  if (number_subframe > 0) {
    hssize_t start[2];
    start[0] = Frame_number;
    start[1] = 0;
    hsize_t edge[2];
    edge[0] = 1;
    edge[1] = number_subframe;

    status = HE5_SWreadfield(sid, (char *)field_name_float.c_str(),
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
MSPI::Shared::L1AFile::write_frame(
  int Row_number,
  size_t Frame_number,
  const std::vector<float>& P1,
  const std::vector<float>& P2,
  const std::vector<float>& Xn,
  const boost::multi_array<unsigned short, 2>& Dn
)
{
  float p1_fill = fill<float>(Row_number, field_name(Row_number, FIELD_INDEX_P1));
  float p2_fill = fill<float>(Row_number, field_name(Row_number, FIELD_INDEX_P2));
  float xn_fill = fill<float>(Row_number, field_name(Row_number, FIELD_INDEX_XN));
  unsigned short dn_fill = fill<unsigned short>(Row_number, field_name(Row_number, FIELD_INDEX_DN));
  
  boost::multi_array<float,2> p1(boost::extents[1][max_number_subframe_]);
  std::fill(p1.data(), p1.data() + p1.num_elements(), p1_fill);
  boost::multi_array<float,2> p2(boost::extents[1][max_number_subframe_]);
  std::fill(p2.data(), p2.data() + p2.num_elements(), p2_fill);
  boost::multi_array<float,2> xn(boost::extents[1][max_number_subframe_]);
  std::fill(xn.data(), xn.data() + xn.num_elements(), xn_fill);
  boost::multi_array<unsigned short,3> dn(boost::extents[1][max_number_subframe_][number_pixel_]);
  std::fill(dn.data(), dn.data() + dn.num_elements(), dn_fill);

  {
    size_t max = std::min(P1.size(), max_number_subframe_);
    for (size_t i = 0 ; i < max ; i++) {
      p1[0][i] = P1[i];
    }
  }

  {
    size_t max = std::min(P2.size(), max_number_subframe_);
    for (size_t i = 0 ; i < max ; i++) {
      p2[0][i] = P2[i];
    }
  }

  {
    size_t max = std::min(Xn.size(), max_number_subframe_);
    for (size_t i = 0 ; i < max ; i++) {
      xn[0][i] = Xn[i];
    }
  }

  size_t max_i = std::min(Dn.shape()[0], max_number_subframe_);
  size_t max_j = std::min(Dn.shape()[1], number_pixel_);
  for (size_t i = 0 ; i < max_i ; i++) {
    for (size_t j = 0 ; j < max_j ; j++) {
      dn[0][i][j] = Dn[i][j];
    }
  }

  std::vector<unsigned> subframe_count(1,P1.size());

  write_frames(Row_number, Frame_number, p1, p2, xn, dn, subframe_count);
}



/////////////////////////////////////////////////////////////////////////////
/// Write data for a collection of frames.  Subframe dimension must be
/// max_number_subframe().  Partial frames are not allowed.  Missing subframe
/// data should be initialized to an appropriate fill value by the caller.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::L1AFile::write_frames(
  int Row_number,
  size_t Start_frame_number,
  const boost::multi_array<float,2>& P1,
  ///< (frame, subframe)
  const boost::multi_array<float,2>& P2, 
  ///< (frame, subframe)
  const boost::multi_array<float,2>& Xn, 
  ///< (frame, subframe)
  const boost::multi_array<unsigned short, 3>& Dn,
  ///< (frame, subframe, pixel)
  const std::vector<unsigned int>& Subframe_count
  ///< Number of valid subframes in each frame.
)
{
  herr_t status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Argument check: Start_frame_number < number_frame(Row_number)
//---------------------------------------------------------------------------

  if (Start_frame_number < number_frame(Row_number)) {
      std::ostringstream msg;
      msg << "Start_frame_number < number_frame(Row_number) = " 
	  << number_frame(Row_number);
      throw MSPI_EXCEPTIONm(msg.str());
  }

//---------------------------------------------------------------------------
// Get dimension sizes.
//---------------------------------------------------------------------------

  size_t number_frame = P1.shape()[0];
  size_t number_subframe = P1.shape()[1];

//---------------------------------------------------------------------------
// Argument check: number_frame > max_number_frame()
//---------------------------------------------------------------------------

  if (Start_frame_number + number_frame > max_number_frame()) {
    throw MSPI_EXCEPTIONm("Start_frame_number + number_frame > max_number_frame()");
  }

//---------------------------------------------------------------------------
// Argument check: number_subframe != max_number_subframe()
//---------------------------------------------------------------------------

  if (number_subframe != max_number_subframe()) {
    throw MSPI_EXCEPTIONm("number_subframe != max_number_subframe()");
  }

//---------------------------------------------------------------------------
// Argument check: P2.shape()[0] != number_frame
//                 P2.shape()[1] != number_subframe
//---------------------------------------------------------------------------

  if (P2.shape()[0] != number_frame) {
    throw MSPI_EXCEPTIONm("P2.shape()[0] != number_frame");
  }
  if (P2.shape()[1] != number_subframe) {
    throw MSPI_EXCEPTIONm("P2.shape()[1] != number_subframe");
  }

//---------------------------------------------------------------------------
// Argument check: Xn.shape()[0] != number_subframe
//                 Xn.shape()[1] != number_subframe
//---------------------------------------------------------------------------

  if (Xn.shape()[0] != number_frame) {
    throw MSPI_EXCEPTIONm("Xn.shape()[0] != number_frame");
  }
  if (Xn.shape()[1] != number_subframe) {
    throw MSPI_EXCEPTIONm("Xn.shape()[1] != number_subframe");
  }

//---------------------------------------------------------------------------
// Argument check: Dn.shape()[0] != number_frame
//                 Dn.shape()[1] != number_subframe
//                 Dn.shape()[2] != number_pixel
//---------------------------------------------------------------------------

  if (Dn.shape()[0] != number_frame) {
    throw MSPI_EXCEPTIONm("Dn.shape()[0] != number_frame");
  }
  if (Dn.shape()[1] != number_subframe) {
    throw MSPI_EXCEPTIONm("Dn.shape()[1] != number_subframe");
  }
  if (Dn.shape()[2] != number_pixel()) {
    throw MSPI_EXCEPTIONm("Dn.shape()[2] != number_pixel()");
  }

//---------------------------------------------------------------------------
// Argument check: Subframe_count.size() != number_frame
//---------------------------------------------------------------------------

  if (Subframe_count.size() != number_frame) {
    throw MSPI_EXCEPTIONm("Subframe_count.size() != number_frame");
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
    hssize_t start[2];
    start[0] = Start_frame_number;
    start[1] = 0;
    hsize_t edge[2];
    edge[0] = number_frame;
    edge[1] = number_subframe;

    status = HE5_SWwritefield(sid, (char *)field_name_p1.c_str(),
			      start, NULL, edge, (void *)P1.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
			    ", field: " + field_name_p1);
    }
  }

//---------------------------------------------------------------------------
// Write P2 data.
//---------------------------------------------------------------------------

  {
    hssize_t start[2];
    start[0] = Start_frame_number;
    start[1] = 0;
    hsize_t edge[2];
    edge[0] = number_frame;
    edge[1] = number_subframe;

    status = HE5_SWwritefield(sid, (char *)field_name_p2.c_str(),
			      start, NULL, edge, (void *)P2.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
			    ", field: " + field_name_p2);
    }
  }

//---------------------------------------------------------------------------
// Write time data.
//---------------------------------------------------------------------------

  {
    hssize_t start[2];
    start[0] = Start_frame_number;
    start[1] = 0;
    hsize_t edge[2];
    edge[0] = number_frame;
    edge[1] = number_subframe;

    status = HE5_SWwritefield(sid, (char *)field_name_time.c_str(),
			      start, NULL, edge, (void *)Xn.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
			    ", field: " + field_name_time);
    }
  }

//---------------------------------------------------------------------------
// Write dn data.
//---------------------------------------------------------------------------

  {
    hssize_t start[3];
    start[0] = Start_frame_number;
    start[1] = 0;
    start[2] = 0;
    hsize_t edge[3];
    edge[0] = number_frame;
    edge[1] = number_subframe;
    edge[2] = number_pixel();

    status = HE5_SWwritefield(sid, (char *)field_name_dn.c_str(),
			      start, NULL, edge, (void *)Dn.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
			    ", field: " + field_name_dn);
    }
  }

//---------------------------------------------------------------------------
// Write the SubframeCount.
//---------------------------------------------------------------------------

  {
    hssize_t start[1];
    start[0] = Start_frame_number;
    hsize_t edge[1];
    edge[0] = number_frame;

    status = HE5_SWwritefield(sid, (char *)"SubframeCount",
			      start, NULL, edge, (void *)&Subframe_count[0]);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWwritefield, swath: " + swath_name_row + 
			    ", field: SubframeCount");
    }
  }

//---------------------------------------------------------------------------
// Update FrameCount attribute for this row.
//---------------------------------------------------------------------------

  number_frame_[Row_number] = std::max(number_frame_[Row_number],
				       Start_frame_number + number_frame);
  uint32_t frame_count = number_frame_[Row_number];
  hsize_t size = 1;
  status = HE5_SWwriteattr(sid, "FrameCount", H5T_NATIVE_UINT32, &size,
                           &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " + swath_name_row);
  }
}


/////////////////////////////////////////////////////////////////////////////
/// Read data for a collection of frames.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::L1AFile::read_frames(
  int Row_number,
  size_t Start_frame_number,
  size_t Number_frame,
  boost::multi_array<float,2>& P1,
  ///< (frame, subframe)
  boost::multi_array<float,2>& P2, 
  ///< (frame, subframe)
  boost::multi_array<float,2>& Xn, 
  ///< (frame, subframe)
  boost::multi_array<unsigned short, 3>& Dn,
  ///< (frame, subframe, pixel)
  std::vector<unsigned int>& Subframe_count
  ///< Number of valid subframes in each frame.
) const
{
  herr_t status;		// Status code returned by HDF-EOS

//---------------------------------------------------------------------------
// Argument check: Start_frame_number + Number_frame > number_frame(Row_number)
//---------------------------------------------------------------------------

  if (Start_frame_number + Number_frame > number_frame(Row_number)) {
    throw MSPI_EXCEPTIONm("Start_frame_number + Number_Frame > number_frame(Row_number)");
  }

//---------------------------------------------------------------------------
// Allocate space
//---------------------------------------------------------------------------
  
  P1.resize(boost::extents[Number_frame][max_number_subframe_]);
  P2.resize(boost::extents[Number_frame][max_number_subframe_]);
  Xn.resize(boost::extents[Number_frame][max_number_subframe_]);
  Dn.resize(boost::extents[Number_frame][max_number_subframe_][number_pixel()]);
  Subframe_count.assign(Number_frame,0);

//---------------------------------------------------------------------------
// Get swath name for this row.
//---------------------------------------------------------------------------

  std::string swath_name_row = swath_name(Row_number);

//---------------------------------------------------------------------------
// Attach to swath for this row.
//---------------------------------------------------------------------------

  hid_t sid = file_.swath_id(swath_name_row);

//---------------------------------------------------------------------------
// Read P1 data.
//---------------------------------------------------------------------------

  {
    std::string fieldname = field_name(Row_number, FIELD_INDEX_P1);
    hssize_t start[2];
    start[0] = Start_frame_number;
    start[1] = 0;
    hsize_t edge[2];
    edge[0] = Number_frame;
    edge[1] = max_number_subframe_;

    status = HE5_SWreadfield(sid, (char *)fieldname.c_str(),
			     start, NULL, edge, P1.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
			    ", field: " + fieldname);
    }
  }

//---------------------------------------------------------------------------
// Read P2 data.
//---------------------------------------------------------------------------

  {
    std::string fieldname = field_name(Row_number, FIELD_INDEX_P2);
    hssize_t start[2];
    start[0] = Start_frame_number;
    start[1] = 0;
    hsize_t edge[2];
    edge[0] = Number_frame;
    edge[1] = max_number_subframe_;

    status = HE5_SWreadfield(sid, (char *)fieldname.c_str(),
			     start, NULL, edge, P2.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
			    ", field: " + fieldname);
    }
  }

//---------------------------------------------------------------------------
// Read XN data.
//---------------------------------------------------------------------------

  {
    std::string fieldname = field_name(Row_number, FIELD_INDEX_XN);
    hssize_t start[2];
    start[0] = Start_frame_number;
    start[1] = 0;
    hsize_t edge[2];
    edge[0] = Number_frame;
    edge[1] = max_number_subframe_;

    status = HE5_SWreadfield(sid, (char *)fieldname.c_str(),
			     start, NULL, edge, Xn.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
			    ", field: " + fieldname);
    }
  }

//---------------------------------------------------------------------------
// Read DN data.
//---------------------------------------------------------------------------

  {
    std::string fieldname = field_name(Row_number, FIELD_INDEX_DN);
    hssize_t start[3];
    start[0] = Start_frame_number;
    start[1] = 0;
    start[2] = 0;
    hsize_t edge[3];
    edge[0] = Number_frame;
    edge[1] = max_number_subframe_;
    edge[2] = number_pixel();

    status = HE5_SWreadfield(sid, (char *)fieldname.c_str(),
			     start, NULL, edge, Dn.data());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
			    ", field: " + fieldname);
    }
  }

//---------------------------------------------------------------------------
// Read SubframeCount.
//---------------------------------------------------------------------------

  {
    std::string fieldname = "SubframeCount";
    hssize_t start[1];
    start[0] = Start_frame_number;
    hsize_t edge[1];
    edge[0] = Number_frame;

    status = HE5_SWreadfield(sid, (char *)fieldname.c_str(), 
			     start, NULL, edge, &(Subframe_count[0]));
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + swath_name_row + 
			    ", field: " + fieldname);
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
/// Add row (channel) with the given attributes.  This creates a new swath 
/// structure in the file.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::L1AFile::add_row(
  int Row_number,
  float Wavelength,
  float Polarization_angle
)
{
  herr_t status;		// Status code returned by HDF-EOS

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
//---------------------------------------------------------------------------

  status = HE5_SWdefdim(sid,(char *)"Pixel",number_pixel());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdim, dimension: Pixel, swath: " + swath_name_row);
  }
  status = HE5_SWdefdim(sid,(char *)"Subframe",max_number_subframe());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdim, dimension: Subframe, swath: " + swath_name_row);
  }
  status = HE5_SWdefdim(sid,(char *)"Frame",frame_dim_size_);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdim, dimension: Frame, swath: " + swath_name_row);
  }

//---------------------------------------------------------------------------
// Create P1 field.
//---------------------------------------------------------------------------

  {
    std::string fieldname = field_name(Row_number, FIELD_INDEX_P1);
    std::vector<hsize_t> chunk_dims(2);
    chunk_dims[0] = frame_dim_size_;
    chunk_dims[1] = max_number_subframe();
    create_field<float>(sid, fieldname, "Frame,Subframe", chunk_dims, 
			fill<float>(Row_number, fieldname));
  }

//---------------------------------------------------------------------------
// Create P2 field.
//---------------------------------------------------------------------------

  {
    std::string fieldname = field_name(Row_number, FIELD_INDEX_P2);
    std::vector<hsize_t> chunk_dims(2);
    chunk_dims[0] = frame_dim_size_;
    chunk_dims[1] = max_number_subframe();
    create_field<float>(sid, fieldname, "Frame,Subframe", chunk_dims, 
			fill<float>(Row_number, fieldname));
  }

//---------------------------------------------------------------------------
// Create XN field.
//---------------------------------------------------------------------------

  {
    std::string fieldname = field_name(Row_number, FIELD_INDEX_XN);
    std::vector<hsize_t> chunk_dims(2);
    chunk_dims[0] = frame_dim_size_;
    chunk_dims[1] = max_number_subframe();
    create_field<float>(sid, fieldname, "Frame,Subframe", chunk_dims, 
			fill<float>(Row_number, fieldname));

  }

//---------------------------------------------------------------------------
// Create DN field.
//---------------------------------------------------------------------------

  {
    std::string fieldname = field_name(Row_number, FIELD_INDEX_DN);
    std::vector<hsize_t> chunk_dims(3);
    chunk_dims[0] = frame_chunk_size_;
    chunk_dims[1] = max_number_subframe();
    chunk_dims[2] = number_pixel();
    bool compressed = true;
    create_field<unsigned short>(sid, fieldname, "Frame,Subframe,Pixel", 
				 chunk_dims, 
				 fill<unsigned short>(Row_number, fieldname), 
				 compressed);
  }

//---------------------------------------------------------------------------
// Create SubframeCount field.
//---------------------------------------------------------------------------

  {
    std::string fieldname = "SubframeCount";
    std::vector<hsize_t> chunk_dims(1);
    chunk_dims[0] = frame_dim_size_;
    unsigned fill_value = 0;
    create_field<unsigned>(sid, fieldname, "Frame", chunk_dims, fill_value);
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



/////////////////////////////////////////////////////////////////////////////
/// Returns the frame info for the specified frame number.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::FrameInfo
MSPI::Shared::L1AFile::frame_info(
  int Frame_number
) const
{
  herr_t status;		// Status code returned by HDF-EOS

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

  hid_t sid = file_.swath_id(frameinfo_swath_name);

//---------------------------------------------------------------------------
// Iterate over FrameInfo fields reading each one.
//---------------------------------------------------------------------------

  for (std::map<std::string,double>::const_iterator ifield = frameinfo.begin();
       ifield != frameinfo.end(); ++ifield) {
    std::string fieldname = ifield->first;
    double data = 0.0;

    hssize_t start[1];
    start[0] = Frame_number;
    hsize_t edge[1];
    edge[0] = 1;

    status = HE5_SWreadfield(sid, (char *)fieldname.c_str(),
			     start, NULL, edge, &data);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWreadfield, swath: " + frameinfo_swath_name + 
		      ", field: " + fieldname);
    }

    frameinfo[fieldname] = data;
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
MSPI::Shared::L1AFile::write_frame_info(
  size_t Frame_number,
  const FrameInfo& Frame_info
)
{
  herr_t status;		// Status code returned by HDF-EOS

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

  hid_t sid = file_.swath_id(frameinfo_swath_name);

//---------------------------------------------------------------------------
// Iterate over FrameInfo fields writing each one.
//---------------------------------------------------------------------------

  for (std::map<std::string,double>::const_iterator ifield = frameinfo.begin();
       ifield != frameinfo.end(); ++ifield) {
    std::string fieldname = ifield->first;
    double data = ifield->second;

    hssize_t start[1];
    start[0] = Frame_number;
    hsize_t edge[1];
    edge[0] = 1;

    status = HE5_SWwritefield(sid, (char *)fieldname.c_str(), 
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
  hsize_t size = 1;
  status = HE5_SWwriteattr(sid, "FrameCount", H5T_NATIVE_UINT32, &size,
                           &frame_count);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWwriteattr: FrameCount, swath: " 
		    + frameinfo_swath_name);
  }

}

/////////////////////////////////////////////////////////////////////////////
/// Destructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::L1AFile::~L1AFile()
{
  if (not read_only_) {
    meta_.hdf5_export(file_.file_attr_group_id());
  }
}
