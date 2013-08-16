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

#ifndef HDF4_L1B1_FILE_H
#define HDF4_L1B1_FILE_H

#include "hdf.h"                  // Definition of int32

#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include "MSPI-Shared/File/hdf4/Table/src/row_table.h"
				  // Definition of RowTable
#include "MSPI-Shared/File/FieldMap/src/field_map.h"
				  // Definition of FieldMap
#include "MSPI-Shared/File/FileHandle/src/hdfeos2_file_handle.h"
				  // Definition of Hdfeos2FileHandle
#include "MSPI-Shared/File/Meta/src/hdf4_meta.h"
				  // Definition of Hdf4Meta

namespace MSPI {
  namespace Shared {
    namespace HDF4 {
      class L1B1File;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief File access interface for MSPI L1B1 files.
///
/// Definitions:
/// - "Row" refers to a row (numbered 0-63) on the CCD array in the MSPI camera.
///   Each spectral/polarimetric channel is assigned to a specific row.  For
///   example row 32 is used for the 660 nm intensity channel.
///
/// - "Frame" refers to a data aquisition for a specific channel at an instant
///    in time.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::HDF4::L1B1File {
public:
  typedef enum {
    CREATE = 0,
    WRITE = 1,
    READ = 2
  } AccessType;

  L1B1File(const std::string& Filename, AccessType Access, 
	   const FieldMap& Field_map);
  L1B1File(const std::string& Filename);

  ~L1B1File();

  std::string epoch() const;

  void epoch(const std::string& Epoch);

  size_t number_frame(int Row_number) const;
  
  size_t number_pixel() const;

  std::vector<int> row_numbers() const;

  std::vector<std::string> field_names(int Row_number) const;

  void add_row(int Row_number, float Wavelength, float Polarization_angle,
	       size_t Max_number_frame);

  std::vector<double> read_time(int Row_number, size_t Start_frame, 
				size_t Number_frame) const;

  void write_time(int Row_number, size_t Start_frame, 
		  const std::vector<double>& Time);
  
  boost::multi_array<float, 2> read_data(int Row_number,
					 const std::string& Field_name, 
					 size_t Start_frame, 
					 size_t Number_frame) const; 

  void write_data(int Row_number,
		  const std::string& Field_name, 
		  size_t Start_frame,
		  const boost::multi_array<float,2>& Data); 

  void append(int Row_number, double Time, const std::vector<std::vector<float> >& Data);

  float wavelength(int Row_number) const;

  float polarization_angle(int Row_number) const;

  std::string swath_name(int Row_number) const;

  void add_meta(const Meta& Meta);
  const Meta& meta() const;
  std::string granule_id() const;

protected:

private:
  int read_frame_count(int Row_number) const;
  const size_t number_pixel_;
  Hdfeos2FileHandle file_;
  Hdf4Meta meta_;
  RowTable row_table_;
  FieldMap field_map_;             ///< Contains swath name and field names 
                                   ///< for each row.

  bool read_only_;
  std::map<int,size_t> number_frame_; ///< Number of frames written for each row.
};

/////////////////////////////////////////////////////////////////////////////
/// Return granule identifier.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::HDF4::L1B1File::granule_id() const
{
  return meta_.get_value<std::string>("granule_id");
}

/////////////////////////////////////////////////////////////////////////////
/// Add the metadata to the current collection.  Throws an exception if
/// existing metadata does not match.
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::HDF4::L1B1File::add_meta(const Meta& Meta)
{
  return meta_.add(Meta);
}

/////////////////////////////////////////////////////////////////////////////
/// Return metadata.
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::Meta&
MSPI::Shared::HDF4::L1B1File::meta() const
{
  return meta_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns the number of frames in the file for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::HDF4::L1B1File::number_frame(
  int Row_number
) const
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
MSPI::Shared::HDF4::L1B1File::number_pixel() const
{
  return number_pixel_;
}



/////////////////////////////////////////////////////////////////////////////
/// Return wavelength for the given row number.
/////////////////////////////////////////////////////////////////////////////
   
inline float
MSPI::Shared::HDF4::L1B1File::wavelength(int Row_number) const
{
  return row_table_.wavelength(Row_number);
}
    


/////////////////////////////////////////////////////////////////////////////
/// Return polarization angle for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::HDF4::L1B1File::polarization_angle(int Row_number) const
{
  return row_table_.polarization_angle(Row_number);
}



/////////////////////////////////////////////////////////////////////////////
/// Return swath name for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline std::string 
MSPI::Shared::HDF4::L1B1File::swath_name(int Row_number) const
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
MSPI::Shared::HDF4::L1B1File::row_numbers() const
{
  return row_table_.row_numbers();
}



/////////////////////////////////////////////////////////////////////////////
/// Returns the vector of field names for the given row number, as
/// defined by the Field Map input to the constructor.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string> 
MSPI::Shared::HDF4::L1B1File::field_names(int Row_number) const
{
  return field_map_.field_names(Row_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Return epoch value.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::HDF4::L1B1File::epoch() const
{
  return meta_.get_value<std::string>("Epoch (UTC)");
}

/////////////////////////////////////////////////////////////////////////////
/// Set epoch value.  This should be a CCSDS ASCII UTC time string.
/// Example: 1993-01-01T00:00:00.000000Z
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::HDF4::L1B1File::epoch(const std::string& Epoch)
{
  meta_.add<std::string>("Epoch (UTC)",Epoch);
}

#endif
