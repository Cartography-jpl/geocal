/*===========================================================================
=                                                                           =
=                               FieldMap                                    =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!END=======================================================================*/

#ifndef FIELD_MAP_H
#define FIELD_MAP_H

#include <vector> 		  // Definition of std::vector
#include <string>                 // Definition of std::string
#include <map>                    // Definition of std::map
#include <set>                    // Definition of std::set
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream> 		// Definition of std::ostringstream
#include <boost/any.hpp> 	// Definition of boost::any

namespace MSPI {
  namespace Shared {
    class FieldMap;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Base class to define a mapping from integer grid number to a set of
/// field ames.
///
/// NOTE: In the MSPI L1B1 file context, a "grid number" corresponds to 
/// a CCD detector row number.  In the MSPI L1B2 file context, a "grid number"
/// corresponds to an arbitrary integer band number assigned to each wavelength.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::FieldMap {
public:
  FieldMap();
  std::vector<std::string> field_names(int Grid_number) const;
  std::vector<std::string> field_names(const std::string& Grid_name) const;

  std::string field_name(int Grid_number, size_t Field_index) const;
  std::string field_name(const std::string& Grid_name, size_t Field_index) const;

  std::string type(int Grid_number, size_t Field_index) const;
  std::string type(const std::string& Grid_name, size_t Field_index) const;

  template<typename T> T fill(int Grid_number, size_t Field_index) const;
  template<typename T> T fill(int Grid_number, const std::string& Field_name) const;

  template<typename T> T fill(const std::string& Grid_name, size_t Field_index) const;
  template<typename T> T fill(const std::string& Grid_name, const std::string& Field_name) const;

  std::vector<int> grid_numbers() const;
  std::vector<std::string> grid_names() const;

  std::string grid_name(int Grid_number) const;

  void init_grid(int Grid_number,const char *Grid_name, int Number_field, ...);
  void init_grid(int Grid_number,const std::string& Grid_name, 
		 const std::vector<std::string>& Field_names);
  void init_grid(int Grid_number,const std::string& Grid_name, 
		 const std::vector<std::string>& Field_names,
		 const std::vector<std::string>& Data_type,
		 const std::vector<boost::any>& Fill);
protected:
private:
  int grid_index(int Grid_number) const;
  int grid_index(const std::string& Grid_name) const;
  int field_index(int Grid_index, const std::string& Grid_name) const;
  std::map<int,int> grid_index_;
  std::map<std::string, int> grid_name_to_index_;
  std::vector<std::vector<std::string> > fields_;
  std::vector<std::vector<std::string> > type_;
  std::vector<std::vector<boost::any> > fill_;
  std::vector<int> grid_numbers_;
  std::vector<std::string> grid_name_;
  std::vector<std::map<std::string,int> > field_index_;
};

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

inline 
MSPI::Shared::FieldMap::FieldMap()
{
}

/////////////////////////////////////////////////////////////////////////////
/// Return field name for the given grid number and field index.
/// If there is no field defined for the given field index, throw an exception.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::FieldMap::field_name(int Grid_number, size_t Field_index) const
{
  std::vector<std::string> field_names1 = field_names(Grid_number);

//---------------------------------------------------------------------------
// Argument check: Field_index < 0
//                 Field_index >= field_names1.size()
//---------------------------------------------------------------------------

  if (Field_index < 0) {
    throw MSPI_EXCEPTIONm("Field index < 0");
  }

  if (Field_index >= field_names1.size()) {
    std::ostringstream message;
    message << "Field index " << Field_index << " not valid for grid number " 
	    << Grid_number;
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Return
//---------------------------------------------------------------------------

  return field_names1[Field_index];
}

/////////////////////////////////////////////////////////////////////////////
/// Return field name for the given grid name and field index.
/// If there is no field defined for the given field index, throw an exception.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::FieldMap::field_name(const std::string& Grid_name, size_t Field_index) const
{
  std::vector<std::string> field_names1 = field_names(Grid_name);

//---------------------------------------------------------------------------
// Argument check: Field_index < 0
//                 Field_index >= field_names1.size()
//---------------------------------------------------------------------------

  if (Field_index < 0) {
    throw MSPI_EXCEPTIONm("Field index < 0");
  }

  if (Field_index >= field_names1.size()) {
    std::ostringstream message;
    message << "Field index " << Field_index << " not valid for grid name " 
	    << Grid_name;
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Return
//---------------------------------------------------------------------------

  return field_names1[Field_index];
}

/////////////////////////////////////////////////////////////////////////////
/// Return type identifier for a field.   This can be any string.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::FieldMap::type(int Grid_number, size_t Field_index) const
{
  return type_[grid_index(Grid_number)][Field_index];
}

/////////////////////////////////////////////////////////////////////////////
/// Return type identifier for a field.   This can be any string.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::FieldMap::type(const std::string& Grid_name, size_t Field_index) const
{
  return type_[grid_index(Grid_name)][Field_index];
}

/////////////////////////////////////////////////////////////////////////////
/// Return fill value for a field.  Data type is specified by the template
/// argument.
/////////////////////////////////////////////////////////////////////////////

template<typename T>
inline T
MSPI::Shared::FieldMap::fill(int Grid_number, size_t Field_index) const
{
  return boost::any_cast<T>(fill_[grid_index(Grid_number)][Field_index]);
}

/////////////////////////////////////////////////////////////////////////////
/// Return fill value for a field.  Data type is specified by the template
/// argument.
/////////////////////////////////////////////////////////////////////////////

template<typename T>
inline T
MSPI::Shared::FieldMap::fill(int Grid_number, const std::string& Field_name) const
{
  int igrid = grid_index(Grid_number);
  int ifield = field_index(igrid, Field_name);
  return boost::any_cast<T>(fill_[igrid][ifield]);
}

/////////////////////////////////////////////////////////////////////////////
/// Return fill value for a field.  Data type is specified by the template
/// argument.
/////////////////////////////////////////////////////////////////////////////

template<typename T>
inline T
MSPI::Shared::FieldMap::fill(const std::string& Grid_name, size_t Field_index) const
{
  return boost::any_cast<T>(fill_[grid_index(Grid_name)][Field_index]);
}

/////////////////////////////////////////////////////////////////////////////
/// Return fill value for a field.  Data type is specified by the template
/// argument.
/////////////////////////////////////////////////////////////////////////////

template<typename T>
inline T
MSPI::Shared::FieldMap::fill(const std::string& Grid_name, const std::string& Field_name) const
{
  int igrid = grid_index(Grid_name);
  int ifield = field_index(igrid, Field_name);
  return boost::any_cast<T>(fill_[igrid][ifield]);
}

/////////////////////////////////////////////////////////////////////////////
/// Return vector of field names for the given grid number.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string>
MSPI::Shared::FieldMap::field_names(int Grid_number) const
{
  return fields_[grid_index(Grid_number)];
}

/////////////////////////////////////////////////////////////////////////////
/// Return vector of field names for the given grid number.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string>
MSPI::Shared::FieldMap::field_names(const std::string& Grid_name) const
{
  return fields_[grid_index(Grid_name)];
}

/////////////////////////////////////////////////////////////////////////////
/// Return vector of grid numbers.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<int>
MSPI::Shared::FieldMap::grid_numbers() const
{
  return grid_numbers_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return vector of grid names.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string>
MSPI::Shared::FieldMap::grid_names() const
{
  return grid_name_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return name for a given grid number.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::FieldMap::grid_name(int Grid_number) const
{
  return grid_name_[grid_index(Grid_number)];
}

//////////////////////////////////////////////////////////////////////////////
/// Private method translating grid_number to grid_index
//////////////////////////////////////////////////////////////////////////////

inline int
MSPI::Shared::FieldMap::grid_index(int Grid_number) const
{
  std::map<int,int>::const_iterator f = grid_index_.find(Grid_number);
  if (f == grid_index_.end()) {
    std::ostringstream m;
    m << "FieldMap : unexpected grid number: " << Grid_number;
    throw MSPI_EXCEPTIONm(m.str());
  }
  return f->second;
}

//////////////////////////////////////////////////////////////////////////////
/// Private method translating grid_name to grid_index
//////////////////////////////////////////////////////////////////////////////

inline int
MSPI::Shared::FieldMap::grid_index(const std::string& Grid_name) const
{
  std::map<std::string, int>::const_iterator f = grid_name_to_index_.find(Grid_name);
  if (f == grid_name_to_index_.end()) {
    std::ostringstream m;
    m << "FieldMap : unexpected grid name: " << Grid_name;
    throw MSPI_EXCEPTIONm(m.str());
  }
  return f->second;
}

//////////////////////////////////////////////////////////////////////////////
/// Private method translating grid index and field name to field index.
//////////////////////////////////////////////////////////////////////////////

inline int
MSPI::Shared::FieldMap::field_index(int Grid_index, const std::string& Field_name) const
{
  const std::map<std::string, int>& field_index = field_index_[Grid_index];

  std::map<std::string, int>::const_iterator f = field_index.find(Field_name);
  if (f == field_index.end()) {
    std::ostringstream m;
    m << "FieldMap : unexpected field name for grid index: " << Grid_index;
    throw MSPI_EXCEPTIONm(m.str());
  }
  return f->second;
}


#endif
