#ifndef GEOCAL_SERIALIZE_FUNCTION_H
#define GEOCAL_SERIALIZE_FUNCTION_H
#include "generic_object.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
void serialize_write(const std::string& Fname, 
		     const boost::shared_ptr<GenericObject>& Obj);

boost::shared_ptr<GenericObject> 
serialize_read_generic(const std::string& Fname);


template<class T> inline boost::shared_ptr<T> 
serialize_read(const std::string& Fname)
{
  return boost::dynamic_pointer_cast<T>(serialize_read_generic(Fname));
}
}
#endif
