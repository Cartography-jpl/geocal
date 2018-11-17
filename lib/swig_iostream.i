// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

// This defines some classes for making python objects look like
// ostream or istream objects.

%include "swig_iostream_inc.i"

%typemap(in) std::ostream& (boost::iostreams::filtering_ostream v) %{
    if($input != Py_None)
        v.push(python_fh_inserter($input));
    $1 = &v;
%}

%typemap(in) std::istream& (boost::iostreams::filtering_istream v) %{
    if($input != Py_None)
        v.push(python_fh_inserter($input));
    $1 = &v;
%}
