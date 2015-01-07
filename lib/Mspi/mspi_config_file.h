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

  template<class T> T value(const std::string& Keyword) const;

//-----------------------------------------------------------------------
/// True if we have a value for the given keyword.
//-----------------------------------------------------------------------
  bool have_key(const std::string& K) const
  { return key_to_value.count(K) != 0; }
  virtual void print(std::ostream& Os) const;

  void add_file(const std::string& Fname);
private:
  const std::string& value_string(const std::string& Keyword) const;
  std::map<std::string, std::string> key_to_value;
};


/****************************************************************//**
  Small adapter that lets you refer to a table by column name. The
  convention is that the table "FOO" has a config entry of FOO.columns
  giving the column names, and FOO.table giving the actual values. We 
  look of a value by doing "value<Type>(index, column_name)".
*******************************************************************/

class MspiConfigTable: public Printable<MspiConfigTable>  {
public:
  MspiConfigTable(const MspiConfigFile& Config, const std::string& Table_name);
  virtual ~MspiConfigTable() {}
  virtual void print(std::ostream& Os) const
  { Os << "MspiConfigTable"; }
  int number_row() const { return data.rows(); }
  bool has_column(const std::string& Column) const
  {
    return column_to_index_.count(Column) != 0;
  }
  template<class T> T value(int Index, const std::string& Column) const;
  template<class T> std::vector<T> value_column(const std::string& Column) 
    const
  {
    std::vector<T> res;
    for(int i = 0; i < number_row(); ++i)
      res.push_back(value<T>(i, Column));
    return res;
  }
private:
  std::map<std::string, int> column_to_index_;
  int column_to_index(const std::string& Column) const
  {
    if(!has_column(Column))
      throw Exception(Column + " is not found in table");
    return column_to_index_.find(Column)->second;
  }
  blitz::Array<std::string, 2> data;
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
    // Default is to use lexical_cast
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
      << "  Error:\n"
      << eoriginal.what() << "\n";
    throw e;
  }
}

//-----------------------------------------------------------------------
/// Return value for index and column
//-----------------------------------------------------------------------

template<class T> inline 
T MspiConfigTable::value(int Index, const std::string& Column) const
{
  try {
    range_check(Index, 0, number_row());
    // Forward to helper class
    return MspiConfigFilePartialHelper<T>().parse_string(data(Index, column_to_index(Column)));
  } catch(std::exception& eoriginal) {
    Exception e;
    e << "Error occurred while trying to parse a table in MspiConfigTable.\n"
      << "  Index:  " << Index << "\n"
      << "  Column: " << Column << "\n"
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
    boost::sregex_token_iterator i(S.begin(), S.end(), boost::regex("\\n+\\s*"), 
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
