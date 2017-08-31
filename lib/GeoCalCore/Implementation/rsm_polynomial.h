#ifndef RSM_POLYNOMIAL_H
#define RSM_POLYNOMIAL_H
#include "printable.h"
#include <blitz/array.h>
namespace GeoCal {
/****************************************************************//**
  This is used to handle a single polynomial for use with a 
  RsmRationalPolynomial. This is a low level class, you don't 
  normally use this directly (although you certainly could if it was
  useful).

  Note we might want to consider making this a template based on
  the sizes so we can specialize/optimize the code. We'll see what
  the performance is like.
*******************************************************************/

class RsmPolynomial : public Printable<RsmPolynomial> {
public:
  RsmPolynomial(int Np_x, int Np_y, int Np_z, bool Is_denominator=false,
		int Max_order=-1);
  virtual ~RsmPolynomial() {}
  double operator()(double X, double Y, double Z) const;
  blitz::Array<double, 1> operator()(const blitz::Array<double, 1>& X,
     const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const;
  blitz::Array<double, 2> jacobian(const blitz::Array<double, 1>& X,
     const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const;

//-----------------------------------------------------------------------
/// The full set of coefficients for the polynomial. 
//-----------------------------------------------------------------------

  const blitz::Array<double, 3>& coefficient() const {return coefficient_;}
  blitz::Array<double, 1> fitted_coefficent() const;
  void fitted_coefficent(const blitz::Array<double, 1>& Fc);

//-----------------------------------------------------------------------
/// True if this is a denominator polynomial. We hold the constant
/// term at 1.0 by convention, and don't fit for this.
//-----------------------------------------------------------------------
  
  bool is_denominator() const { return is_denominator_;}

//-----------------------------------------------------------------------
/// Maximum order cross term we fit for, or -1 if we don't restrict this.
//-----------------------------------------------------------------------

  int max_order() const {return max_order_;}
  
  void print(std::ostream& Os) const;
private:
  blitz::Array<double, 3> coefficient_;
  bool is_denominator_;
  int max_order_;
  int fitted_coefficent_size;

  RsmPolynomial() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmPolynomial);
#endif

