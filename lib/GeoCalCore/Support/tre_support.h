#ifndef TRE_SUPPORT_H
#define TRE_SUPPORT_H
#include "geocal_exception.h"
#include <boost/format.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/optional.hpp>
#include <istream>
#include <string>

namespace GeoCal {
  // We do most of our TRE code in python (see python/lib/nitf),
  // however there is a small amount that make more sense to do in our
  // C++ code. I don't have a good sense yet at what exactly we need
  // to support this, we certainly don't want to duplicate all the
  // complicated logic we have in python. So for now, this is just an
  // accumulation of routines we use in a few different places.

//-----------------------------------------------------------------------
// Format and check size. 
//-----------------------------------------------------------------------

inline std::string str_check_size(const boost::basic_format<char>& F, int sz)
{
  std::string res = boost::str(F);
  if((int) res.size() != sz) {
    Exception e;
    e << "Formatted string is not the expected size.\n"
      << "Expected size: " << sz << "\n"
      << "Actual size:   " << res.size() << "\n"
      << "String:       '" << res << "'\n";
    throw e;
  }
  return res;
}

//-----------------------------------------------------------------------
// Read a fixed size string, and convert to given type. It is unclear
// if having extra spaces if for example a double is valid or not, but
// we do have some files with this property. If requested, we can trim
// spaces.
//-----------------------------------------------------------------------

template<class T> inline T read_size(std::istream& In, int size,
				     bool trim_space = false)
{
  char buf[1000];
  In.read(buf, size);
  if(In.gcount() != size)
    throw Exception("Not enough characters to read");
  if(!trim_space)
    return boost::lexical_cast<T>(buf, size);
  std::string t(buf,size);
  boost::algorithm::trim(t);
  return boost::lexical_cast<T>(t);
}

template<> inline boost::optional<double> read_size<boost::optional<double> > (std::istream& In, int size, bool trim_space)
{
  char buf[1000];
  In.read(buf, size);
  if(In.gcount() != size)
    throw Exception("Not enough characters to read");
  for(int i = 0; i < size; ++i)
    if(buf[i] != ' ') {
      if(!trim_space)
	return boost::lexical_cast<double>(buf, size);
      else {
	std::string t(buf,size);
	boost::algorithm::trim(t);
	return boost::lexical_cast<double>(t);
      }
    }
  return boost::optional<double>();
}

template<> inline boost::optional<int> read_size<boost::optional<int> >(std::istream& In, int size, bool trim_space)
{
  char buf[1000];
  In.read(buf, size);
  if(In.gcount() != size)
    throw Exception("Not enough characters to read");
  for(int i = 0; i < size; ++i)
    if(buf[i] != ' ') {
      if(!trim_space)
	return boost::lexical_cast<int>(buf, size);
      else {
	std::string t(buf,size);
	boost::algorithm::trim(t);
	return boost::lexical_cast<int>(t);
      }
    }
  return boost::optional<int>();
}
  

// We use the convention that a missing number (so all spaces) is
// returned as NaN, which we use as a holder for missing data
inline double read_size_nan(std::istream& In, int size, bool trim_space=false)
{
  char buf[1000];
  In.read(buf, size);
  if(In.gcount() != size)
    throw Exception("Not enough characters to read");
  for(int i = 0; i < size; ++i)
    if(buf[i] != ' ') {
      if(!trim_space)
	return boost::lexical_cast<double>(buf, size);
      else {
	std::string t(buf,size);
	boost::algorithm::trim(t);
	return boost::lexical_cast<double>(t);
      }
    }
  return std::numeric_limits<double>::quiet_NaN();
}

// We use the convention that a missing number (so all spaces) is
// returned as fill_value, which we use as a holder for missing data
inline int read_size_fill(std::istream& In, int size, int fill_value = -9999, bool trim_space=false)
{
  char buf[1000];
  In.read(buf, size);
  if(In.gcount() != size)
    throw Exception("Not enough characters to read");
  for(int i = 0; i < size; ++i)
    if(buf[i] != ' ') {
      if(!trim_space)
	return boost::lexical_cast<double>(buf, size);
      else {
	std::string t(buf,size);
	boost::algorithm::trim(t);
	return boost::lexical_cast<double>(t);
      }
    }
  return fill_value;
}
  
//-----------------------------------------------------------------------
/// Check end of stream
//-----------------------------------------------------------------------
inline void check_end_of_stream(std::istream& in)
{
  // Try to read more data. This should actually read 0 bytes, but we
  // can't do the "obvious" thing of in.eof() because eof is only set
  // once we try to read past the end of stream
  char buf[101];
  in.read(buf, 100);
  if(in.gcount() > 0) {
    Exception e;
    buf[100] = '\0'; // Make sure we end somewhere.
    e << "There is unexpected stuff at the end of the TRE string\n"
      << "Stuff: '" << buf << "'\n";
    throw e;
  }
}

template<class T> inline std::string write_optional
(boost::format& F, const boost::optional<T> & V, int Str_length)
{
  if(V)
    return str_check_size(F % *V, Str_length);
  std::string res;
  for(int i = 0; i < Str_length ; ++i)
    res += " ";
  return res;
}
}
#endif
