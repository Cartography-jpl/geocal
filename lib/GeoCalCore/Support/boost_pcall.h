#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/process.hpp>
#include <cstdio>
#define SWIG_MAPPER_NAMESPACE GeoCal
#include "serialize_function.h"

namespace GeoCal {
//-----------------------------------------------------------------------
/// This is a simple process call, where we pass a boost serialized
/// object to a process and receive back a boost serialized
/// object. This is meant for short utility programs where something
/// is much easier to do in python than in C++, e.g., boost_nitf_rsm.
///
/// The Command is directly passed to the shell, so this can include
/// arguments (e.g., boost_nitf_rsm to_nitf <fname>).
//-----------------------------------------------------------------------
template<class In, class Out>
boost::shared_ptr<Out> boost_pcall(const std::string& Command,
				   const boost::shared_ptr<In>& Din)
{
}

template<class Out>
boost::shared_ptr<Out> boost_pcall(const std::string& Command)
{
  FILE* f = popen(Command.c_str(), "r");
  if(!f) {
    Exception e;
    e << "Trouble calling " << Command << " in boost_pcall";
    throw e;
  }
  std::string data;
  data.reserve(1000);		// We'll need some space, so go ahead
				// at set it aside.
  int c = fgetc(f);
  while(!feof(f)) {
    data.push_back((char) c);
    c = getc(f);
  }
  int status = pclose(f);
  if(status) {
    Exception e;
    e << "Trouble calling " << Command << " in boost_pcall";
    throw e;
  }
  return serialize_read_binary_string<Out>(data);
}
  
template<class In>
void boost_pcall_noret(const std::string& Command,
		       const boost::shared_ptr<In>& Din)
{
  std::string data = serialize_write_binary(Din);
  FILE* f = popen(Command.c_str(), "w");
  if(!f) {
    Exception e;
    e << "Trouble calling " << Command << " in boost_pcall";
    throw e;
  }
  fwrite(data.c_str(), sizeof(char), data.size(), f);
  int status = pclose(f);
  if(status) {
    Exception e;
    e << "Trouble calling " << Command << " in boost_pcall";
    throw e;
  }
}
  
}
