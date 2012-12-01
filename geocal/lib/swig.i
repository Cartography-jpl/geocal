// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%module(directors="1", allprotected="1") geocal

// Map std::string to and from the native string type
%naturalvar std::string;
%include <std_string.i>

// On the Mac, passing empty strings between shared libraries is
// broken. So set up a special rule to make sure any temporary is
// initialized to a dummy value.

//--------------------------------------------------------------
// Translate exceptions into the appropriate language exception type
//--------------------------------------------------------------

%include "exception.i"

%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  } catch (Swig::DirectorException &e) { 
    SWIG_fail; 
  }
}

%{
static std::string parse_python_exception() {
  PyObject *type = NULL, *value = NULL, *tb = NULL;
  PyErr_Fetch(&type, &value, &tb);
  PyObject* mod = PyImport_ImportModule("traceback");
  PyObject* err_str_list = PyObject_CallMethodObjArgs(mod,
		 PyString_FromString("format_exception"), type, value, tb, NULL);
  std::string ret = "Python error that I can't parse";
  if(err_str_list) {
    PyObject* err_str = 
      PyObject_CallMethodObjArgs(PyString_FromString(""),
				 PyString_FromString("join"), 
				 err_str_list, NULL);
    if(err_str)
      ret = PyString_AsString(err_str);
    Py_XDECREF(err_str);
  } else if(value) {
    ret = PyString_AsString(value);
  }
  Py_XDECREF(mod);
  Py_XDECREF(err_str_list);
  Py_XDECREF(type);
  Py_XDECREF(value);
  Py_XDECREF(tb);
  return ret;
}
%}
%feature("director:except") {
    if ($error != NULL) {
      GeoCal::Exception e;
      e << "Python error occured:\n"
	<< parse_python_exception();
      throw e;
    }
}

//--------------------------------------------------------------
// Add support for boost shared_ptr.
//--------------------------------------------------------------

#ifdef SWIGPYTHON
%{
#include <boost/shared_ptr.hpp>
%}

// There appears to be a bug in the shared_ptr handler of SWIG, as of
// version 2.0.4. This is the normal handler, with some fixes added.
//%include "boost_shared_ptr.i"
%include "my_shared_ptr.i"

//--------------------------------------------------------------
// Function that allows us to access something as a python 
// attribute.
//--------------------------------------------------------------

 // We stick the TYPE at the end with the "..." so that we can pass
 // templates that contain "," through. This avoid the normal problem
 // with the preprocessor of getting confused by "," in a template
 // name, thinking these are separate arguments to macro

%define %python_attribute(NAME, TYPE...)
  %rename(_ ## NAME) NAME;
  TYPE NAME() const;
%pythoncode {
@property
def NAME(self):
    return self._ ## NAME()
}
%enddef

%define %python_attribute_with_set(NAME, TYPE...)
  %rename(_ ## NAME) NAME;
  TYPE NAME() const;
  void NAME(const TYPE& V);
%pythoncode {
@property
def NAME(self):
    return self._ ## NAME()

@NAME.setter
def NAME(self, value):
  self._ ## NAME(value)
}
%enddef

%define %python_attribute_nonconst(NAME, TYPE...)
  %rename(_ ## NAME) NAME;
  TYPE NAME();
%pythoncode {
@property
def NAME(self):
    return self._ ## NAME()
}
%enddef

%define %python_attribute2(NAME, NAME2, TYPE...)
  %rename(_ ## NAME) NAME2;
  TYPE NAME2() const;
%pythoncode {
@property
def NAME(self):
    return self._ ## NAME()
}
%enddef

%define %python_attribute2_nonconst(NAME, NAME2, TYPE...)
  %rename(_ ## NAME) NAME2;
  TYPE NAME2();
%pythoncode {
@property
def NAME(self):
    return self._ ## NAME()
}
%enddef

%define %python_attribute_abstract(NAME, TYPE...)
  %rename(_ ## NAME) NAME;
  virtual TYPE NAME() const = 0;
%pythoncode {
@property
def NAME(self):
    return self._ ## NAME()
}
%enddef

%define %geocal_shared_ptr(NAME)
%shared_ptr(GeoCal::NAME);
%enddef

#else
%include "ruby_shared_ptr.i"
#endif

#ifdef SWIGPYTHON

//--------------------------------------------------------------
// Make sure numpy is available
//--------------------------------------------------------------

%pythoncode {
  import numpy as np
}
//--------------------------------------------------------------
// Helper class for python that holds an object and when deleted
// decrements the reference to it.
//--------------------------------------------------------------

%{
class PythonObject {
public:
  PythonObject(PyObject* Obj = 0) : obj(Obj) {}
  ~PythonObject() { Py_XDECREF(obj); }
  PyObject* obj;
  operator PyObject*() {return obj;}
};

%}

//--------------------------------------------------------------
// We use the generic name "print_to_string" in our classes, but
// this corresponds to the __str__ object in python. So go ahead
// and rename everything if we are in python
//--------------------------------------------------------------

%rename(__str__) print_to_string;

%pythoncode {
def _new_from_init(cls, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example ImageCoordinate'''
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

def _new_from_set(cls, *args):
    '''For use with pickle, covers common case where we use a set function 
    to assign the value'''
    inst = cls.__new__(cls)
    inst.__init__()
    inst.set(*args)
    return inst
}

%define %pickle_init(ARG...)
  %pythoncode {
def __reduce__(self):
  return _new_from_init, (self.__class__, ARG)
}
%enddef

#endif

