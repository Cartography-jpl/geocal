#ifndef MSPI_CONFIG_FILE_H
#define MSPI_CONFIG_FILE_H
#include "geocal_exception.h"
#include "printable.h"
#include <iterator> 
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

  template<class T> T value(const std::string& Keyword) const;
  virtual void print(std::ostream& Os) const;
private:
  const std::string& value_string(const std::string& Keyword) const;
  std::map<std::string, std::string> key_to_value;
  std::string fname;
};


/****************************************************************//**
  C++ doesn't allow function partial specialization. We introduce a 
  class as a way around this, since we can do partial specialization
  on a class.
*******************************************************************/

template <class T> class MspiConfigFilePartialHelper {
public:
  T parse_string(const std::string& S) const
  {
    // Default is to sue lexical_cast
    return boost::lexical_cast<T>(S);
  }
};


//-----------------------------------------------------------------------
/// Return value for given keyword
//-----------------------------------------------------------------------

template<class T> inline 
T MspiConfigFile::value(const std::string& Keyword) const
{
  try {
    // Forward to helper class
    return MspiConfigFilePartialHelper<T>().parse_string(value_string(Keyword));
  } catch(std::exception& eoriginal) {
    Exception e;
    e << "Error occurred while trying to parse a keyword in MspiConfigFile.\n"
      << "  Keyword: " << Keyword << "\n"
      << "  File:    " << file_name() << "\n"
      << "  Error:\n"
      << eoriginal.what() << "\n";
    throw e;
  }
}

//-----------------------------------------------------------------------
/// Specialization for vectors.
//-----------------------------------------------------------------------

template <class T> class MspiConfigFilePartialHelper<std::vector<T> > {
public:
  std::vector<T> parse_string(const std::string& S) const
  {
    std::vector<T> res;
    boost::sregex_token_iterator i(S.begin(), S.end(), boost::regex("\\s+"), 
				   -1);
    boost::sregex_token_iterator iend;
    if(i == iend || *i == "")  // Handle special case of an empty list
      return res;
    for(; i != iend; ++i)
      res.push_back(MspiConfigFilePartialHelper<T>().parse_string(*i));
    return res;
  }
};

//-----------------------------------------------------------------------
/// Specialization for blitz Array.
//-----------------------------------------------------------------------

template <class T> class MspiConfigFilePartialHelper<blitz::Array<T, 1> > {
public:
  blitz::Array<T, 1> parse_string(const std::string& S) const
  {
    boost::sregex_token_iterator i(S.begin(), S.end(), boost::regex("\\s+"), 
				   -1);
    boost::sregex_token_iterator iend;
    blitz::Array<T, 1> res(std::distance(i, iend));
    if(i == iend || *i == "")  // Handle special case of an empty list
      return res;
    typename blitz::Array<T, 1>::iterator rit = res.begin();
    for(; i != iend; ++i, ++rit)
      *rit = MspiConfigFilePartialHelper<T>().parse_string(*i);
    return res;
  }
};

// //-----------------------------------------------------------------------
// /// Return value for given keyword
// //-----------------------------------------------------------------------

// template<> inline 
// std::vector<int> MspiConfigFile::value(const std::string& Keyword) const
// {
//   std::string v = value_string(Keyword);
//   std::vector<int> res;
//   boost::sregex_token_iterator i(v.begin(), v.end(), boost::regex("\\s+"), 
// 				 -1);
//   boost::sregex_token_iterator iend;
//   if(i == iend || *i == "")		// Handle special case of an empty list
//     return res;
//   for(; i != iend; ++i)
//     res.push_back(boost::lexical_cast<int>(*i));
//   return res;
// }

// //-----------------------------------------------------------------------
// /// Return value for given keyword
// //-----------------------------------------------------------------------

// template<> inline 
// std::vector<std::string> MspiConfigFile::value(const std::string& Keyword) const
// {
//   std::string v = value_string(Keyword);
//   std::vector<std::string> res;
//   boost::sregex_token_iterator i(v.begin(), v.end(), boost::regex("\\s+"), 
// 				 -1);
//   boost::sregex_token_iterator iend;
//   if(i == iend || *i == "")		// Handle special case of an empty list
//     return res;
//   for(; i != iend; ++i)
//     res.push_back(*i);
//   return res;
// }

// //-----------------------------------------------------------------------
// /// Return value for given keyword
// //-----------------------------------------------------------------------

// template<> inline 
// blitz::Array<double,1> MspiConfigFile::value(const std::string& Keyword) const
// {
//   std::vector<double> t = value<std::vector<double> >(Keyword);
//   blitz::Array<double, 1> res(t.size());
//   std::copy(t.begin(), t.end(), res.begin());
//   return res;
// }

// //-----------------------------------------------------------------------
// /// Return value for given keyword
// //-----------------------------------------------------------------------

// template<> inline 
// blitz::Array<int,1> MspiConfigFile::value(const std::string& Keyword) const
// {
//   std::vector<int> t = value<std::vector<int> >(Keyword);
//   blitz::Array<int, 1> res(t.size());
//   std::copy(t.begin(), t.end(), res.begin());
//   return res;
// }

}
#endif
