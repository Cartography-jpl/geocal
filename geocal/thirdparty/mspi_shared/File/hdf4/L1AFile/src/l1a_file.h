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

#ifndef HDF4_L1A_FILE_H
#define HDF4_L1A_FILE_H

#include "hdf.h"                  // Definition of int32

#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include "MSPI-Shared/File/hdf4/Table/src/row_table.h"
				  // Definition of RowTable
#include "MSPI-Shared/File/FieldMap/src/field_map.h"
                                  // Definition of FieldMap
#include "MSPI-Shared/File/L1AFile/src/frame_info.h"
                                  // Definition of FrameInfo
#include "MSPI-Shared/File/FileHandle/src/hdfeos2_file_handle.h"
				  // Definition of Hdfeos2FileHandle
#include "MSPI-Shared/File/Meta/src/hdf4_meta.h"
				  // Definition of Hdf4Meta

#define FRAME_INFO_IDX -1	  // FrameInfo index into number_frame_

namespace MSPI {
  namespace Shared {
    namespace HDF4 {
      class L1AFile;
    }
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

class MSPI::Shared::HDF4::L1AFile {
public:
  typedef enum {
    CREATE = 0,
    WRITE = 1,
    READ = 2
  } AccessType;

  L1AFile(const std::string& Filename, AccessType Access, const FieldMap& Field_map);

  ~L1AFile();

  std::string epoch() const;

  int number_frame(int Row_number) const;

  size_t number_pixel() const;

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

  void write_frame_info(int Frame_number, const FrameInfo& Frame_info);

  void write_frame(int Row_number, int Frame_number, 
		   const std::vector<float>& P1, 
		   const std::vector<float>& P2, 
		   const std::vector<float>& Xn, 
		   const boost::multi_array<unsigned short, 2>& Dn);

  float wavelength(int Row_number) const;

  float polarization_angle(int Row_number) const;

  std::string swath_name(int Row_number) const;
  void add_meta(const Meta& Meta);
  const Meta& meta() const;
  std::string granule_id() const;

protected:

private:
  enum  {
    FIELD_INDEX_P1=0,
    FIELD_INDEX_P2=1,
    FIELD_INDEX_XN=2,
    FIELD_INDEX_DN=3
  };  ///< Indices of fields in the field map.  Order must be consistent with 
      ///< the order of field names in the field map.

  const size_t number_pixel_;            ///< Number of pixels in sensor.
  const size_t max_number_subframe_;     ///< Max number of subframes in each frame.
  const int max_number_frame_;        ///< Max number of frames. 
  std::map<int, int> number_frame_;   ///< Number of frames written to each row.
  Hdfeos2FileHandle file_;
  Hdf4Meta meta_;
  FieldMap field_map_;                ///< Field map for file.
  RowTable row_table_;                ///< Row table for file.

  std::map<int,int32> dn_sds_id_;
  std::map<int,int32> sw_sid_;
  int32 sw_sid(int Row) const;
  int32 dn_sds_id(int Row) const;
  std::string epoch_;
};

inline int32
MSPI::Shared::HDF4::L1AFile::sw_sid(int Row) const
{
  std::map<int,int>::const_iterator f = sw_sid_.find(Row);
  if (f == sw_sid_.end()) {
    std::ostringstream m;
    m << "Invalid row number: "<<Row;
    throw MSPI_EXCEPTIONm(m.str());
  }
  return f->second;
}

inline int32
MSPI::Shared::HDF4::L1AFile::dn_sds_id(int Row) const
{
  std::map<int,int>::const_iterator f = dn_sds_id_.find(Row);
  if (f == dn_sds_id_.end()) {
    std::ostringstream m;
    m << "Invalid row number: "<<Row;
    throw MSPI_EXCEPTIONm(m.str());
  }
  return f->second;
}

/////////////////////////////////////////////////////////////////////////////
/// Return granule identifier.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::HDF4::L1AFile::granule_id() const
{
  return meta_.get_value<std::string>("granule_id");
}

/////////////////////////////////////////////////////////////////////////////
/// Add the metadata to the current collection.  Throws an exception if
/// existing metadata does not match.
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::HDF4::L1AFile::add_meta(const Meta& Meta)
{
  return meta_.add(Meta);
}

/////////////////////////////////////////////////////////////////////////////
/// Return metadata.
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::Meta&
MSPI::Shared::HDF4::L1AFile::meta() const
{
  return meta_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns the number of frames in the file for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline int
MSPI::Shared::HDF4::L1AFile::number_frame(int Row_number) const
{

//---------------------------------------------------------------------------
// Get the number of frames for this row number.
// If the row number has not been created, return 0.
//---------------------------------------------------------------------------

  std::map<int,int>::const_iterator 
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
MSPI::Shared::HDF4::L1AFile::number_pixel() const
{
  return number_pixel_;
}



/////////////////////////////////////////////////////////////////////////////
/// Returns the maximum size of the subframe dimension.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::HDF4::L1AFile::max_number_subframe() const
{
  return max_number_subframe_;
}



/////////////////////////////////////////////////////////////////////////////
/// Return wavelength for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::HDF4::L1AFile::wavelength(int Row_number) const
{
  return row_table_.wavelength(Row_number);
}



/////////////////////////////////////////////////////////////////////////////
/// Return polarization_angle for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::HDF4::L1AFile::polarization_angle(int Row_number) const
{
  return row_table_.polarization_angle(Row_number);
}



/////////////////////////////////////////////////////////////////////////////
/// Return swath name for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline std::string 
MSPI::Shared::HDF4::L1AFile::swath_name(int Row_number) const
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
MSPI::Shared::HDF4::L1AFile::row_numbers() const
{
  return row_table_.row_numbers();
}



/////////////////////////////////////////////////////////////////////////////
/// Returns the vector of field names for the given row number, as
/// defined by the Field Map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::HDF4::L1AFile::field_name(int Row_number, int Field_index) const
{
  return field_map_.field_name(Row_number, Field_index);
}

/////////////////////////////////////////////////////////////////////////////
/// Returns the vector of field names for the given row number, as
/// defined by the Field Map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string> 
MSPI::Shared::HDF4::L1AFile::field_names(int Row_number) const
{
  return field_map_.field_names(Row_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Returns a vector of all subframe times for a given row and frame number.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<float>
MSPI::Shared::HDF4::L1AFile::read_time(
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
MSPI::Shared::HDF4::L1AFile::read_p1(
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
MSPI::Shared::HDF4::L1AFile::read_p2(
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
MSPI::Shared::HDF4::L1AFile::epoch() const
{
  return meta_.get_value<std::string>("Epoch (UTC)");
}

#endif
