// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
#ifndef SWIG_MODULE_ALREADY_DONE
#if SWIG_VERSION < 0x040000  
%module(directors="1", allprotected="1") foo
#else
%module(moduleimport="from ._swig_wrap import $module", directors="1", allprotected="1") foo
#endif
#endif

%{
#define SWIG_MAPPER_NAMESPACE GeoCal
%}
%include "swig_pickle.i"
%include "swig_python_attribute.i"
%{
#include <sstream>
#include <boost/math/quaternion.hpp>
#include <blitz/array.h>
#include "geocal_quaternion.h"
#include "geocal_exception.h"
%}

namespace boost {
  namespace math {
    template<class T> class quaternion {
    public:
      quaternion(T a, T b, T c, T d);
      %python_attribute(R_component_1, T);
      %python_attribute(R_component_2, T);
      %python_attribute(R_component_3, T);
      %python_attribute(R_component_4, T);
      %extend {
         quaternion<T > __add__(T x) {return (boost::math::quaternion<T >(*$self) += x); }
         quaternion<T > __add__(const quaternion<T >& x) {return (boost::math::quaternion<T >(*$self) += x); }
         quaternion<T > __sub__(T x) {return (boost::math::quaternion<T >(*$self) -= x); }
         quaternion<T > __sub__(const quaternion<T >& x) {return (boost::math::quaternion<T >(*$self) -= x); }
         quaternion<T > __mul__(T x) {return (boost::math::quaternion<T >(*$self) *= x); }
         quaternion<T > __mul__(const quaternion<T >& x) {return (boost::math::quaternion<T >(*$self) *= x); }
         quaternion<T > __div__(T x) {return (boost::math::quaternion<T >(*$self) /= x); }
         quaternion<T > __div__(const quaternion<T >& x) {return (boost::math::quaternion<T >(*$self) /= x); }
	 quaternion<T > conj() { return conj(*$self); }
	 blitz::Array<T, 2> to_matrix() const
	   { 
	     return GeoCal::quaternion_to_matrix(*$self);
	   }
	 static quaternion<T > 
	   from_matrix(const blitz::Array<T, 2>& Mat)
	 {
	   return GeoCal::matrix_to_quaternion(Mat);
	 }
         std::string print_to_string() const
         { 
           std::string buf("blah");
           buf.reserve(1000);
           std::ostringstream os(buf);
           os << *$self;
           return os.str();
         }
      }
      %pickle_init(1, self.R_component_1,
		   self.R_component_2, self.R_component_3,
		   self.R_component_4);
%pythoncode {
def __str__(self):
    return "Quaternion (" + str(self.R_component_1) + ", " + str(self.R_component_2) + ", " + str(self.R_component_3) + ", " + str(self.R_component_4) + ")"
}
    };
    template<class T> T abs(const boost::math::quaternion<T>&);
    %template(Quaternion_double) quaternion<double>;
    %template(Quaternion_double_abs) abs<double>;
  }
}
