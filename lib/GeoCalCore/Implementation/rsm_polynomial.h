#ifndef RSM_POLYNOMIAL_H
#define RSM_POLYNOMIAL_H
#include "printable.h"
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <blitz/array.h>
namespace GeoCal {
  class RsmRationalPolynomial;
/****************************************************************//**
  This is used to handle a single polynomial for use with a 
  RsmRationalPolynomial. This is a low level class, you don't 
  normally use this directly (although you certainly could if it was
  useful).

  Note we might want to consider making this a template based on
  the sizes so we can specialize/optimize the code. We'll see what
  the performance is like.
*******************************************************************/

class RsmPolynomial : public boost::noncopyable,
		      public Printable<RsmPolynomial> {
  // Could make copyable, we just haven't bothered. But put this in
  // place so we don't accidentally try to copy
public:
  RsmPolynomial(int Np_x, int Np_y, int Np_z, bool Is_denominator=false,
		int Max_order=-1);
  RsmPolynomial(const RsmPolynomial& Rp);
  virtual ~RsmPolynomial() {}
  double operator()(double X, double Y, double Z) const;
  blitz::Array<double, 1> operator()(const blitz::Array<double, 1>& X,
     const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const;
  blitz::Array<double, 2> operator()(const blitz::Array<double, 2>& X,
     const blitz::Array<double, 2>& Y, const blitz::Array<double, 2>& Z) const;
  blitz::Array<double, 3> operator()(const blitz::Array<double, 3>& X,
     const blitz::Array<double, 3>& Y, const blitz::Array<double, 3>& Z) const;
  blitz::Array<double, 2> jacobian_fitted_coefficent
  (const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
   const blitz::Array<double, 1>& Z) const;
  blitz::Array<double, 1> jacobian(double X, double Y, double Z) const;

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

  void set_rpc_coeff(const boost::array<double, 20>& V);
  void print(std::ostream& Os) const;
  std::string tre_string() const;
  void read_tre_string(std::istream& In);
private:
  blitz::Array<double, 3> coefficient_;
  bool is_denominator_;
  int max_order_;
  int fitted_coefficent_size;

  RsmPolynomial() {}
  friend class boost::serialization::access;
  // This is a friend just so it can call RsmPolynomial default
  // constructor from its default constructor. It doesn't do anything
  // else with private member of RsmPolynomial.
  friend class RsmRationalPolynomial;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmPolynomial);
#endif

