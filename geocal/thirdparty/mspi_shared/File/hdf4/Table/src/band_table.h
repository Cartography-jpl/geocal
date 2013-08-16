//===========================================================================
//                                                                          
//                              BandTable                                   
//                                                                          
//===========================================================================
//
//             Copyright 2013, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef HDF4_BAND_TABLE_H
#define HDF4_BAND_TABLE_H

#include <map>			  // Definition of std::map
#include "hdf.h"		  // Definition of int32
#include <set>			  // Definition of std::set
#include <string>		  // Definition of std::string
#include <sstream>  		// Definition of std::ostringstream
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <vector>		// Definition of std::vector
#include <algorithm>  		// Definition of std::sort

namespace MSPI {
  namespace Shared {
    namespace HDF4 {
      class BandTable;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Provides access to band table in HDF file.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::HDF4::BandTable {
public:

  BandTable(int32 Fid);

  BandTable();
  
  inline std::vector<int> band_numbers() const;

  inline float wavelength(int Band_number) const;
    
  inline double E0(int Band_number) const;
    
  inline std::string grid_name(int Band_number) const;

  void add(int Band_number, float Wavelength, double E0, const std::string& Grid_name);

protected:

private:
  int32 fid_;

  static const int grid_name_maxlen_ = 32;
  typedef struct {
    int band_number;
    float wavelength;
    double e0;
    char grid_name[grid_name_maxlen_];
  } BandTableRecord;

  std::string table_name_;
  size_t type_size_;
  const char *field_name_[4];
  size_t field_offset_[4];
  int32 field_type_[4];
  size_t field_size_[4];
  std::string field_name_list_;

  inline const BandTableRecord& find(int Band_number) const;

  std::set<int> band_set_;
  std::map<int,BandTableRecord> table_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return record for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::HDF4::BandTable::BandTableRecord&
MSPI::Shared::HDF4::BandTable::find(int Band_number) const
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
MSPI::Shared::HDF4::BandTable::band_numbers() const
{
  std::vector<int> result(band_set_.begin(), band_set_.end());
  std::sort(result.begin(), result.end());
  return result;
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return wavelength for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::HDF4::BandTable::wavelength(int Band_number) const
{
  return find(Band_number).wavelength;
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return solar irradiance (E0) for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::HDF4::BandTable::E0(int Band_number) const
{
  return find(Band_number).e0;
}
    
/////////////////////////////////////////////////////////////////////////////
/// Return grid name for the given band number.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::HDF4::BandTable::grid_name(int Band_number) const
{

  const char *grid_name = find(Band_number).grid_name;
  return std::string(grid_name,strlen(grid_name));
}

#endif
