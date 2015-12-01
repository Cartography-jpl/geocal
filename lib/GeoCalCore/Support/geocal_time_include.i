// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
// This goes with geocal_time.i. This should be %include rather than
// %import
%include <std_vector.i>
%include "common.i"

%{
#include "geocal_time.h"
%}

%import "geocal_time.i"

%template(Vector_Time) std::vector<GeoCal::Time>;
%template(Vector_Time2) std::vector<boost::shared_ptr<GeoCal::Time> >;
%extend std::vector<GeoCal::Time> {
  %pickle_serialization();
};

//--------------------------------------------------------------
/// The default conversion of a python sequence to a std::vector<Time>
/// doesn't work This expression here does this correctly.
//--------------------------------------------------------------

%{
namespace swig {
  template <>
  struct traits_asptr<std::vector<GeoCal::Time> >  {
    static int asptr(PyObject *obj, 
		     std::vector<GeoCal::Time> **vec) {
      // Long name, so shorten
      typedef std::vector<GeoCal::Time> vtype;
      if (obj == Py_None || SWIG_Python_GetSwigThis(obj)) {
	vtype *p;
	if (::SWIG_ConvertPtr(obj,(void**)&p,
			      swig::type_info<vtype>(),0) == SWIG_OK) {
	  if (vec) *vec = p;
	  return SWIG_OLDOBJ;
	}
      } else if (PySequence_Check(obj)) {
	try {
	  if (vec) {
	    vtype *pseq = new vtype();
	    PyObject *iterator = PyObject_GetIter(obj);
	    while(PyObject *item = PyIter_Next(iterator)) {
	      boost::shared_ptr<GeoCal::Time> *itemp;
	      int newmem = 0;
	      int res = SWIG_ConvertPtrAndOwn(item, (void**) &itemp, 
					      swig::type_info<boost::shared_ptr<GeoCal::Time> >(),  0, &newmem);
	      if(!SWIG_IsOK(res)) {
		Py_DECREF(item);
		Py_DECREF(iterator);
		return SWIG_ERROR;
	      }
	      pseq->push_back(**itemp);
	      Py_DECREF(item);
	    }
	    Py_DECREF(iterator);
	    *vec = pseq;
	    return SWIG_NEWOBJ;
	  } else {
	    SwigPySequence_Cont<boost::shared_ptr<GeoCal::Time> > swigpyseq(obj);
	    return swigpyseq.check() ? SWIG_OK : SWIG_ERROR;
	  }
	} catch (std::exception& e) {
	  if (vec) {
	    if (!PyErr_Occurred()) {
	      PyErr_SetString(PyExc_TypeError, e.what());
	    }
	  }
	  return SWIG_ERROR;
	}
      }
      return SWIG_ERROR;
    }
  };
}
%}
