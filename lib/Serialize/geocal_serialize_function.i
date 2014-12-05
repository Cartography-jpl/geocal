// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%{
  // Need this before anything else, so we get all the serialization
  // code
#define USE_BOOST_SERIALIZATON
%}
%include "common.i"

%{
#include "geocal_serialize.h"
#include "geocal_serialize_function.h"
%}
%import "generic_object.i"

namespace GeoCal {
  void serialize_write(const std::string& Fname, 
		       const boost::shared_ptr<GenericObject>& Obj);
  boost::shared_ptr<GenericObject> 
  serialize_read_generic(const std::string& Fname);
}
