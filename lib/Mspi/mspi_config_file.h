#ifndef MSPI_CONFIG_FILE_H
#define MSPI_CONFIG_FILE_H
#include "geocal_exception.h"
#include "printable.h"
#include <iterator> 
#include <boost/lexical_cast.hpp>
#include <string>
#include <map>
#include <cstdlib>
#include <boost/regex.hpp>
#include <blitz/array.h>
#include <boost/algorithm/string.hpp>

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

//-----------------------------------------------------------------------
/// True if we have a value for the given keyword.
//-----------------------------------------------------------------------
  bool have_key(const std::string& K) const
  { return key_to_value.count(K) != 0; }
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
/// Specialization for bool.
//-----------------------------------------------------------------------

template <> class MspiConfigFilePartialHelper<bool> {
public:
  bool parse_string(const std::string& S) const
  {
    std::string scopy(S);
    boost::to_lower(scopy);
    if(scopy =="t" || 
       scopy =="true" ||
       scopy ==".true.")
      return true;
    if(scopy =="f" || 
       scopy =="false" ||
       scopy ==".false.")
      return false;
    throw Exception("Can't convert to boolean.");
  }
};

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

//-----------------------------------------------------------------------
/// Specialization for vectors of vectors.
//-----------------------------------------------------------------------

template <class T> class MspiConfigFilePartialHelper<std::vector<std::vector<T> > > {
public:
  std::vector<std::vector<T> > parse_string(const std::string& S) const
  {
    std::vector<std::vector<T> > res;
    boost::sregex_token_iterator i(S.begin(), S.end(), boost::regex("\\n+"), 
				   -1);
    boost::sregex_token_iterator iend;
    if(i == iend || *i == "")  // Handle special case of an empty list
      return res;
    for(; i != iend; ++i) {
      std::vector<T> ln = MspiConfigFilePartialHelper<std::vector<T> >().parse_string(*i);
      // If ln is empty, don't add it. This allows for things like
      // comment lines in the middle of an array.
      if(ln.size() > 0)
	res.push_back(ln);
    }
    return res;
  }
};


//-----------------------------------------------------------------------
/// Specialization for 2d blitz Array
//-----------------------------------------------------------------------

template <class T> class MspiConfigFilePartialHelper<blitz::Array<T, 2> > {
public:
  blitz::Array<T, 2> parse_string(const std::string& S) const
  {
    // Leverage off of vector of vectors. This lets us defer the check
    // on every line being the same size.
    std::vector<std::vector<T> > v = 
      MspiConfigFilePartialHelper<std::vector<std::vector<T> > >().parse_string(S);
    int nrow = (int) v.size();
    if(nrow == 0)
      throw Exception("Zero sized array isn't support by blitz");
    int ncol = (int) v[0].size();
    blitz::Array<T, 2> res(nrow, ncol);
    for(int i = 0; i < res.rows(); ++i) {
      if((int) v[i].size() != res.cols())
	throw Exception("Uneven size array isn't support by blitz");
      for(int j = 0; j < res.cols(); ++j)
	res(i, j) = v[i][j];
    }
    return res;
  }
};
    
}
#endif
