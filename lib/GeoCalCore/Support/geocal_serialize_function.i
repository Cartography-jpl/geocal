// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "geocal_serialize_function.h"
%}
%import "generic_object.i"

namespace GeoCal {
bool have_serialize_supported();
void serialize_write(const std::string& Fname, 
		     const boost::shared_ptr<GenericObject>& Obj);
std::string serialize_write_string(const boost::shared_ptr<GenericObject>& Obj);

boost::shared_ptr<GenericObject> 
serialize_read_generic(const std::string& Fname);

boost::shared_ptr<GenericObject> 
serialize_read_generic_string(const std::string& Data);

}
