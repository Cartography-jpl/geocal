#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/process/v2.hpp>
#include <boost/asio.hpp>
#include <sstream>
#include <vector>
#include <cstdio>
#define SWIG_MAPPER_NAMESPACE GeoCal
#include "geocal_exception.h"
#include "serialize_function.h"

namespace GeoCal {
//-----------------------------------------------------------------------
/// Run Command, optionally passing In to stdin and capturing stdout in
/// Out. If In is null, stdin is inherited, if Out is null stdout is
/// inherited.
///
/// The Command is split on whitespace into the executable and its
/// arguments, the executable is searched for on the path. Throws an
/// exception if the process can't be found or returns a non-zero exit
/// status.
///
/// This uses boost process v2 (available in boost 1.80 and later).
//-----------------------------------------------------------------------
inline void boost_pcall_run(const std::string& Command,
			    const std::string* In, std::string* Out)
{
  namespace bp = boost::process::v2;
  namespace asio = boost::asio;
  std::istringstream is(Command);
  std::vector<std::string> args;
  std::string a;
  while(is >> a)
    args.push_back(a);
  if(args.empty())
    throw Exception("Empty command passed to boost_pcall");
  auto exec = bp::environment::find_executable(args[0]);
  if(exec.empty()) {
    Exception e;
    e << "Could not find " << args[0] << " in boost_pcall";
    throw e;
  }
  args.erase(args.begin());
  asio::io_context ctx;
  asio::writable_pipe pin(ctx);
  asio::readable_pipe pout(ctx);
  // Note the process_stdio needs to be constructed in place with the pipes,
  // moving or assigning one closes the child end of the pipes.
  bp::process c = In ?
    (Out ? bp::process(ctx, exec, args, bp::process_stdio{pin, pout, {}}) :
     bp::process(ctx, exec, args, bp::process_stdio{pin, {}, {}})) :
    bp::process(ctx, exec, args, bp::process_stdio{{}, pout, {}});
  // We need to write stdin and read stdout at the same time. If we write
  // all of stdin first, the process can block writing to a full stdout
  // pipe while we are blocked writing to a full stdin pipe (a deadlock for
  // anything larger than the pipe buffer). So do these asynchronously.
  boost::system::error_code write_ec, read_ec;
  if(In)
    asio::async_write(pin, asio::buffer(*In),
		      [&](const boost::system::error_code& ec, std::size_t) {
			write_ec = ec;
			// Our various programs wait for stdin to have a EOF
			// before running (since data is binary, no other marker).
			// So explicitly close the pipe here.
			pin.close();
		      });
  if(Out)
    // This sucks up all the data the process writes out until it
    // finishes and stdout gets an EOF when it is closed.
    asio::async_read(pout, asio::dynamic_buffer(*Out),
		     [&](const boost::system::error_code& ec, std::size_t) {
		       read_ec = ec;
		     });
  ctx.run();
  if(write_ec)
    throw Exception("Trouble writing input in boost_pcall: " +
		    write_ec.message());
  if(read_ec && read_ec != asio::error::eof)
    throw Exception("Trouble reading output in boost_pcall: " +
		    read_ec.message());
  c.wait();
  if(c.exit_code() != 0) {
    Exception e;
    e << "Trouble calling " << Command << " in boost_pcall";
    throw e;
  }
}

//-----------------------------------------------------------------------
/// This is a simple process call, where we pass a boost serialized
/// object to a process and receive back a boost serialized
/// object. This is meant for short utility programs where something
/// is much easier to do in python than in C++, e.g., boost_nitf_rsm.
///
/// The Command can include arguments (e.g., boost_nitf_rsm to_nitf <fname>).
//-----------------------------------------------------------------------
template<class In, class Out>
boost::shared_ptr<Out> boost_pcall(const std::string& Command,
				   const boost::shared_ptr<In>& Din)
{
  std::string data = serialize_write_binary(Din);
  std::string res;
  boost_pcall_run(Command, &data, &res);
  return serialize_read_binary_string<Out>(res);
}

template<class Out>
boost::shared_ptr<Out> boost_pcall(const std::string& Command)
{
  std::string res;
  boost_pcall_run(Command, 0, &res);
  return serialize_read_binary_string<Out>(res);
}
  
template<class In>
void boost_pcall_noret(const std::string& Command,
		       const boost::shared_ptr<In>& Din)
{
  std::string data = serialize_write_binary(Din);
  boost_pcall_run(Command, &data, 0);
}
  
}
