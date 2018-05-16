// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "camera_rational_polynomial.h"
%}

%base_import(quaternion_camera)

%geocal_shared_ptr(GeoCal::CameraRationalPolyomial);
namespace GeoCal {
class CameraRationalPolyomial : public QuaternionCamera {
public:
  CameraRationalPolyomial(int Nu, int Nv, double Xi, double U0,
			  double V0,
			  boost::math::quaternion<double> Pld_q_c,
			  const blitz::Array<double, 2>& Kappa,
			  const blitz::Array<double, 2>& Kappa_inverse);
  %python_attribute(kappa, const blitz::Array<double, 2>&);
  %python_attribute(kappa_inverse, const blitz::Array<double, 2>&);
  blitz::Array<double, 1> apply_rational(const blitz::Array<double, 1>& X,
			 const blitz::Array<double, 2>& Coeff) const;
  blitz::Array<double, 1> construct_chi_matrix(const blitz::Array<double, 1>& X,
					       int ord) const;
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("CameraRationalPolyomial")
