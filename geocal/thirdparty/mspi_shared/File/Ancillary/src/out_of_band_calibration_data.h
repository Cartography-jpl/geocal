//===========================================================================
//                                                                         
//                          OutOfBandCalibrationData                        
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef OUT_OF_BAND_CALIBRATION_DATA_H
#define OUT_OF_BAND_CALIBRATION_DATA_H

#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include <map>			  // Definition of std::map
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <vector> 		// Definition of std::vector
#include "MSPI-Shared/File/Meta/src/hdf5_meta.h"
				  // Definition of Hdf5Meta

namespace MSPI {
  namespace Shared {
    class OutOfBandCalibrationData;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides out-of-band calibration data.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::OutOfBandCalibrationData {
public:
  OutOfBandCalibrationData(const std::vector<int>& Row_number,
			   const boost::multi_array<float,3>& Data);
  OutOfBandCalibrationData(const std::string& Filename);
  const boost::multi_array<float,2>& data(int Row_number) const;
  const std::vector<int>& row_numbers() const;
  size_t number_row() const;
  size_t number_col() const;
  size_t number_pixel() const;
  std::string granule_id() const;

private:
  void init(const std::vector<int>& Row_number,
	    const boost::multi_array<float,3>& Data);
  size_t row_index(int Row_number) const;
  size_t number_row_;
  size_t number_col_;
  size_t number_pixel_;
  std::vector<boost::multi_array<float,2> > data_; ///< (row, column, pixel)
  std::vector<int> row_number_;
  std::map<int,size_t> row_index_;
  Hdf5Meta meta_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return granule identifier.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::OutOfBandCalibrationData::granule_id() const
{
  return meta_.get_value<std::string>("granule_id");
}

/////////////////////////////////////////////////////////////////////////////
/// Return vector of row numbers.
/////////////////////////////////////////////////////////////////////////////

inline const std::vector<int>&
MSPI::Shared::OutOfBandCalibrationData::row_numbers() const
{
  return row_number_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the number of rows.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::OutOfBandCalibrationData::number_row() const
{
  return number_row_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the number of columns.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::OutOfBandCalibrationData::number_col() const
{
  return number_col_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the number of pixels.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::OutOfBandCalibrationData::number_pixel() const
{
  return number_pixel_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return coefficients for the given row number.  (column, pixel)
/////////////////////////////////////////////////////////////////////////////

inline
const boost::multi_array<float,2>& 
MSPI::Shared::OutOfBandCalibrationData::data(int Row_number) const
{
  return data_[row_index(Row_number)];
}

//////////////////////////////////////////////////////////////////////////////
/// Private method returning row index associated with the given detector 
/// row number.
//////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::OutOfBandCalibrationData::row_index(int Row_number) const
{
  std::map<int,size_t>::const_iterator f = row_index_.find(Row_number);
  if (f == row_index_.end()) {
    std::ostringstream m;
    m << "OutOfBandCalibrationData: row number not available: " << Row_number;
    throw MSPI_EXCEPTIONm(m.str());
  }
  return f->second;
}

#endif
