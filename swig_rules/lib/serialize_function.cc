#include <iostream>
#include "serialize_function.h"
#include "serialize_shared_ptr.h"
#include "weak_ptr_serialize_support.h"
#include <boost/regex.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <string>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
#include <boost/archive/polymorphic_xml_iarchive.hpp>
#include <boost/archive/polymorphic_xml_oarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>
#endif
using namespace SWIG_MAPPER_NAMESPACE;

/****************************************************************//**
   This allows filtering stream to be created on top of an 
   underlying ofstream. See boost::iostream documentation for details
   on how to create these. This class is intended to be used as a base
   class for other classes that assemble the proper filters.
*******************************************************************/

class FilteringOstream : public std::ostream {
public:
  virtual ~FilteringOstream() {}
protected:

//-----------------------------------------------------------------------
/// Constructor. This opens the given file and sets up the stream
/// buffer, but nothing is placed in the filter. The derived class should
/// set up the streambuf.
//-----------------------------------------------------------------------

  FilteringOstream(const std::string& Fname) 
    : std::ostream(&sb), f(Fname.c_str()) {}

//-----------------------------------------------------------------------
/// Underlying ofstream.
//-----------------------------------------------------------------------

  std::ofstream f;

//-----------------------------------------------------------------------
/// Filtering streambuf.
//-----------------------------------------------------------------------

  boost::iostreams::filtering_ostreambuf sb;
};

/****************************************************************//**
   This allows filtering stream to created on top of an 
   underlying ifstream. See boost::iostream documentation for details
   on how to create these. This class is intended to be used as a base
   class for other classes that assemble the proper filters.
*******************************************************************/

class FilteringIstream : public std::istream {
public:
  virtual ~FilteringIstream() {}
protected:

//-----------------------------------------------------------------------
/// Constructor. This opens the given file and sets up the stream
/// buffer, but nothing is placed in the filter. The derived class should
/// set up the streambuf.
//-----------------------------------------------------------------------

  FilteringIstream(const std::string& Fname) 
    : std::istream(&sb), f(Fname.c_str()) {}

//-----------------------------------------------------------------------
/// Underlying ifstream.
//-----------------------------------------------------------------------

  std::ifstream f;

//-----------------------------------------------------------------------
/// Filtering streambuf.
//-----------------------------------------------------------------------

  boost::iostreams::filtering_istreambuf sb;
};

/****************************************************************//**
/// This opens a file for writing, possibly with automatic
/// compression. We look at the File name, and if ends if ".gz" then
/// we automatically gzip the file.
*******************************************************************/

class OstreamCompress : public FilteringOstream {
public:
//-----------------------------------------------------------------------
/// This opens a file for writing, possibly with automatic
/// compression. We look at the File name, and if ends if ".gz" then
/// we automatically gzip the file.
//-----------------------------------------------------------------------
  OstreamCompress(const std::string& Fname)
    : FilteringOstream(Fname)
  {
    if(boost::regex_search(Fname, boost::regex("\\.gz$")))
      sb.push(boost::iostreams::gzip_compressor());
    sb.push(f);
  }    
  virtual ~OstreamCompress() {}
};

/****************************************************************//**
/// This opens a file for reading, possibly with automatic
/// decompression. We look at the File name, and if ends if ".gz" then
/// we automatically gunzip the file.
///
/// See all also IfstreamCs which both handles decompression and
/// stripping out comments.
*******************************************************************/

class IstreamCompress : public FilteringIstream {
public:
//-----------------------------------------------------------------------
/// This opens a file for reading, possibly with automatic
/// decompression. We look at the File name, and if ends if ".gz" then
/// we automatically gunzip the file.
///
/// See all also IfstreamCs which both handles decompression and
/// stripping out comments.
//-----------------------------------------------------------------------
  IstreamCompress(const std::string& Fname)
    : FilteringIstream(Fname)
  {
    if(boost::regex_search(Fname, boost::regex("\\.gz$")))
      sb.push(boost::iostreams::gzip_decompressor());
    sb.push(f);
  }
  virtual ~IstreamCompress() {}
};

//-----------------------------------------------------------------------
/// Utility class. This changes to a new directory, and on destruction
/// changes back.
//-----------------------------------------------------------------------

class DirChange {
public:
  DirChange(const std::string& newdir)
  {
    dirhandle = open(".", O_RDONLY);
    if(dirhandle == -1)
      throw std::runtime_error("Open failed");
    int status = chdir(newdir.c_str());
    if(status != 0)
      throw std::runtime_error("Could not change to directory");
  }
  ~DirChange()
  {
    int status = fchdir(dirhandle);
    if(status != 0) {
      close(dirhandle);
      // Don't throw, because this terminates.
      //      throw std::runtime_error("Call to fchdir failed");
    }
    close(dirhandle);
  }
private:
  int dirhandle;
};

//-----------------------------------------------------------------------
/// Return true if we were built with serialization support, false
/// otherwise. 
//-----------------------------------------------------------------------

bool SWIG_MAPPER_NAMESPACE::have_serialize_supported()
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  return true;
#else
  return false;
#endif
}

static bool skip_weak_ptr_handling_val = false;

//-----------------------------------------------------------------------
/// As described in weak_ptr_serialize_support, we can optionally
/// handle boost::weak_ptr for transversing the serialization twice -
/// once to mark pointer we encounter and once to serialize all
/// weak_ptr pointing to objects we encountered.
///
/// If you don't actually need this support for boost::weak_ptr, you
/// can turn this off and avoid the cost of navigating twice through
/// the serialization.
//-----------------------------------------------------------------------

bool SWIG_MAPPER_NAMESPACE::skip_weak_ptr_handling()
{ return skip_weak_ptr_handling_val; }

//-----------------------------------------------------------------------
/// As described in weak_ptr_serialize_support, we can optionally
/// handle boost::weak_ptr for transversing the serialization twice -
/// once to mark pointer we encounter and once to serialize all
/// weak_ptr pointing to objects we encountered.
///
/// If you don't actually need this support for boost::weak_ptr, you
/// can turn this off and avoid the cost of navigating twice through
/// the serialization.
//-----------------------------------------------------------------------

void SWIG_MAPPER_NAMESPACE::skip_weak_ptr_handling(bool Skip)
{ skip_weak_ptr_handling_val = Skip; }

//-----------------------------------------------------------------------
/// Function to go through serialization, marking all pointers we
/// encounter. 
//-----------------------------------------------------------------------

static void mark_pointer(const boost::shared_ptr<GenericObject>& Obj)
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  if(skip_weak_ptr_handling())
    return;
  clear_ptr_serialized_reference();
  std::ostringstream os_first_pass;
  boost::archive::polymorphic_binary_oarchive oa(os_first_pass);
  oa << boost::serialization::make_nvp("geocal_object", Obj);
  // Don't actually want output, just side effect of marking pointers
#else
  throw std::runtime_error("SWIG_MAPPER_NAMESPACE was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Simple function that wraps around writing a boost::serialization
/// to a xml archive. We abstract this
/// away to give a slightly cleaner interface, but mostly so we can 
/// later have the option of changing the underlying serialization. This
/// is a more limited interface, we can only write or read a single
/// object. But this is what we primarily do anyways, and we can easily
/// create higher level container objects if we end up needing multiple
/// objects (e.g., can have a std::map if we end up needing it).
//-----------------------------------------------------------------------

void SWIG_MAPPER_NAMESPACE::serialize_write(const std::string& Fname, 
			     const boost::shared_ptr<GenericObject>& Obj)
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  mark_pointer(Obj);
  OstreamCompress os(Fname.c_str());
  boost::archive::polymorphic_xml_oarchive oa(os);
  oa << boost::serialization::make_nvp("geocal_object", Obj);
#else
  throw std::runtime_error("SWIG_MAPPER_NAMESPACE was not built with boost::serialization support");
#endif
}

void SWIG_MAPPER_NAMESPACE::serialize_write_binary(const std::string& Fname, 
			     const boost::shared_ptr<GenericObject>& Obj)
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  mark_pointer(Obj);
  OstreamCompress os(Fname.c_str());
  boost::archive::polymorphic_binary_oarchive oa(os);
  oa << boost::serialization::make_nvp("geocal_object", Obj);
#else
  throw std::runtime_error("SWIG_MAPPER_NAMESPACE was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Variation of serialize_write that writes to a string instead of a file.
//-----------------------------------------------------------------------

std::string SWIG_MAPPER_NAMESPACE::serialize_write_string
(const boost::shared_ptr<GenericObject>& Obj)
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  mark_pointer(Obj);
  std::ostringstream os;
  {
    boost::archive::polymorphic_xml_oarchive oa(os);
    oa << boost::serialization::make_nvp("geocal_object", Obj);
  }
  return os.str();
#else
  throw std::runtime_error("SWIG_MAPPER_NAMESPACE was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Variation of serialize_write that writes to a binary string
/// instead of xml
//-----------------------------------------------------------------------

std::string SWIG_MAPPER_NAMESPACE::serialize_write_binary
(const boost::shared_ptr<GenericObject>& Obj)
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  mark_pointer(Obj);
  std::ostringstream os;
  {
    boost::archive::polymorphic_binary_oarchive oa(os);
    oa << boost::serialization::make_nvp("geocal_object", Obj);
  }
  return os.str();
#else
  throw std::runtime_error("SWIG_MAPPER_NAMESPACE was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Variation of serialize_read_generic that takes a binary string rather
/// than xml
//-----------------------------------------------------------------------

boost::shared_ptr<GenericObject> 
SWIG_MAPPER_NAMESPACE::serialize_read_binary(const std::string& Data)
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  std::istringstream is(Data);
  boost::archive::polymorphic_binary_iarchive ia(is);
  boost::shared_ptr<GenericObject> obj;
  ia >> boost::serialization::make_nvp("geocal_object", obj);
  return obj;
#else
  throw std::runtime_error("SWIG_MAPPER_NAMESPACE was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Simple function that wraps around reading a boost::serialization
/// to a xml archive. We abstract this
/// away to give a slightly cleaner interface, but mostly so we can 
/// later have the option of changing the underlying serialization. This
/// is a more limited interface, we can only write or read a single
/// object. But this is what we primarily do anyways, and we can easily
/// create higher level container objects if we end up needing multiple
/// objects (e.g., can have a std::map if we end up needing it).
///
/// Note that it can often be useful to have relative pathnames in a
/// xml file (e.g., we have a test xml file that is delivered with the
/// source, where the absolute path might changes). So before doing
/// the object creation, we change to the local directory of the xml
/// file. These means paths are relative to the xml file, *not* our
/// current directory.
//-----------------------------------------------------------------------

boost::shared_ptr<GenericObject> 
SWIG_MAPPER_NAMESPACE::serialize_read_generic(const std::string& Fname)
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  IstreamCompress is(Fname.c_str());
  if(!is)
    throw std::runtime_error("Trouble opening file " + Fname);
  boost::archive::polymorphic_xml_iarchive ia(is);
  boost::filesystem::path p(Fname);
  std::string dir = p.parent_path().string();
  if(dir == "")
    dir = ".";
  DirChange d(dir);
  boost::shared_ptr<GenericObject> obj;
  ia >> boost::serialization::make_nvp("geocal_object", obj);
  return obj;
#else
  throw std::runtime_error("SWIG_MAPPER_NAMESPACE was not built with boost::serialization support");
#endif
}

boost::shared_ptr<GenericObject> 
SWIG_MAPPER_NAMESPACE::serialize_read_binary_generic(const std::string& Fname)
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  IstreamCompress is(Fname.c_str());
  if(!is)
    throw std::runtime_error("Trouble opening file " + Fname);
  boost::archive::polymorphic_binary_iarchive ia(is);
  boost::filesystem::path p(Fname);
  std::string dir = p.parent_path().string();
  if(dir == "")
    dir = ".";
  DirChange d(dir);
  boost::shared_ptr<GenericObject> obj;
  ia >> boost::serialization::make_nvp("geocal_object", obj);
  return obj;
#else
  throw std::runtime_error("SWIG_MAPPER_NAMESPACE was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Variation of serialize_read_generic that takes a string rather
/// than reading a file.
//-----------------------------------------------------------------------

boost::shared_ptr<GenericObject> 
SWIG_MAPPER_NAMESPACE::serialize_read_generic_string(const std::string& Data)
{
#ifdef SWIG_HAVE_BOOST_SERIALIZATION
  std::istringstream is(Data);
  boost::archive::polymorphic_xml_iarchive ia(is);
  boost::shared_ptr<GenericObject> obj;
  ia >> boost::serialization::make_nvp("geocal_object", obj);
  return obj;
#else
  throw std::runtime_error("SWIG_MAPPER_NAMESPACE was not built with boost::serialization support");
#endif
}
