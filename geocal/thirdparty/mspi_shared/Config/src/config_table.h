//===========================================================================
//                                                                          
//                                ConfigTable                               
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef CONFIG_TABLE_H
#define CONFIG_TABLE_H

#include <string>		// Definition of std::string
#include <map> 			// Definition of std::map
#include <sstream> 		// Definition of std::ostringstream
#include <vector>	        // Definition of std::vector
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <cstdlib> 		// Definition of atof
#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include "config_file.h"	  // Definition of ConfigFile

namespace MSPI {
  namespace Shared {
    class ConfigTable;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides access to a table of parameters contained in a config file.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::ConfigTable {
public:
  ConfigTable(const ConfigFile& Config, const std::string& Table_name);
  bool is_defined(const std::string& Column_name) const;
  size_t number_row() const;
  size_t number_column() const;
  template <class T> std::vector<T> get_values(const std::string& Column_name) const;
  template <class T> T get_cell(const std::string& Column_name, size_t Row_index) const;
  template <class T> std::vector<T> get(const std::string& Column_name, size_t Size) const;
protected:
private:
  bool atobool(const std::string& String) const;
  const std::vector<std::string>& get(const std::string& Column_name) const;
  int number_row_;
  int number_column_;
  std::vector<std::string> column_name_;
  std::map<std::string, std::vector<std::string> > value_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return the number of rows.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::ConfigTable::number_row() const
{
  return number_row_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the number of columns.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::ConfigTable::number_column() const
{
  return number_column_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return boolean indicating if the given column name is defined.
/////////////////////////////////////////////////////////////////////////////

inline bool
MSPI::Shared::ConfigTable::is_defined(const std::string& Column_name) const
{
  std::map<std::string, std::vector<std::string> >::const_iterator f = value_.find(Column_name);
  if (f == value_.end()) {
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////
/// Get scalar parameter.  Throw an exception if the parameter in the file
/// is not scalar.
///////////////////////////////////////////////////////////////////////////

template <class T> 
inline T 
MSPI::Shared::ConfigTable::get_cell(const std::string& Column_name, size_t Row_index) const
{
  std::vector<T> values = get_values<T>(Column_name);
  if (values.size() <= Row_index) {
    std::ostringstream message;
    message << "Row index "<<Row_index<<" out of range for '"<<Column_name<<"' with size "<<values.size();
    throw MSPI_EXCEPTIONm(message.str());
  }
  return values[Row_index];
}

///////////////////////////////////////////////////////////////////////////
/// Get vector parameter.  The expected size of the vector must be specified.
/// Throw an exception if the actual size of the vector in the file does
/// not match the expected size.
///////////////////////////////////////////////////////////////////////////

template <class T> 
inline std::vector<T> 
MSPI::Shared::ConfigTable::get(const std::string& Column_name, size_t Size) const
{
  std::vector<T> values = get_values<T>(Column_name);
  if (values.size() != Size) {
    std::ostringstream message;
    message << "Requested "<<Size<<" rows for column '"<<Column_name<<"' from a table with "<<values.size()<<" rows.";
    throw MSPI_EXCEPTIONm(message.str());
  }
  return values;
}

/////////////////////////////////////////////////////////////////////////////
/// Template specializations must be inside namespace scope.
/////////////////////////////////////////////////////////////////////////////

namespace MSPI {
  namespace Shared {

/////////////////////////////////////////////////////////////////////////////
/// Retrieve vector of double values paired with the given name.  Throw an 
/// exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

    template <> inline std::vector<double>
    ConfigTable::get_values<double>(const std::string& Column_name) const
    {
      std::vector<std::string> values = get(Column_name);
      std::vector<double> double_values(values.size());
      for (size_t i = 0 ; i < values.size() ; i++) {
	double_values[i] = atof(values[i].c_str());
      }
      return double_values;
    }
    
/////////////////////////////////////////////////////////////////////////////
/// Retrieve vector of float values paired with the given name.  Throw an 
/// exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

    template <> inline std::vector<float>
    ConfigTable::get_values<float>(const std::string& Column_name) const
    {
      std::vector<std::string> values = get(Column_name);
      std::vector<float> float_values(values.size());
      for (size_t i = 0 ; i < values.size() ; i++) {
	float_values[i] = atof(values[i].c_str());
      }
      return float_values;
    }

/////////////////////////////////////////////////////////////////////////////
/// Retrieve vector of integer values paired with the given name.  Throw an 
/// exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

    template <> inline std::vector<int>
    ConfigTable::get_values<int>(const std::string& Column_name) const
    {
      std::vector<std::string> values = get(Column_name);
      std::vector<int> int_values(values.size());
      for (size_t i = 0 ; i < values.size() ; i++) {
	int_values[i] = atoi(values[i].c_str());
      }
      return int_values;
    }

/////////////////////////////////////////////////////////////////////////////
/// Retrieve vector of integer values paired with the given name.  Throw an 
/// exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

    template <> inline std::vector<unsigned short>
    ConfigTable::get_values<unsigned short>(const std::string& Column_name) const
    {
      std::vector<std::string> values = get(Column_name);
      std::vector<unsigned short> short_values(values.size());
      for (size_t i = 0 ; i < values.size() ; i++) {
	short_values[i] = atoi(values[i].c_str());
      }
      return short_values;
    }

/////////////////////////////////////////////////////////////////////////////
/// Retrieve vector of boolean values paired with the given name.  Throw an 
/// exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

    template <> inline std::vector<bool>
    ConfigTable::get_values<bool>(const std::string& Column_name) const
    {
      std::vector<std::string> values = get(Column_name);
      std::vector<bool> bool_values(values.size());
      for (size_t i = 0 ; i < values.size() ; i++) {
	bool_values[i] = atobool(values[i].c_str());
      }
      return bool_values;
    }

/////////////////////////////////////////////////////////////////////////////
/// Retrieve vector of string values paired with the given name.  Throw an 
/// exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

    template <> inline std::vector<std::string>
    ConfigTable::get_values<std::string>(const std::string& Column_name) const
    {
      return get(Column_name);
    }
  }
} // End namespace MSPI::Shared for template specializations.

#endif
