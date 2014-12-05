#ifndef GEOCAL_SERIALIZE_FUNCTION_H
#define GEOCAL_SERIALIZE_FUNCTION_H

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>

namespace GeoCal {
/****************************************************************//**
  Simple function that wraps around a xml archive. We abstract this
  away to give a slightly cleaner interface, but mostly so we can 
  later have the option of changing the underlying serialization. This
  is a more limited interface, we can only write or read a single
  object. But this is what we primarily do anyways, and we can easily
  create higher level container objects if we end up needing multiple
  objects (e.g., can have a std::map if we end up needing it).
*******************************************************************/
inline void serialize_write(const std::string& Fname, 
		     const boost::shared_ptr<GenericObject>& Obj)
{
  std::ofstream os(Fname.c_str());
  boost::archive::xml_oarchive oa(os);
  oa << boost::serialization::make_nvp("geocal_object", Obj);
}

inline boost::shared_ptr<GenericObject> 
serialize_read_generic(const std::string& Fname)
{
  std::ifstream is(Fname.c_str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<GenericObject> obj;
  ia >> boost::serialization::make_nvp("geocal_object", obj);
  return obj;
}


template<class T> inline boost::shared_ptr<T> 
serialize_read(const std::string& Fname)
{
  return boost::dynamic_pointer_cast<T>(serialize_read_generic(Fname));
}
}
#endif
