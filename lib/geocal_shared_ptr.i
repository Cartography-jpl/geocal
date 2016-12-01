// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

// This wraps a shared_ptr up for GeoCal. In particular, we use
// RTTI to make sure that the most specific type is returned in
// python, rather than the most general type. This maps better to the
// standard duck typing done in python. See swig_cast_test.py for a
// simple example of this.

// There appears to be a bug in the shared_ptr handler of SWIG, as of
// version 2.0.4. This is the normal handler, with some fixes added.
%include "my_shared_ptr.i"

%{
#include "swig_type_mapper.h"
#include <boost/make_shared.hpp>
%}

%define %geocal_shared_ptr(TYPE...)
%shared_ptr(TYPE)
%init {
  GeoCal::SwigTypeMapperBase::add(typeid(TYPE), boost::make_shared<GeoCal::SwigTypeMapper< TYPE > > ("boost::shared_ptr< TYPE > *"));
}

%typemap(out) const boost::shared_ptr< TYPE >& {
  %set_output(GeoCal::swig_to_python($1));
}

%typemap(out) boost::shared_ptr< TYPE >& {
  %set_output(GeoCal::swig_to_python($1));
}

%typemap(out) boost::shared_ptr< TYPE > {
  %set_output(GeoCal::swig_to_python($1));
}

%typemap(out) boost::shared_ptr< TYPE >* {
  %set_output(GeoCal::swig_to_python(*$1));
}

%typemap(out) const boost::shared_ptr< TYPE > * {
  %set_output(GeoCal::swig_to_python(*$1));
}

%typemap(out) const boost::shared_ptr< TYPE > *& {
  %set_output(GeoCal::swig_to_python(*$1));
}

%enddef

