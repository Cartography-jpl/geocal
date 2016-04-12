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

%typemap(out) std::string {
  $result = PyByteArray_FromStringAndSize($1.data(), $1.size());
}

%typemap(in) const std::string& {
  if(!PyByteArray_Check($input)) {
    PyErr_Clear();
    PyErr_SetString(PyExc_TypeError,"not a bytearray");
    return NULL;
  }
  $1 = new std::string(PyByteArray_AS_STRING($input), PyByteArray_GET_SIZE($input));
}

%typemap(freearg) const std::string& {
  delete $1;
}

namespace GeoCal {
std::string serialize_write_binary(const boost::shared_ptr<GenericObject>& Obj);

boost::shared_ptr<GenericObject> 
serialize_read_binary(const std::string& Data);
}
