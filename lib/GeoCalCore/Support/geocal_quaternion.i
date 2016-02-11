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
  boost::math::quaternion<double> quat_rot_x(double A);
  boost::math::quaternion<double> quat_rot_y(double A);
  boost::math::quaternion<double> quat_rot_z(double A);
  boost::math::quaternion<double> quat_rot(const std::string& Rot, 
					   double A1);
  boost::math::quaternion<double> quat_rot(const std::string& Rot, 
					   double A1, double A2);
  boost::math::quaternion<double> quat_rot(const std::string& Rot, 
					   double A1, double A2, double A3);
  boost::math::quaternion<double> quat_rot(const std::string& Rot, 
					   double A1, double A2, double A3, 
					   double A4);
  boost::math::quaternion<double> quat_rot(const std::string& Rot, 
					   double A1, double A2, double A3, 
					   double A4, double A5);
  boost::math::quaternion<double> quat_rot(const std::string& Rot, 
					   double A1, double A2, double A3, 
					   double A4, double A5, double A6);
  boost::math::quaternion<double> quat_rot(const std::string& Rot, 
					   double A1, double A2, double A3, 
					   double A4, double A5, double A6,
					   double A7);
  blitz::Array<double, 2> 
  quaternion_to_matrix(const boost::math::quaternion<double>& q);
  boost::math::quaternion<double> 
  matrix_to_quaternion(const blitz::Array<double, 2>& m);
  blitz::Array<double, 1> 
  quaternion_to_array(const boost::math::quaternion<double>& q);
  boost::math::quaternion<double> 
  array_to_quaternion(const blitz::Array<double, 1>& m);
  void quat_to_ypr(const boost::math::quaternion<double>& qin, 
		   double& OUTPUT, double& OUTPUT, double& OUTPUT);
  void quat_to_euler(const boost::math::quaternion<double>& qin, 
		     double& OUTPUT, double& OUTPUT, double& OUTPUT);
  boost::math::quaternion<double> 
  determine_quat_rot(const boost::array<double, 3>& V1, 
		     const boost::array<double, 3>& V2);

  %name(quat_normalize) void normalize(boost::math::quaternion<double>& Q);

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

