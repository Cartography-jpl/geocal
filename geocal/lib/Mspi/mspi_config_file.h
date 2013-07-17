#ifndef MSPI_CONFIG_FILE_H
#define MSPI_CONFIG_FILE_H
#include "printable.h"
#include "geocal_exception.h"
#include <map>
#include <vector>
#include <cstdlib>
#include <boost/multi_array.hpp>

namespace GeoCal {
/****************************************************************//**
  This is the configuration file reader used by MSPI. This gets
  keyword/value pairs from a text file.
*******************************************************************/

class MspiConfigFile : Printable<MspiConfigFile> {
public:
  explicit MspiConfigFile(const std::string& Filename);
  virtual ~MspiConfigFile() {}
  void add_replace(const std::string& Filename);

//-----------------------------------------------------------------------
/// Return boolean indicating if the given name is defined.
//-----------------------------------------------------------------------

  bool is_defined(const std::string& Name) const
  { return value_.find(Name) != value_.end(); }

//-----------------------------------------------------------------------
/// Returns number of elements associated with the given name.
//-----------------------------------------------------------------------

  size_t size(const std::string& Name) const
  { return get(Name).size();}


  template <class T> std::vector<T> get_values(const std::string& Name) const;
  template <class T> T get(const std::string& Name) const;
  template <class T> std::vector<T> get(const std::string& Name, size_t Size) 
    const;
  template <class T> boost::multi_array<T,2> 
    get(const std::string& Name, 
	size_t Number_row, size_t Number_column) const;
  virtual void print(std::ostream& Os) const
  { Os << "MspiConfigFile\n"
       << "  Filename: " << filename_ << "\n";
  }
private:
  std::string filename_;
  bool atobool(const std::string& String) const;
  const std::vector<std::string>& get(const std::string& Name) const;

  std::map<std::string, std::vector<std::string> > 
    parse(const std::string& Filename);

  std::map<std::string, std::vector<std::string> > value_;
};

//-----------------------------------------------------------------------
/// Get scalar parameter.  Throw an exception if the parameter in the file
/// is not scalar.
//-----------------------------------------------------------------------

template <class T> inline T 
MspiConfigFile::get(const std::string& Name) const
{
  std::vector<T> values = get_values<T>(Name);
  if (values.size() != 1) {
    Exception e;
    e << "Parameter '" << Name << "' with size " << values.size()
      <<" does not match requested size of 1";
    throw e;
  }
  return values[0];
}

//-----------------------------------------------------------------------
/// Get vector parameter.  The expected size of the vector must be specified.
/// Throw an exception if the actual size of the vector in the file does
/// not match the expected size.
//-----------------------------------------------------------------------

template <class T> 
inline std::vector<T> 
MspiConfigFile::get(const std::string& Name, size_t Size) const
{
  std::vector<T> values = get_values<T>(Name);
  if (values.size() != Size) {
    Exception e;
    e << "Parameter '" << Name << "' with size " << values.size()
      << " does not match requested size of " << Size;
    throw e;
  }
  return values;
}

//-----------------------------------------------------------------------
/// Get 2-dimensional array parameter.  Throw an exception if there are 
/// insufficient elements to construct the array.
//-----------------------------------------------------------------------

template <class T> 
inline boost::multi_array<T,2> 
MspiConfigFile::get
(const std::string& Name, 
 size_t Number_row,
 size_t Number_column
 ) const
{
  std::vector<T> value = get<T>(Name, Number_row * Number_column);
  boost::const_multi_array_ref<T,2> 
    array(&value[0], boost::extents[Number_row][Number_column]);
  return array;
}

//-----------------------------------------------------------------------
/// Retrieve vector of double values paired with the given name.  Throw an 
/// exception if the name is not defined.
//-----------------------------------------------------------------------

template <> inline std::vector<double>
MspiConfigFile::get_values<double>(const std::string& Name) const
{
  std::vector<std::string> values = get(Name);
  std::vector<double> double_values(values.size());
  for (size_t i = 0 ; i < values.size() ; i++)
    double_values[i] = atof(values[i].c_str());
  return double_values;
}
template <> inline std::vector<float>
MspiConfigFile::get_values<float>(const std::string& Name) const
{
  std::vector<std::string> values = get(Name);
  std::vector<float> float_values(values.size());
  for (size_t i = 0 ; i < values.size() ; i++)
    float_values[i] = atof(values[i].c_str());
  return float_values;
}

template <> inline std::vector<int>
MspiConfigFile::get_values<int>(const std::string& Name) const
{
  std::vector<std::string> values = get(Name);
  std::vector<int> int_values(values.size());
  for (size_t i = 0 ; i < values.size() ; i++)
    int_values[i] = atoi(values[i].c_str());
  return int_values;
}

template <> inline std::vector<signed long long>
MspiConfigFile::get_values<signed long long>(const std::string& Name) const
{
  std::vector<std::string> values = get(Name);
  std::vector<signed long long> result(values.size());
  for (size_t i = 0 ; i < values.size() ; i++)
    result[i] = strtoull(values[i].c_str(),NULL,10);
  return result;
}

template <> inline std::vector<bool>
MspiConfigFile::get_values<bool>(const std::string& Name) const
{
  std::vector<std::string> values = get(Name);
  std::vector<bool> bool_values(values.size());
  for (size_t i = 0 ; i < values.size() ; i++)
    bool_values[i] = atobool(values[i].c_str());
  return bool_values;
}

template <> inline std::vector<std::string>
MspiConfigFile::get_values<std::string>(const std::string& Name) const
{
  return get(Name);
}

}
#endif
