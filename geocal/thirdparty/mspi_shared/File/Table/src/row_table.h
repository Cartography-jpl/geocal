/*===========================================================================
=                                                                           =
=                                  RowTable                                 =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                    Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: RowTable

!ABSTRACT:

   File access interface for Row Table.

!DERIVED FROM:

!DESCRIPTION:

!PUBLIC MEMBERS:

   RowTable(hid_t Hdf5_loc_id)
     - Constructor.  Creates a new table if one doesn't already exist.
       Opens/creates a table named "Row Table" in the given group.

   ~RowTable()
     - Destructor.

   std::set<int> row_numbers() const
     - Return the set of row numbers in the current table.

   float wavelength(int Row) const
     - Return wavelength for the given row.
   
   float polarization_angle(int Row) const
     - Return polarization angle for a given row.

   std::string swath_name(int Row) const
     - Return swath name for the given row.

   void add(int Row_number, float Wavelength, float Polarization_angle, std::string& Swath_name)
     - Add a new row number to the table.  If a record already exists
       for the given row number, throw an exception.

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

   RowTableRecord& find(int Row_number) const
      - Retrieve record for the given row number.  If there is no record
        for the given row number, throw an exception. 

   hid_t loc_id_
      - HDF5 group id.

   std::set<int> row_numbers_
     - Contains row numbers.

   std::map<int,float> wavelength_
     - Contains wavelength for each row.
     
   std::map<int,float> polarization_angle_
     - Contains polarization for each row.
     
   std::map<int,std::string> swath_name_
     - Contains swath name for each row.

!ASSOCIATED FUNCTIONS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef ROW_TABLE_H
#define ROW_TABLE_H

#include <map>			  // Definition of std::map
#include "hdf5.h"		  // Definition of hid_t
#include <set>			  // Definition of std::set
#include <string>		  // Definition of std::string
#include <sstream>  		// Definition of std::ostringstream
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <cstring>		// Definition of strlen
#include <vector>		// Definition of std::vector
#include <algorithm>  		// Definition of std::sort

namespace MSPI {
  namespace Shared {
    class RowTable;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Provides access to row table in HDF file.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::RowTable {
public:

  RowTable(hid_t Hdf5_loc_id);

  RowTable();
  
  ~RowTable();

  inline std::vector<int> row_numbers() const;

  inline float wavelength(int Row_number) const;

  inline float polarization_angle(int Row_number) const;
    
  inline std::string swath_name(int Row_number) const;

  void add(int Row_number, float Wavelength, float Polarization_angle, const std::string& Swath_name);

protected:

private:
  static const int swath_name_maxlen_ = 32;
  typedef struct {
    int row_number;
    float wavelength;
    float polarization_angle;
    char swath_name[swath_name_maxlen_];
  } RowTableRecord;

  std::string table_name_;
  size_t type_size_;
  size_t field_offset_[4];
  hid_t field_type_[4];
  size_t field_size_[4];

  inline const RowTableRecord& find(int Row_number) const;

  hid_t loc_id_;
  std::set<int> row_set_;
  std::map<int,RowTableRecord> table_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return record for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::RowTable::RowTableRecord&
MSPI::Shared::RowTable::find(int Row_number) const
{
  std::map<int,RowTableRecord>::const_iterator h = table_.find(Row_number);
  if (h == table_.end()) {
    std::ostringstream message;  
    message << "No record for row: " << Row_number;
    throw MSPI_EXCEPTIONm(message.str());
  }

  return h->second;

}
    
/////////////////////////////////////////////////////////////////////////////
/// Return sorted vector of row numbers.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<int>
MSPI::Shared::RowTable::row_numbers() const
{
  std::vector<int> result(row_set_.begin(), row_set_.end());
  std::sort(result.begin(), result.end());
  return result;
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return wavelength for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::RowTable::wavelength(int Row_number) const
{
  return find(Row_number).wavelength;
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return polarization angle for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::RowTable::polarization_angle(int Row_number) const
{
  return find(Row_number).polarization_angle;
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return swath name for the given row number.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::RowTable::swath_name(int Row_number) const
{

  const char *swath_name = find(Row_number).swath_name;
  return std::string(swath_name,strlen(swath_name));
}

#endif
