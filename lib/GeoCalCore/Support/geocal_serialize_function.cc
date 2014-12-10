#include "geocal_serialize_function.h"
#include "geocal_serialize_support.h"
#include "geocal_exception.h"
#include <fstream>
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
#include <boost/archive/polymorphic_xml_iarchive.hpp>
#include <boost/archive/polymorphic_xml_oarchive.hpp>
#endif
using namespace GeoCal;

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

//-----------------------------------------------------------------------
/// Simple function that wraps around reading a boost::serialization
/// to a xml archive. We abstract this
/// away to give a slightly cleaner interface, but mostly so we can 
/// later have the option of changing the underlying serialization. This
/// is a more limited interface, we can only write or read a single
/// object. But this is what we primarily do anyways, and we can easily
/// create higher level container objects if we end up needing multiple
/// objects (e.g., can have a std::map if we end up needing it).
//-----------------------------------------------------------------------

boost::shared_ptr<GenericObject> 
GeoCal::serialize_read_generic(const std::string& Fname)
{
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
  std::ifstream is(Fname.c_str());
  boost::archive::polymorphic_xml_iarchive ia(is);
  boost::shared_ptr<GenericObject> obj;
  ia >> boost::serialization::make_nvp("geocal_object", obj);
  return obj;
#else
  throw Exception("GeoCal was not built with boost::serialization support");
#endif
}
