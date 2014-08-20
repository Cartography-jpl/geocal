// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "geocal_quaternion.h"
%}
%import "swig_quaternion.i"

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
  void quat_to_ypr(const boost::math::quaternion<double>& qin, 
		   double& OUTPUT, double& OUTPUT, double& OUTPUT);
 }

