// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "geocal_time.h"
%}

%base_import(generic_object)

%geocal_shared_ptr(GeoCal::Time);

%pythoncode {
import datetime
import time

def _new_time(pgs):
  return Time.time_pgs(pgs)
}

namespace GeoCal {

// Handle returns as a argout

#ifdef SWIGPYTHON
 %typemap(in,numinputs=0) Time &OUTPUT (GeoCal::Time temp) {
  $1 = &temp;
 }

 %typemap(argout) Time &OUTPUT {
   SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype> *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<$1_basetype>(new $1_basetype(*$1));
   %append_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_SHARED_PTR_QNAMESPACE::shared_ptr<GeoCal::Time> *), SWIG_POINTER_OWN));
 }
#endif

class Time : public GenericObject {
public:
  static Time time_et(double et);
  static Time time_pgs(double pgs);
  static Time time_j2000(double j2000);
  static Time time_gps(double gps);
  static Time time_unix(double unix_time);
  static Time time_acs(double acs);
  %python_attribute(acs, double)
  %python_attribute(pgs, double)
  %python_attribute(gps, double)
  %python_attribute(j2000, double)
  %python_attribute(et, double)
  static Time parse_time(const std::string Time_string);
  std::string print_to_string() const;
  static const Time min_valid_time;
  static const Time max_valid_time;
  
#ifdef SWIGPYTHON
  %extend {
     int __cmp__(const Time& T2) {
        if(*$self < T2)
         return -1;
        if(T2 < *$self)
         return 1;
        return 0;
     }
     Time __add__(double T) { return *$self + T; }
     Time __radd__(double T) { return *$self + T; }
     Time __sub__(double T) { return *$self - T; }
     double __sub__(const Time& T2) { return *$self - T2; }
     double unix_time() { return $self->unix_time_double(); }
  }
  %pythoncode {
def __reduce__(self):
  return _new_time, (self.pgs,)

  }
#endif
};

// Don't think we actually want to use the Python typemap here, so
// we'll suppress this for now. Leave code here in case we change our
// mind.

// //************************************************************
// // Type map to use the Python type datetime as input and output. 
// // We also support a python float as a unix time (seconds from
// // January 1, 1970, as the normal unix time is specified).
// //************************************************************

// #ifdef SWIGPYTHON

// %{
// //--------------------------------------------------------------
// // Function to return datetime module, importing if this is the
// // first call
// //--------------------------------------------------------------

// PyObject* datetime_module()
// {
//   static PyObject* mod = 0;
//   if(!mod)
//     mod = PyImport_ImportModule("datetime");
//   return mod;
// }

// //--------------------------------------------------------------
// // Return datetime.datetime object.
// //--------------------------------------------------------------

// PyObject* datetime_dot_datetime()
// {
//   static PyObject* obj = 0;
//   if(!obj)
//     obj = PyDict_GetItemString(PyModule_GetDict(datetime_module()) , 
// 			       "datetime");
//   return obj;
// }

// //--------------------------------------------------------------
// // Function to return time module, importing if this is the
// // first call
// //--------------------------------------------------------------

// PyObject* time_module()
// {
//   static PyObject* mod = 0;
//   if(!mod)
//     mod = PyImport_ImportModule("time");
//   return mod;
// }

// //--------------------------------------------------------------
// // Return time.mktime object.
// //--------------------------------------------------------------

// PyObject* time_dot_mktime()
// {
//   static PyObject* obj = 0;
//   if(!obj)
//     obj = PyDict_GetItemString(PyModule_GetDict(time_module()) , 
// 			       "mktime");
//   return obj;
// }

// //--------------------------------------------------------------
// // Convert datetime to GeoCal::Time. This includes handling 
// // the microsecond field for fractional seconds (time.mktime
// // truncates to seconds).
// //--------------------------------------------------------------

// GeoCal::Time datetime_to_time(PyObject* datetime)
// {
//   PyObject* tm = PyObject_CallMethod(datetime, 
// 				     const_cast<char*>("timetuple"), 
// 				     const_cast<char*>(""));
//   PythonObject res(PyObject_CallFunctionObjArgs(time_dot_mktime(), tm, NULL));
//   double tval = PyFloat_AsDouble(res);
//   PythonObject res2(PyObject_GetAttrString(datetime, "microsecond"));
//   tval += PyInt_AsLong(res2) / 1000000.0;
//   return GeoCal::Time::time_unix(tval);
// }
// %}

// //--------------------------------------------------------------
// // We convert unix time to GeoCal::Time object. 
// //--------------------------------------------------------------

// %typemap(in) GeoCal::Time {
//   if(PyFloat_Check($input)) {
//     $1 = GeoCal::Time::time_unix(PyFloat_AsDouble($input));
//   } else {
//     if(!PyObject_IsInstance($input, datetime_dot_datetime())) {
//       PyErr_SetString(PyExc_ValueError,"Expected a float or datetime object.");
//       return NULL;
//     }
//     $1 = datetime_to_time($input);
//   } 
// }

// //--------------------------------------------------------------
// // We convert unix time to GeoCal::Time object.
// //--------------------------------------------------------------

// %typemap(in) const GeoCal::Time& (GeoCal::Time t){
//   if(PyFloat_Check($input)) {
//     t = GeoCal::Time::time_unix(PyFloat_AsDouble($input));
//   } else {
//     if(!PyObject_IsInstance($input, datetime_dot_datetime())) {
//       PyErr_SetString(PyExc_ValueError,"Expected a float or datetime object.");
//       return NULL;
//     }
//     t = datetime_to_time($input);
//   } 
//   $1 = &t;
// }

// //--------------------------------------------------------------
// // Convert unix time from GeoCal::Time object to python
// // datetime. 
// //--------------------------------------------------------------

// %typemap(out) GeoCal::Time {
//   $result = PyObject_CallMethod(datetime_dot_datetime(), 
// 				const_cast<char*>("fromtimestamp"), 
// 				const_cast<char*>("d"), $1.unix_time());
// }

// //--------------------------------------------------------------
// // Convert unix time from GeoCal::Time object to python
// // datetime. 
// //--------------------------------------------------------------

// %typemap(out) const GeoCal::Time& {
//   $result = PyObject_CallMethod(datetime_dot_datetime(), 
// 				const_cast<char*>("fromtimestamp"), 
// 				const_cast<char*>("d"), $1->unix_time());
// }

// //--------------------------------------------------------------
// // Check if object is datetime or float.
// //--------------------------------------------------------------

// %typemap(typecheck,precedence=SWIG_TYPECHECK_INTEGER) GeoCal::Time, const GeoCal::Time& {
//   $1 = PyObject_IsInstance($input, datetime_dot_datetime()) || 
//     PyFloat_Check($input);
// }
// #endif

}

%template(Vector_Time) std::vector<GeoCal::Time>;
%extend std::vector<GeoCal::Time> {
  %pickle_vector()
};
