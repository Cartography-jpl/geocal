// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"
%{
#include "generic_object.h"
%}

%shared_ptr(GeoCal::GenericObject)
namespace GeoCal {
class GenericObject {
public:
  virtual ~GenericObject();
};
}

%typemap(out) const boost::shared_ptr< GeoCal::GenericObject >& {
  %set_output(GeoCal::swig_to_python_or_none($1));
}

%typemap(out) boost::shared_ptr< GeoCal::GenericObject >& {
  %set_output(GeoCal::swig_to_python_or_none($1));
}

%typemap(out) boost::shared_ptr< GeoCal::GenericObject > {
  %set_output(GeoCal::swig_to_python_or_none($1));
}

