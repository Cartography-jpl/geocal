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
  namespace bp = boost::process;
  std::string data = serialize_write_binary(Din);
  bp::opstream pin;
  bp::ipstream pout;
  // bp::child is low level, so it doesn't search on the path. Determine
  // the location of the executable
  auto pos = Command.find(" ");
  std::string exec = bp::search_path(Command.substr(0,pos)).string();
  if(pos != std::string::npos)
    exec += Command.substr(pos);
  bp::child c(exec, bp::std_in < pin, bp::std_out > pout);
  pin << data;
  pin.flush();
  // Our various programs wait for stdin to have a EOF before running
  // (since data is binary, no other marker). So explicitly close the
  // pipe here.
  pin.pipe().close();
  std::stringstream d;
  // Bit obscure looking, but this is idiomatic in C++ for reading
  // all of a stream. This sucks up all the data the process writes
  // out until it finishes and stdout gets an EOF when it is closed.
  d << pout.rdbuf();
  c.wait();
  if(c.exit_code() != 0) {
    Exception e;
    e << "Trouble calling " << Command << " in boost_pcall";
    throw e;
  }
  return serialize_read_binary_string<Out>(d.str());
}

template<class Out>
boost::shared_ptr<Out> boost_pcall(const std::string& Command)
{
  namespace bp = boost::process;
  bp::ipstream pout;
  // bp::child is low level, so it doesn't search on the path. Determine
  // the location of the executable
  auto pos = Command.find(" ");
  std::string exec = bp::search_path(Command.substr(0,pos)).string();
  if(pos != std::string::npos)
    exec += Command.substr(pos);
  bp::child c(exec, bp::std_out > pout);
  std::stringstream d;
  // Bit obscure looking, but this is idiomatic in C++ for reading
  // all of a stream. This sucks up all the data the process writes
  // out until it finishes and stdout gets an EOF when it is closed.
  d << pout.rdbuf();
  c.wait();
  if(c.exit_code() != 0) {
    Exception e;
    e << "Trouble calling " << Command << " in boost_pcall";
    throw e;
  }
  return serialize_read_binary_string<Out>(d.str());
}
  
template<class In>
void boost_pcall_noret(const std::string& Command,
		       const boost::shared_ptr<In>& Din)
{
  namespace bp = boost::process;
  std::string data = serialize_write_binary(Din);
  bp::opstream pin;
  // bp::child is low level, so it doesn't search on the path. Determine
  // the location of the executable
  auto pos = Command.find(" ");
  std::string exec = bp::search_path(Command.substr(0,pos)).string();
  if(pos != std::string::npos)
    exec += Command.substr(pos);
  bp::child c(exec, bp::std_in < pin);
  pin << data;
  pin.flush();
  // Our various programs wait for stdin to have a EOF before running
  // (since data is binary, no other marker). So explicitly close the
  // pipe here.
  pin.pipe().close();
  c.wait();
  if(c.exit_code() != 0) {
    Exception e;
    e << "Trouble calling " << Command << " in boost_pcall";
    throw e;
  }
}
  
}
