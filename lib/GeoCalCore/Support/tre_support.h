#ifndef TRE_SUPPORT_H
#define TRE_SUPPORT_H
#include "geocal_exception.h"
#include <boost/format.hpp> 
#include <boost/lexical_cast.hpp>
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
// Read a fixed size string, and convert to given type.
//-----------------------------------------------------------------------

template<class T> inline T read_size(std::istream& In, int size)
{
  char buf[1000];
  In.read(buf, size);
  if(In.gcount() != size)
    throw Exception("Not enough characters to read");
  return boost::lexical_cast<T>(buf, size);
}

// We use the convention that a missing number (so all spaces) is
// returned as NaN, which we use as a holder for missing data
inline double read_size_nan(std::istream& In, int size)
{
  char buf[1000];
  In.read(buf, size);
  if(In.gcount() != size)
    throw Exception("Not enough characters to read");
  for(int i = 0; i < size; ++i)
    if(buf[i] != ' ')
      return boost::lexical_cast<double>(buf, size);
  return std::numeric_limits<double>::quiet_NaN();
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
}
#endif
