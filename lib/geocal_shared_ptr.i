// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

// This wraps a shared_ptr up for GeoCal. In particular, we use
// RTTI to make sure that the most specific type is returned in
// python, rather than the most general type. This maps better to the
// standard duck typing done in python.

%{
#define SWIG_MAPPER_NAMESPACE GeoCal
%}

%include "shared_ptr_type_mapper.i"

%define %geocal_shared_ptr(TYPE...)
%shared_ptr_type_mapper(GeoCal, TYPE)
%enddef

