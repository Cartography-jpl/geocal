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
  if((int) res.size() != sz)
    throw Exception("Formatted string is not the expected size");
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

}
#endif
