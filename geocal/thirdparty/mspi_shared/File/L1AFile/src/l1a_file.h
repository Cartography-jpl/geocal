//===========================================================================
//                                                                         
//                                 L1AFile                                 
//                                                                         
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef L1A_FILE_H
#define L1A_FILE_H

#include "HE5_HdfEosDef.h"	  // Definition of hid_t

#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include "MSPI-Shared/File/Table/src/row_table.h"
				  // Definition of RowTable
#include "MSPI-Shared/File/FieldMap/src/field_map.h"
                                  // Definition of FieldMap
#include "MSPI-Shared/File/L1AFile/src/frame_info.h"
                                  // Definition of FrameInfo
#include "MSPI-Shared/File/FileHandle/src/hdfeos5_file_handle.h"
				  // Definition of Hdfeos5FileHandle
#include "MSPI-Shared/File/Meta/src/hdf5_meta.h"
				  // Definition of Hdf5Meta

#define FRAME_INFO_IDX -1	  // FrameInfo index into number_frame_

namespace MSPI {
  namespace Shared {
    class L1AFile;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief File access interface for MSPI L1A files.
///
///   "Row" refers to a row (numbered 0-63) on the CCD array in MSPI camera.
///   Each spectral/polarimetric channel is assigned to a specific row.  For
///   example row 32 is used for the 660 nm intensity channel.
///
///   "Frame" refers to a data aquisition for a specific channel at an instant
///   in time.
///
///   "Subframe" refers to data aquistions within a frame over the period
///   of each high-frequency cycle of the PEM carrier wave.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::L1AFile {
public:
  typedef enum {
    CREATE = 0,
    WRITE = 1,
    READ = 2
  } AccessType;

  enum  {
    FIELD_INDEX_P1=0,
    FIELD_INDEX_P2=1,
    FIELD_INDEX_XN=2,
    FIELD_INDEX_DN=3
  };  ///< Indices of fields in the field map.  Order must be consistent with 
      ///< the order of field names in the field map.

  L1AFile(const std::string& Filename, AccessType Access, const FieldMap& Field_map,
	  size_t Max_number_frame, size_t Max_number_subframe);
  ~L1AFile();

  std::string epoch() const;

  size_t number_frame(int Row_number) const;
  
  template<class T>
  T fill(int Row_number, const std::string& Field_name) const;
  
  size_t number_pixel() const;

  size_t max_number_frame() const;

  size_t max_number_subframe() const;

  std::vector<int> row_numbers() const;

  std::vector<std::string> field_names(int Row_number) const;
  std::string field_name(int Row_number, int Field_index) const;

  void add_row(int Row_number, float Wavelength, float Polarization_angle);

  FrameInfo frame_info(int Frame_number) const;

  std::vector<float> read_time(int Row_number, int Frame_number) const;
  std::vector<float> read_p1(int Row_number, int Frame_number) const;
  std::vector<float> read_p2(int Row_number, int Frame_number) const;

  std::vector<float> read_float(int Row_number, int Field_index, int Frame_number) const;

  boost::multi_array<unsigned short, 2> read_data(int Row_number, int Frame_number) const;

  void write_frame_info(size_t Frame_number, const FrameInfo& Frame_info);

  void write_frame(int Row_number, size_t Frame_number, 
		   const std::vector<float>& P1, 
		   const std::vector<float>& P2, 
		   const std::vector<float>& Xn, 
		   const boost::multi_array<unsigned short, 2>& Dn);

  void write_frames(int Row_number, size_t Start_frame_number, 
		    const boost::multi_array<float,2>& P1,
		    const boost::multi_array<float,2>& P2, 
		    const boost::multi_array<float,2>& Xn, 
		    const boost::multi_array<unsigned short, 3>& Dn,
		    const std::vector<unsigned int>& Subframe_count);

  void read_frames(int Row_number, size_t Start_frame_number, size_t Number_frame,
		   boost::multi_array<float,2>& P1,
		   boost::multi_array<float,2>& P2, 
		   boost::multi_array<float,2>& Xn, 
		   boost::multi_array<unsigned short, 3>& Dn,
		   std::vector<unsigned int>& Subframe_count) const;

  float wavelength(int Row_number) const;

  float polarization_angle(int Row_number) const;

  std::string swath_name(int Row_number) const;

  void add_meta(const Meta& Meta);
  const Meta& meta() const;
  std::string granule_id() const;

protected:

private:
  template <class T> 
  void create_field(hid_t Swid,
		    const std::string& Field_name,
		    const std::string& Dimlist,
		    const std::vector<hsize_t> Chunk_dims,
		    T Fill_value,
		    bool Compressed = false);
  template <class T> hid_t datatype() const;

  int frame_dim_size_;	      ///< Size of frame dimension
  int frame_chunk_size_;      ///< Frame dimension chunk size, must be
			      ///< an even divisor of frame dimension.
  const size_t number_pixel_;	     ///< Number of pixels in sensor.
  size_t max_number_frame_; 
  size_t max_number_subframe_;
  std::map<int, size_t> number_frame_;   ///< Number of frames written to each row.
  Hdfeos5FileHandle file_;
  Hdf5Meta meta_;
  RowTable row_table_;                ///< Row table for file.
  FieldMap field_map_;                ///< Field map for file.
  bool read_only_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return fill value
/////////////////////////////////////////////////////////////////////////////

template<class T>
inline T
MSPI::Shared::L1AFile::fill(
  int Row_number, 
  const std::string& Field_name
) const
{
  return field_map_.fill<T>(Row_number, Field_name);
}

/////////////////////////////////////////////////////////////////////////////
/// Return granule identifier.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::L1AFile::granule_id() const
{
  return meta_.get_value<std::string>("granule_id");
}

/////////////////////////////////////////////////////////////////////////////
/// Add the metadata to the current collection.  Throws an exception if
/// existing metadata does not match.
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::L1AFile::add_meta(const Meta& Meta)
{
  return meta_.add(Meta);
}

/////////////////////////////////////////////////////////////////////////////
/// Return metadata.
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::Meta&
MSPI::Shared::L1AFile::meta() const
{
  return meta_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns the number of frames in the file for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::L1AFile::number_frame(int Row_number) const
{

//---------------------------------------------------------------------------
// Get the number of frames for this row number.
// If the row number has not been created, return 0.
//---------------------------------------------------------------------------

  std::map<int,size_t>::const_iterator 
    h = number_frame_.find(Row_number);
  
  if (h == number_frame_.end()) {
    return 0;
  }

//---------------------------------------------------------------------------
// Return
//---------------------------------------------------------------------------

  return h->second;
}



/////////////////////////////////////////////////////////////////////////////
/// Returns the size of the pixel dimension.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::L1AFile::number_pixel() const
{
  return number_pixel_;
}


/////////////////////////////////////////////////////////////////////////////
/// Returns the maximum size of the frame dimension.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::L1AFile::max_number_frame() const
{
  return max_number_frame_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns the maximum size of the subframe dimension.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::L1AFile::max_number_subframe() const
{
  return max_number_subframe_;
}



/////////////////////////////////////////////////////////////////////////////
/// Return wavelength for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::L1AFile::wavelength(int Row_number) const
{
  return row_table_.wavelength(Row_number);
}



/////////////////////////////////////////////////////////////////////////////
/// Return polarization_angle for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::L1AFile::polarization_angle(int Row_number) const
{
  return row_table_.polarization_angle(Row_number);
}



/////////////////////////////////////////////////////////////////////////////
/// Return swath name for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline std::string 
MSPI::Shared::L1AFile::swath_name(int Row_number) const
{
  return row_table_.swath_name(Row_number);
}



/////////////////////////////////////////////////////////////////////////////
/// Returns the set of row numbers in the Row Table.  This is equivalently
/// the set of rows for which there exists a swath structure in the
/// file; and may be a subset of the row numbers defined in the Field
/// Map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<int> 
MSPI::Shared::L1AFile::row_numbers() const
{
  return row_table_.row_numbers();
}



/////////////////////////////////////////////////////////////////////////////
/// Returns the vector of field names for the given row number, as
/// defined by the Field Map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::L1AFile::field_name(int Row_number, int Field_index) const
{
  return field_map_.field_name(Row_number, Field_index);
}

/////////////////////////////////////////////////////////////////////////////
/// Returns the vector of field names for the given row number, as
/// defined by the Field Map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string> 
MSPI::Shared::L1AFile::field_names(int Row_number) const
{
  return field_map_.field_names(Row_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Returns a vector of all subframe times for a given row and frame number.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<float>
MSPI::Shared::L1AFile::read_time(
  int Row_number,
  int Frame_number
) const
{
  return read_float(Row_number, FIELD_INDEX_XN, Frame_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Returns a vector of P1 values for a given row and frame number.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<float>
MSPI::Shared::L1AFile::read_p1(
  int Row_number,
  int Frame_number
) const
{
  return read_float(Row_number, FIELD_INDEX_P1, Frame_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Returns a vector of P2 values for a given row and frame number.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<float>
MSPI::Shared::L1AFile::read_p2(
  int Row_number,
  int Frame_number
) const
{
  return read_float(Row_number, FIELD_INDEX_P2, Frame_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Return epoch as a CCSDS ASCII UTC time string.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::L1AFile::epoch() const
{
  return meta_.get_value<std::string>("Epoch (UTC)");
}

/////////////////////////////////////////////////////////////////////////////
/// Return HDF datatype identifier
/// Note: template specializations must be inside a namespace block.
/////////////////////////////////////////////////////////////////////////////

namespace MSPI {
  namespace Shared {
    template <> inline hid_t L1AFile::datatype<unsigned short>() const { return H5T_NATIVE_USHORT; }
    template <> inline hid_t L1AFile::datatype<unsigned int>() const { return H5T_NATIVE_UINT; }
    template <> inline hid_t L1AFile::datatype<int>() const { return H5T_NATIVE_INT; }
    template <> inline hid_t L1AFile::datatype<float>() const { return H5T_NATIVE_FLOAT; }
    template <> inline hid_t L1AFile::datatype<double>() const { return H5T_NATIVE_DOUBLE; }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Create field.
/////////////////////////////////////////////////////////////////////////////

template <class T>
inline void
MSPI::Shared::L1AFile::create_field(
  hid_t Swid,
  const std::string& Field_name,
  const std::string& Dimlist,
  const std::vector<hsize_t> Chunk_dims,
  T Fill_value,
  bool Compressed
)
{
  herr_t status;

//---------------------------------------------------------------------------
// Set chunking.
//---------------------------------------------------------------------------

  status = HE5_SWdefchunk(Swid, Chunk_dims.size(), &Chunk_dims[0]);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefchunk");
  }

//---------------------------------------------------------------------------
// Set compression.
//---------------------------------------------------------------------------

  if (Compressed) {
    int comp_code = HE5_HDFE_COMP_DEFLATE;
    int comp_level[1] = {5};  // 1 to 9, 1=faster, 9=better compression
    herr_t status = HE5_SWdefcomp(Swid, comp_code, comp_level);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdefcomp");
    }
  }

//---------------------------------------------------------------------------
// Set fill value for this field.   This must be done *before* the call to
// HE5_SWdefdatafield.
//---------------------------------------------------------------------------

  status = HE5_SWsetfillvalue(Swid, (char *)Field_name.c_str(),
			      datatype<T>(), &Fill_value);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWsetfillvalue, field: "+Field_name);
  }

//---------------------------------------------------------------------------
// Create field.
//---------------------------------------------------------------------------

  status = HE5_SWdefdatafield(Swid, Field_name.c_str(),
			      (char *)Dimlist.c_str(),
			      NULL, 
			      datatype<T>(), HE5_HDFE_NOMERGE);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with SWdefdatafield, field: "+Field_name);
  }
}

#endif
