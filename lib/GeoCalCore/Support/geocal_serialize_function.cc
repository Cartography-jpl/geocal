#include "geocal_serialize_function.h"
#include "geocal_serialize_support.h"
#include "geocal_exception.h"
#include "dir_change.h"
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
#include <boost/archive/polymorphic_xml_iarchive.hpp>
#include <boost/archive/polymorphic_xml_oarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>
#endif
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Return true if we were built with serialization support, false
/// otherwise. 
//-----------------------------------------------------------------------

bool GeoCal::have_serialize_supported()
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  return true;
#else
  return false;
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

void GeoCal::serialize_write(const std::string& Fname, 
			     const boost::shared_ptr<GenericObject>& Obj)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::ofstream os(Fname.c_str());
  boost::archive::polymorphic_xml_oarchive oa(os);
  oa << boost::serialization::make_nvp("geocal_object", Obj);
#else
  throw Exception("GeoCal was not built with boost::serialization support");
#endif
}

void GeoCal::serialize_write_binary(const std::string& Fname, 
			     const boost::shared_ptr<GenericObject>& Obj)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::ofstream os(Fname.c_str());
  boost::archive::polymorphic_binary_oarchive oa(os);
  oa << boost::serialization::make_nvp("geocal_object", Obj);
#else
  throw Exception("GeoCal was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Variation of serialize_write that writes to a string instead of a file.
//-----------------------------------------------------------------------

std::string GeoCal::serialize_write_string
(const boost::shared_ptr<GenericObject>& Obj)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::ostringstream os;
  boost::archive::polymorphic_xml_oarchive oa(os);
  oa << boost::serialization::make_nvp("geocal_object", Obj);
  return os.str();
#else
  throw Exception("GeoCal was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Variation of serialize_write that writes to a binary string
/// instead of xml
//-----------------------------------------------------------------------

std::string GeoCal::serialize_write_binary
(const boost::shared_ptr<GenericObject>& Obj)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::ostringstream os;
  boost::archive::polymorphic_binary_oarchive oa(os);
  oa << boost::serialization::make_nvp("geocal_object", Obj);
  return os.str();
#else
  throw Exception("GeoCal was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Variation of serialize_read_generic that takes a binary string rather
/// than xml
//-----------------------------------------------------------------------

boost::shared_ptr<GenericObject> 
GeoCal::serialize_read_binary(const std::string& Data)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::istringstream is(Data);
  boost::archive::polymorphic_binary_iarchive ia(is);
  boost::shared_ptr<GenericObject> obj;
  ia >> boost::serialization::make_nvp("geocal_object", obj);
  return obj;
#else
  throw Exception("GeoCal was not built with boost::serialization support");
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
GeoCal::serialize_read_generic(const std::string& Fname)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::ifstream is(Fname.c_str());
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
  throw Exception("GeoCal was not built with boost::serialization support");
#endif
}

boost::shared_ptr<GenericObject> 
GeoCal::serialize_read_binary_generic(const std::string& Fname)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::ifstream is(Fname.c_str());
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
  throw Exception("GeoCal was not built with boost::serialization support");
#endif
}

//-----------------------------------------------------------------------
/// Variation of serialize_read_generic that takes a string rather
/// than reading a file.
//-----------------------------------------------------------------------

boost::shared_ptr<GenericObject> 
GeoCal::serialize_read_generic_string(const std::string& Data)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::istringstream is(Data);
  boost::archive::polymorphic_xml_iarchive ia(is);
  boost::shared_ptr<GenericObject> obj;
  ia >> boost::serialization::make_nvp("geocal_object", obj);
  return obj;
#else
  throw Exception("GeoCal was not built with boost::serialization support");
#endif
}
