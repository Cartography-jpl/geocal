// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%{
  // Need this before blitz gets included.
#include "auto_derivative.h"
%}
%include <std_vector.i>
%include "common.i"

%{
#include "geocal_autoderivative_quaternion.h"
%}
%import "swig_quaternion.i"
%import "auto_derivative.i"

%template(Quaternion_AutoDerivative_double) boost::math::quaternion<GeoCal::AutoDerivative<double> >;

namespace GeoCal {
  boost::math::quaternion<AutoDerivative<double> > quat_rot_x(const AutoDerivative<double>& A);
  boost::math::quaternion<AutoDerivative<double> > quat_rot_y(const AutoDerivative<double>& A);
  boost::math::quaternion<AutoDerivative<double> > quat_rot_z(const AutoDerivative<double>& A);
  boost::math::quaternion<AutoDerivative<double> > quat_rot(const std::string& Rot, 
					   const AutoDerivative<double>& A1);
  boost::math::quaternion<AutoDerivative<double> > quat_rot(const std::string& Rot, 
					   const AutoDerivative<double>& A1, const AutoDerivative<double>& A2);
  boost::math::quaternion<AutoDerivative<double> > quat_rot(const std::string& Rot, 
					   const AutoDerivative<double>& A1, const AutoDerivative<double>& A2, const AutoDerivative<double>& A3);
  boost::math::quaternion<AutoDerivative<double> > quat_rot(const std::string& Rot, 
					   const AutoDerivative<double>& A1, const AutoDerivative<double>& A2, const AutoDerivative<double>& A3, 
					   const AutoDerivative<double>& A4);
  boost::math::quaternion<AutoDerivative<double> > quat_rot(const std::string& Rot, 
					   const AutoDerivative<double>& A1, const AutoDerivative<double>& A2, const AutoDerivative<double>& A3, 
					   const AutoDerivative<double>& A4, const AutoDerivative<double>& A5);
  boost::math::quaternion<AutoDerivative<double> > quat_rot(const std::string& Rot, 
					   const AutoDerivative<double>& A1, const AutoDerivative<double>& A2, const AutoDerivative<double>& A3, 
					   const AutoDerivative<double>& A4, const AutoDerivative<double>& A5, const AutoDerivative<double>& A6);
  boost::math::quaternion<AutoDerivative<double> > quat_rot(const std::string& Rot, 
					   const AutoDerivative<double>& A1, const AutoDerivative<double>& A2, const AutoDerivative<double>& A3, 
					   const AutoDerivative<double>& A4, const AutoDerivative<double>& A5, const AutoDerivative<double>& A6,
					   const AutoDerivative<double>& A7);
 }

