#ifndef MSPI_CONFIG_FILE_H
#define MSPI_CONFIG_FILE_H
#include "printable.h"
#include <boost/lexical_cast.hpp>
#include <string>
#include <map>
#include <boost/regex.hpp>
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
  MSPI uses ASCII configuration files.

  The files can contain comments, which start with a "#" and go to
  the end of the line.

  Everything else should be in a form like:
  value_1 value_2 ... : keyword_name

  It is perfectly ok for there to be multiple lines before the 
  variable name, so something like:

  value_1 value_2
  value_3 value_4 : keyword_name

  Anything at the end of the file is ignored.
*******************************************************************/

class MspiConfigFile: public Printable<MspiConfigFile>  {
public:
  explicit MspiConfigFile(const std::string& Fname);
  virtual ~MspiConfigFile() {}

//-----------------------------------------------------------------------
/// File name for MspiConfigFile.
//-----------------------------------------------------------------------

  const std::string& file_name() const {return fname;}

//-----------------------------------------------------------------------
/// Return the value for the given keyword, lexical_cast to the given
/// type.
//-----------------------------------------------------------------------
  template<class T> T value(const std::string& Keyword) const
  {
    return boost::lexical_cast<T>(value_string(Keyword));
  }
  virtual void print(std::ostream& Os) const;
private:
  const std::string& value_string(const std::string& Keyword) const;
  std::map<std::string, std::string> key_to_value;
  std::string fname;
};


//-----------------------------------------------------------------------
/// Return value for given keyword
//-----------------------------------------------------------------------

template<> inline 
std::vector<double> MspiConfigFile::value(const std::string& Keyword) const
{
  std::string v = value_string(Keyword);
  std::vector<double> res;
  boost::sregex_token_iterator i(v.begin(), v.end(), boost::regex("\\s+"), 
				 -1);
  boost::sregex_token_iterator iend;
  if(i == iend || *i == "")		// Handle special case of an empty list
    return res;
  for(; i != iend; ++i)
    res.push_back(boost::lexical_cast<double>(*i));
  return res;
}

//-----------------------------------------------------------------------
/// Return value for given keyword
//-----------------------------------------------------------------------

template<> inline 
std::vector<int> MspiConfigFile::value(const std::string& Keyword) const
{
  std::string v = value_string(Keyword);
  std::vector<int> res;
  boost::sregex_token_iterator i(v.begin(), v.end(), boost::regex("\\s+"), 
				 -1);
  boost::sregex_token_iterator iend;
  if(i == iend || *i == "")		// Handle special case of an empty list
    return res;
  for(; i != iend; ++i)
    res.push_back(boost::lexical_cast<int>(*i));
  return res;
}

//-----------------------------------------------------------------------
/// Return value for given keyword
//-----------------------------------------------------------------------

template<> inline 
std::vector<std::string> MspiConfigFile::value(const std::string& Keyword) const
{
  std::string v = value_string(Keyword);
  std::vector<std::string> res;
  boost::sregex_token_iterator i(v.begin(), v.end(), boost::regex("\\s+"), 
				 -1);
  boost::sregex_token_iterator iend;
  if(i == iend || *i == "")		// Handle special case of an empty list
    return res;
  for(; i != iend; ++i)
    res.push_back(*i);
  return res;
}

//-----------------------------------------------------------------------
/// Return value for given keyword
//-----------------------------------------------------------------------

template<> inline 
blitz::Array<double,1> MspiConfigFile::value(const std::string& Keyword) const
{
  std::vector<double> t = value<std::vector<double> >(Keyword);
  blitz::Array<double, 1> res(t.size());
  std::copy(t.begin(), t.end(), res.begin());
  return res;
}

//-----------------------------------------------------------------------
/// Return value for given keyword
//-----------------------------------------------------------------------

template<> inline 
blitz::Array<int,1> MspiConfigFile::value(const std::string& Keyword) const
{
  std::vector<int> t = value<std::vector<int> >(Keyword);
  blitz::Array<int, 1> res(t.size());
  std::copy(t.begin(), t.end(), res.begin());
  return res;
}

}
#endif
