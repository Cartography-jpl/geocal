#include "rsm_polynomial.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmPolynomial::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmPolynomial);
  ar & GEOCAL_NVP_(coefficient)
    & GEOCAL_NVP_(is_denominator)
    & GEOCAL_NVP_(max_order)
    & GEOCAL_NVP(fitted_coefficent_size);
}

GEOCAL_IMPLEMENT(RsmPolynomial);
#endif

//-----------------------------------------------------------------------
/// Constructor. You indicated the order of the polynomial in each
/// dimension, and if this is a denominator (where we hold the
/// constant term to 1.0 by convention, and don't fit for this). The
/// maximum order of cross terms can be given, or left as -1 in which
/// case we don't limit the cross terms.
//-----------------------------------------------------------------------

RsmPolynomial::RsmPolynomial(int Np_x, int Np_y, int Np_z,
			     bool Is_denominator,
			     int Max_order)
: coefficient_(Np_x+1, Np_y+1, Np_z+1),
  is_denominator_(Is_denominator),
  max_order_(Max_order)
{
  fitted_coefficent_size = 0;
  for(int i = 0; i < coefficient_.rows(); ++i)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int k = 0; k < coefficient_.depth(); ++k)
	if(!(is_denominator_  && i == 0  && j == 0 && k ==0))
	  if(max_order_ < 0 || i + j + k <= max_order_)
	    ++fitted_coefficent_size;
}

//-----------------------------------------------------------------------
/// Apply the polynomial to the given X, Y, and Z value.
//-----------------------------------------------------------------------

double RsmPolynomial::operator()(double X, double Y, double Z) const
{
  firstIndex i1; secondIndex i2; thirdIndex i3;
  Array<double, 1> xpow(coefficient_.rows());
  Array<double, 1> ypow(coefficient_.cols());
  Array<double, 1> zpow(coefficient_.depth());
  xpow(0) = ypow(0) = zpow(0) = 1;
  for(int i = 1; i < xpow.rows(); ++i)
    xpow(i) = xpow(i-1) * X;
  for(int i = 1; i < ypow.rows(); ++i)
    ypow(i) = ypow(i-1) * Y;
  for(int i = 1; i < zpow.rows(); ++i)
    zpow(i) = zpow(i-1) * Z;
  double res = sum(coefficient_ * xpow(i1) * ypow(i2) * zpow(i3));
  return res;
}

//-----------------------------------------------------------------------
/// Apply the polynomial to the given X, Y, and Z value.
//-----------------------------------------------------------------------

blitz::Array<double, 1> RsmPolynomial::operator()
(const blitz::Array<double, 1>& X,
 const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const
{
  firstIndex i1; secondIndex i2; thirdIndex i3; fourthIndex i4;
  Range ra = Range::all();
  if(X.rows() != Y.rows() || X.rows() != Z.rows())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 2> xpow(X.rows(),coefficient_.rows());
  Array<double, 2> ypow(Y.rows(),coefficient_.cols());
  Array<double, 2> zpow(Z.rows(),coefficient_.depth());
  xpow(ra, 0) = 1;
  ypow(ra, 0) = 1;
  zpow(ra, 0) = 1;
  for(int i = 1; i < xpow.cols(); ++i)
    xpow(ra, i) = xpow(ra, i-1) * X;
  for(int i = 1; i < ypow.cols(); ++i)
    ypow(ra, i) = ypow(ra, i-1) * Y;
  for(int i = 1; i < zpow.cols(); ++i)
    zpow(ra, i) = zpow(ra, i-1) * Z;

  blitz::Array<double, 1> res(xpow.rows());
  // This looks like a valid statement, but it seems to confuse blitz.
  // So we expand this out
  //res = sum(sum(sum(coefficient_(i2,i3,i4) * xpow(i1,i2) * ypow(i1,i3) *
  //zpow(i1,i4), i4),i3),i2);
  res = 0;
  for(int i = 0; i < xpow.cols(); ++i)
    for(int j = 0; j < ypow.cols(); ++j)
      for(int k = 0; k < zpow.cols(); ++k)
	res += coefficient_(i,j,k) * xpow(ra,i) * ypow(ra,j) * zpow(ra,k);
  return res;
}

//-----------------------------------------------------------------------
/// 
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmPolynomial::jacobian
(const blitz::Array<double, 1>& X,
 const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const
{
}

//-----------------------------------------------------------------------
/// Return the set of coefficients that we are fitting for.
//-----------------------------------------------------------------------

blitz::Array<double, 1> RsmPolynomial::fitted_coefficent() const
{
  blitz::Array<double, 1> res(fitted_coefficent_size);
  int ind = 0;
  for(int i = 0; i < coefficient_.rows(); ++i)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int k = 0; k < coefficient_.depth(); ++k)
	if(!(is_denominator_  && i == 0  && j == 0 && k ==0))
	  if(max_order_ < 0 || i + j + k <= max_order_)
	    res(ind++) = coefficient_(i,j,k);
  return res;
}

//-----------------------------------------------------------------------
/// Set the value of the set of coefficients that we are fitting for.
//-----------------------------------------------------------------------

void RsmPolynomial::fitted_coefficent(const blitz::Array<double, 1>& Fc)
{
  if(Fc.rows() != fitted_coefficent_size)
    throw Exception("Fc is the wrong size");
  int ind = 0;
  for(int i = 0; i < coefficient_.rows(); ++i)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int k = 0; k < coefficient_.depth(); ++k) {
	if(!(is_denominator_  && i == 0  && j == 0 && k ==0)) {
	  if(max_order_ < 0 || i + j + k <= max_order_)
	    coefficient_(i,j,k) = Fc(ind++);
	  else
	    coefficient_(i,j,k) = 0;
	} else {
	    coefficient_(i,j,k) = 1;
	}
      }
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmPolynomial::print(std::ostream& Os) const
{
  Os << "RsmPolynomial:\n"
     << "  Np x:           " << coefficient_.rows()-1 << "\n"
     << "  Np y:           " << coefficient_.cols()-1 << "\n"
     << "  Np z:           " << coefficient_.depth()-1 << "\n"
     << "  Is denominator: " << is_denominator_ << "\n"
     << "  Max order:      "  << max_order_ << "\n";
}
