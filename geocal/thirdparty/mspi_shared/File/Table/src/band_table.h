//===========================================================================
//                                                                          
//                              BandTable                                   
//                                                                          
//===========================================================================
//
//             Copyright 2008, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef BAND_TABLE_H
#define BAND_TABLE_H

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
    class BandTable;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Provides access to band table in HDF file.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::BandTable {
public:

  BandTable(hid_t Group_id);

  BandTable();
  
  inline std::vector<int> band_numbers() const;

  inline float wavelength(int Band_number) const;

  inline double E0(int Band_number) const;
    
  inline std::string grid_name(int Band_number) const;

  void add(int Band_number, float Wavelength, double E0, const std::string& Grid_name);

protected:

private:
  static const int grid_name_maxlen_ = 32;
  typedef struct {
    int band_number;
    float wavelength;
    double e0;
    char grid_name[grid_name_maxlen_];
  } BandTableRecord;

  std::string table_name_;
  size_t type_size_;
  size_t field_offset_[4];
  hid_t field_type_[4];
  size_t field_size_[4];

  inline const BandTableRecord& find(int Band_number) const;

  hid_t group_id_;
  std::set<int> band_set_;
  std::map<int,BandTableRecord> table_;
};

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////

inline
MSPI::Shared::BandTable::BandTable()
  : table_name_("Band Table"),
    group_id_(-1)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Return record for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::BandTable::BandTableRecord&
MSPI::Shared::BandTable::find(int Band_number) const
{
  std::map<int,BandTableRecord>::const_iterator h = table_.find(Band_number);
  if (h == table_.end()) {
    std::ostringstream message;  
    message << "No record for band: " << Band_number;
    throw MSPI_EXCEPTIONm(message.str());
  }

  return h->second;

}
    
/////////////////////////////////////////////////////////////////////////////
/// Return sorted vector of band numbers.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<int>
MSPI::Shared::BandTable::band_numbers() const
{
  std::vector<int> result(band_set_.begin(), band_set_.end());
  std::sort(result.begin(), result.end());
  return result;
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return wavelength for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::BandTable::wavelength(int Band_number) const
{
  return find(Band_number).wavelength;
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return solar irradiance (E0) for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::BandTable::E0(int Band_number) const
{
  return find(Band_number).e0;
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return grid name for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::BandTable::grid_name(int Band_number) const
{
  const char *grid_name = find(Band_number).grid_name;
  return std::string(grid_name,strlen(grid_name));
}

#endif
