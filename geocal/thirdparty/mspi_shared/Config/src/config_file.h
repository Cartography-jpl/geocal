/*===========================================================================
=                                                                           =
=                                 ConfigFile                                =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                  Config File

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: ConfigFile

=============================================================================
!END=======================================================================*/

#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <string>		// Definition of std::string
#include <map> 			// Definition of std::map
#include <sstream> 		// Definition of std::ostringstream
#include <vector>	        // Definition of std::vector
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <cstdlib> 		// Definition of atof
#include "boost/multi_array.hpp"  // Definition of boost::multi_array

namespace MSPI {
  namespace Shared {
    class ConfigFile;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides configuration parameters, input from an external file.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::ConfigFile {
public:
  explicit ConfigFile(const std::string& Filename);
  void add_replace(const std::string& Filename);
  bool is_defined(const std::string& Name) const;
  size_t size(const std::string& Name) const;
  template <class T> std::vector<T> get_values(const std::string& Name) const;
  template <class T> T get(const std::string& Name) const;
  template <class T> std::vector<T> get(const std::string& Name, size_t Size) const;
  template <class T> boost::multi_array<T,2> 
    get(const std::string& Name, 
	size_t Number_row, size_t Number_column) const;
protected:
private:
  std::string filename_;
  bool atobool(const std::string& String) const;
  const std::vector<std::string>& get(const std::string& Name) const;

  std::map<std::string, std::vector<std::string> > 
    parse(const std::string& Filename);

  std::map<std::string, std::vector<std::string> > value_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return boolean indicating if the given name is defined.
/////////////////////////////////////////////////////////////////////////////

inline bool
MSPI::Shared::ConfigFile::is_defined(const std::string& Name) const
{
  std::map<std::string, std::vector<std::string> >::const_iterator f = value_.find(Name);
  if (f == value_.end()) {
    return false;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////
/// Return number of elements associated with the given name.
/// Throw an exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::ConfigFile::size(const std::string& Name) const
{
  return get(Name).size();
}


///////////////////////////////////////////////////////////////////////////
/// Get scalar parameter.  Throw an exception if the parameter in the file
/// is not scalar.
///////////////////////////////////////////////////////////////////////////

template <class T> 
inline T 
MSPI::Shared::ConfigFile::get(const std::string& Name) const
{
  std::vector<T> values = get_values<T>(Name);
  if (values.size() != 1) {
    std::ostringstream message;
    message << "Parameter '"<<Name<<"' with size "<<values.size()<<" does not match requested size of 1";
    throw MSPI_EXCEPTIONm(message.str());
  }
  return values[0];
}

///////////////////////////////////////////////////////////////////////////
/// Get vector parameter.  The expected size of the vector must be specified.
/// Throw an exception if the actual size of the vector in the file does
/// not match the expected size.
///////////////////////////////////////////////////////////////////////////

template <class T> 
inline std::vector<T> 
MSPI::Shared::ConfigFile::get(const std::string& Name, size_t Size) const
{
  std::vector<T> values = get_values<T>(Name);
  if (values.size() != Size) {
    std::ostringstream message;
    message << "Parameter '"<<Name<<"' with size "<<values.size()<<" does not match requested size of "<<Size;
    throw MSPI_EXCEPTIONm(message.str());
  }
  return values;
}

///////////////////////////////////////////////////////////////////////////
/// Get 2-dimensional array parameter.  Throw an exception if there are 
/// insufficient elements to construct the array.
///////////////////////////////////////////////////////////////////////////

template <class T> 
inline boost::multi_array<T,2> 
MSPI::Shared::ConfigFile::get(
  const std::string& Name, 
  size_t Number_row,
  size_t Number_column
) const
{
  std::vector<T> value = get<T>(Name, Number_row * Number_column);
  boost::const_multi_array_ref<T,2> 
    array(&value[0], boost::extents[Number_row][Number_column]);
  return array;
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
    ConfigFile::get_values<double>(const std::string& Name) const
    {
      std::vector<std::string> values = get(Name);
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
    ConfigFile::get_values<float>(const std::string& Name) const
    {
      std::vector<std::string> values = get(Name);
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
    ConfigFile::get_values<int>(const std::string& Name) const
    {
      std::vector<std::string> values = get(Name);
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

    template <> inline std::vector<signed long long>
    ConfigFile::get_values<signed long long>(const std::string& Name) const
    {
      std::vector<std::string> values = get(Name);
      std::vector<signed long long> result(values.size());
      for (size_t i = 0 ; i < values.size() ; i++) {
	result[i] = strtoull(values[i].c_str(),NULL,10);
      }
      return result;
    }

/////////////////////////////////////////////////////////////////////////////
/// Retrieve vector of boolean values paired with the given name.  Throw an 
/// exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

    template <> inline std::vector<bool>
    ConfigFile::get_values<bool>(const std::string& Name) const
    {
      std::vector<std::string> values = get(Name);
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
    ConfigFile::get_values<std::string>(const std::string& Name) const
    {
      return get(Name);
    }
  }
} // End namespace MSPI::Shared for template specializations.

#endif
