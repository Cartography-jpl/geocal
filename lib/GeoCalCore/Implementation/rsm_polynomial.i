// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_polynomial"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::RsmPolynomial);

namespace GeoCal {
class RsmPolynomial : public GenericObject {
public:
  RsmPolynomial(int Np_x, int Np_y, int Np_z, bool Is_denominator=false,
		int Max_order=-1);
  std::string print_to_string() const;
  double operator()(double X, double Y, double Z) const;
  blitz::Array<double, 1> operator()(const blitz::Array<double, 1>& X,
    const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const;
  blitz::Array<double, 2> jacobian(const blitz::Array<double, 1>& X,
     const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const;
  %python_attribute(coefficient, blitz::Array<double, 3>);
  %python_attribute_with_set(fitted_coefficent, blitz::Array<double, 1>);
  %python_attribute(is_denominator, bool);
  %python_attribute(max_order, int);
  %pickle_serialization()
};
}
