/*===========================================================================
=                                                                           =
=                           Rdqi                                            =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#ifndef RDQI_H
#define RDQI_H

#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include <vector>		  // Definition of std::vector
#include <map>			  // Definition of std::map
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.

namespace MSPI {
  namespace Shared {
    class Rdqi;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides per-pixel data quality indicator for each detector row.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::Rdqi {
public:
  Rdqi(const std::vector<int>& Row_number,
       const boost::multi_array<float,2>& Data);
  Rdqi(const std::string& Filename);
  const std::vector<float>& data(int Row_number) const;
  const std::vector<int>& row_numbers() const;
  size_t number_row() const;
  size_t number_pixel() const;

private:
  void init(const std::vector<int>& Row_number,
	    const boost::multi_array<float,2>& Data);
  size_t row_index(int Row_number) const;
  size_t number_row_;
  size_t number_pixel_;
  std::vector<std::vector<float> > data_; ///< (row, pixel)
  std::vector<int> row_number_;
  std::map<int,size_t> row_index_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return vector of row numbers.
/////////////////////////////////////////////////////////////////////////////

inline const std::vector<int>&
MSPI::Shared::Rdqi::row_numbers() const
{
  return row_number_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the number of rows.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::Rdqi::number_row() const
{
  return number_row_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the number of pixels.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::Rdqi::number_pixel() const
{
  return number_pixel_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return data quality indicator for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline
const std::vector<float>& 
MSPI::Shared::Rdqi::data(int Row_number) const
{
  return data_[row_index(Row_number)];
}

//////////////////////////////////////////////////////////////////////////////
/// Private method returning row index associated with the given detector 
/// row number.
//////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::Rdqi::row_index(int Row_number) const
{
  std::map<int,size_t>::const_iterator f = row_index_.find(Row_number);
  if (f == row_index_.end()) {
    std::ostringstream m;
    m << "Rdqi: row number not available: " << Row_number;
    throw MSPI_EXCEPTIONm(m.str());
  }
  return f->second;
}

#endif
