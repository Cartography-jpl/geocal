// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "swig_pickle.i"
%include "swig_python_attribute.i"
%{
#include <sstream>
#include <boost/math/quaternion.hpp>
#include <blitz/array.h>
#include "geocal_matrix.h"
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
         quaternion<T> __add__(T x) {return (boost::math::quaternion<T>(*$self) += x); }
         quaternion<T> __add__(const quaternion<T>& x) {return (boost::math::quaternion<T>(*$self) += x); }
         quaternion<T> __sub__(T x) {return (boost::math::quaternion<T>(*$self) -= x); }
         quaternion<T> __sub__(const quaternion<T>& x) {return (boost::math::quaternion<T>(*$self) -= x); }
         quaternion<T> __mul__(T x) {return (boost::math::quaternion<T>(*$self) *= x); }
         quaternion<T> __mul__(const quaternion<T>& x) {return (boost::math::quaternion<T>(*$self) *= x); }
         quaternion<T> __div__(T x) {return (boost::math::quaternion<T>(*$self) /= x); }
         quaternion<T> __div__(const quaternion<T>& x) {return (boost::math::quaternion<T>(*$self) /= x); }
	 quaternion<T> conj() { return conj(*$self); }
	 blitz::Array<double, 2> to_matrix() const
	   { blitz::Array<double, 2> res(3,3);
	     double d[3][3];
	     GeoCal::quaternion_to_matrix(*$self, d);
	     for(int i = 0; i < 3; ++i)
	       for(int j = 0; j < 3; ++j)
		 res(i,j) = d[i][j];
	     return res;
	   }
	 static quaternion<double> 
	   from_matrix(const blitz::Array<double, 2>& Mat) const
	 {
	   if(Mat.rows() != 3 || Mat.cols() != 3)
	     throw GeoCal::Exception("Matrix must be 3x3");
	   double d[3][3];
	     for(int i = 0; i < 3; ++i)
	       for(int j = 0; j < 3; ++j)
		 d[i][j] = Mat(i,j);
	   return GeoCal::matrix_to_quaternion(d);
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
    };
    %template(Quaternion_double) quaternion<double>;
  }
}
