#include "rsm_polynomial.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
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
  coefficient_ = 0;
  for(int i = 0; i < coefficient_.rows(); ++i)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int k = 0; k < coefficient_.depth(); ++k)
	if(!(is_denominator_  && i == 0  && j == 0 && k ==0))
	  if(max_order_ < 0 || i + j + k <= max_order_)
	    ++fitted_coefficent_size;
}

//-----------------------------------------------------------------------
/// Copy constructor.
//-----------------------------------------------------------------------

RsmPolynomial::RsmPolynomial(const RsmPolynomial& Rp)
  : coefficient_(Rp.coefficient_.copy()),
    is_denominator_(Rp.is_denominator_),
    max_order_(Rp.max_order_),
    fitted_coefficent_size(Rp.fitted_coefficent_size)
{
}

//-----------------------------------------------------------------------
/// Determine if we have a zero crossing. We use a simple sufficient
/// test, and if that fails we look directly for zero crossings using
/// A spacing in X,Y,Z of the given spacing.
//-----------------------------------------------------------------------

bool RsmPolynomial::check_zero_crossing(double Grid_spacing) const
{
  // A sufficient condition is the sum of all the abs value of all
  // coefficients excluding positive coefficients with all even powers
  // if < 1. Failure of this condition doesn't mean we have a zero
  // crossing, but we then need a more exhaustive check.
  double sum_abs = 0.0;
  for(int i = 0; i  < coefficient_.rows(); ++i)
    for(int j = 0; j  < coefficient_.cols(); ++j)
      for(int k = 0; k  < coefficient_.depth(); ++k) {
	if(coefficient_(i,j,k) > 0 &&
	   i % 2 == 0 && j % 2 == 0 && k % 2 == 0)
	  sum_abs += 0;
	else
	  sum_abs += fabs(coefficient_(i,j,k));
      }
  if(sum_abs < 1)
    return false;
  int sign = ((*this)(-1,-1,-1) < 0 ? -1 : 1);
  for(double x = -1.0; x <= 1.0; x += Grid_spacing)
    for(double y = -1.0; y <= 1.0; y += Grid_spacing)
      for(double z = -1.0; z <= 1.0; z += Grid_spacing)
	if(sign * (*this)(x,y,z) < 0)
	  return true;
  return false;
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
  for(int i = 1; i < coefficient_.rows(); ++i)
    xpow(ra, i) = xpow(ra, i-1) * X;
  for(int i = 1; i < coefficient_.cols(); ++i)
    ypow(ra, i) = ypow(ra, i-1) * Y;
  for(int i = 1; i < coefficient_.depth(); ++i)
    zpow(ra, i) = zpow(ra, i-1) * Z;

  blitz::Array<double, 1> res(xpow.rows());
  // This looks like a valid statement, but it seems to confuse blitz.
  // So we expand this out
  //res = sum(sum(sum(coefficient_(i2,i3,i4) * xpow(i1,i2) * ypow(i1,i3) *
  //zpow(i1,i4), i4),i3),i2);
  res = 0;
  for(int i = 0; i < coefficient_.rows(); ++i)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int k = 0; k < coefficient_.depth(); ++k)
	res += coefficient_(i,j,k) * xpow(ra,i) * ypow(ra,j) * zpow(ra,k);
  return res;
}

//-----------------------------------------------------------------------
/// Apply the polynomial to the given X, Y, and Z value.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmPolynomial::operator()
(const blitz::Array<double, 2>& X,
 const blitz::Array<double, 2>& Y, const blitz::Array<double, 2>& Z) const
{
  firstIndex i1; secondIndex i2; thirdIndex i3; fourthIndex i4;
  Range ra = Range::all();
  if(X.rows() != Y.rows() || X.rows() != Z.rows() ||
     X.cols() != Y.cols() || X.cols() != Z.cols())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 3> xpow(X.rows(),X.cols(),coefficient_.rows());
  Array<double, 3> ypow(Y.rows(),Y.cols(),coefficient_.cols());
  Array<double, 3> zpow(Z.rows(),Z.cols(),coefficient_.depth());
  xpow(ra, ra, 0) = 1;
  ypow(ra, ra, 0) = 1;
  zpow(ra, ra, 0) = 1;
  for(int i = 1; i < coefficient_.rows(); ++i)
    xpow(ra, ra, i) = xpow(ra, ra, i-1) * X;
  for(int i = 1; i < coefficient_.cols(); ++i)
    ypow(ra, ra, i) = ypow(ra, ra, i-1) * Y;
  for(int i = 1; i < coefficient_.depth(); ++i)
    zpow(ra, ra, i) = zpow(ra, ra, i-1) * Z;

  blitz::Array<double, 2> res(X.shape());
  // This looks like a valid statement, but it seems to confuse blitz.
  // So we expand this out
  //res = sum(sum(sum(coefficient_(i2,i3,i4) * xpow(i1,i2) * ypow(i1,i3) *
  //zpow(i1,i4), i4),i3),i2);
  res = 0;
  for(int i = 0; i < coefficient_.rows(); ++i)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int k = 0; k < coefficient_.depth(); ++k)
	res += coefficient_(i,j,k) * xpow(ra,ra,i) * ypow(ra,ra,j) *
	  zpow(ra,ra,k);
  return res;
}

//-----------------------------------------------------------------------
/// Apply the polynomial to the given X, Y, and Z value.
//-----------------------------------------------------------------------

blitz::Array<double, 3> RsmPolynomial::operator()
(const blitz::Array<double, 3>& X,
 const blitz::Array<double, 3>& Y, const blitz::Array<double, 3>& Z) const
{
  firstIndex i1; secondIndex i2; thirdIndex i3; fourthIndex i4;
  Range ra = Range::all();
  if(X.rows() != Y.rows() || X.rows() != Z.rows() ||
     X.cols() != Y.cols() || X.cols() != Z.cols() ||
     X.depth() != Y.depth() || X.depth() != Z.depth())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 4> xpow(X.rows(),X.cols(),X.depth(),coefficient_.rows());
  Array<double, 4> ypow(Y.rows(),Y.cols(),Y.depth(),coefficient_.cols());
  Array<double, 4> zpow(Z.rows(),Z.cols(),Z.depth(),coefficient_.depth());
  xpow(ra, ra, ra, 0) = 1;
  ypow(ra, ra, ra, 0) = 1;
  zpow(ra, ra, ra, 0) = 1;
  for(int i = 1; i < coefficient_.rows(); ++i)
    xpow(ra, ra, ra,i) = xpow(ra, ra, ra,i-1) * X;
  for(int i = 1; i < coefficient_.cols(); ++i)
    ypow(ra, ra, ra,i) = ypow(ra, ra, ra,i-1) * Y;
  for(int i = 1; i < coefficient_.depth(); ++i)
    zpow(ra, ra, ra,i) = zpow(ra, ra, ra,i-1) * Z;

  blitz::Array<double, 3> res(X.shape());
  // This looks like a valid statement, but it seems to confuse blitz.
  // So we expand this out
  //res = sum(sum(sum(coefficient_(i2,i3,i4) * xpow(i1,i2) * ypow(i1,i3) *
  //zpow(i1,i4), i4),i3),i2);
  res = 0;
  for(int i = 0; i < coefficient_.rows(); ++i)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int k = 0; k < coefficient_.depth(); ++k)
	res += coefficient_(i,j,k) * xpow(ra,ra,ra,i) * ypow(ra,ra,ra,j) *
	  zpow(ra,ra,ra,k);
  return res;
}

//-----------------------------------------------------------------------
/// This is the jacobian of the value with respect to
/// the fitted_coefficent
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmPolynomial::jacobian_fitted_coefficent
(const blitz::Array<double, 1>& X,
 const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const
{
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
  blitz::Array<double, 2> res(xpow.rows(), fitted_coefficent_size);
  int ind = 0;
  for(int i = 0; i < coefficient_.rows(); ++i)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int k = 0; k < coefficient_.depth(); ++k)
	if(!(is_denominator_  && i == 0  && j == 0 && k ==0))
	  if(max_order_ < 0 || i + j + k <= max_order_)
	    res(ra, ind++) = xpow(ra,i) * ypow(ra,j) * zpow(ra, k);
  return res;
}

//-----------------------------------------------------------------------
/// This is the jacobian of the value with respect to
/// X, Y, and Z.
//-----------------------------------------------------------------------

blitz::Array<double, 1> RsmPolynomial::jacobian
(double X, double Y, double Z) const
{
  Array<double, 1> res(3);
  res = 0.0;
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
  for(int i = 0; i < coefficient_.rows(); ++i)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int k = 0; k < coefficient_.depth(); ++k) {
	if(i > 0)
	  res(0) += coefficient_(i,j,k) * i * xpow(i-1) * ypow(j) * zpow(k);
	if(j > 0)
	  res(1) += coefficient_(i,j,k) * xpow(i) * j * ypow(j-1) * zpow(k);
	if(k > 0)
	  res(2) += coefficient_(i,j,k) * xpow(i) * ypow(j) * k * zpow(k-1);
      }
  return res;
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
/// Set a RsmPolynomial to match coefficients from RPC_B format RPC.
/// This is really only useful for testing, comparing against our RPC code
//-----------------------------------------------------------------------

void RsmPolynomial::set_rpc_coeff(const boost::array<double, 20>& V)
{
  if(coefficient_.rows() < 4 || coefficient_.cols() < 4 ||
     coefficient_.depth() < 4)
    throw Exception("We need to have at least a 3rd order polynomial to assign RPC coefficients");
  coefficient_ = 0;
  coefficient_(0,0,0) = V[0];
  coefficient_(1,0,0) = V[1];
  coefficient_(0,1,0) = V[2];
  coefficient_(0,0,1) = V[3];
  coefficient_(1,1,0) = V[4];
  coefficient_(1,0,1) = V[5];
  coefficient_(0,1,1) = V[6];
  coefficient_(2,0,0) = V[7];
  coefficient_(0,2,0) = V[8];
  coefficient_(0,0,2) = V[9];
  coefficient_(1,1,1) = V[10];
  coefficient_(3,0,0) = V[11];
  coefficient_(1,2,0) = V[12];
  coefficient_(1,0,2) = V[13];
  coefficient_(2,1,0) = V[14];
  coefficient_(0,3,0) = V[15];
  coefficient_(0,1,2) = V[16];
  coefficient_(2,0,1) = V[17];
  coefficient_(0,2,1) = V[18];
  coefficient_(0,0,3) = V[19];
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

static boost::format sizeformat("%|1$01d|%|2$01d|%|3$01d|%|4$03d|");
static boost::format coeffformat("%|1$+21.14E|");

//-----------------------------------------------------------------------
/// Write out the polynomial data as a TRE string. Note that you don't
/// generally use this directly, rather this is used by
/// RsmRationalPolynomial to create a RSMPCA TRE.
///
/// Note also that the TRE has a fixed precision which is less than
/// the machine precision. Writing a RsmPolynomial and then reading it
/// from a TRE does *not* in general give the exact same
/// RsmPolynomial, rather just one that is close.
//-----------------------------------------------------------------------

std::string RsmPolynomial::tre_string() const
{
  if(coefficient_.rows() - 1 > 5 ||
     coefficient_.cols() - 1 > 5 ||
     coefficient_.depth() - 1 > 5)
    throw Exception("The maximum polynomial order supported by the NITF TRE is 5");
  std::string s1 = str_check_size(sizeformat
				  % (coefficient_.rows() - 1)
				  % (coefficient_.cols() - 1)
				  % (coefficient_.depth() - 1)
				  % coefficient_.size(),
   				  1 + 1 + 1 + 3);
  // Seems odd, but for whatever reason this is stored in column major
  // form in a TRE rather than row major. So we need to flip the order here to
  // what looks "backwards"
  for(int k = 0; k < coefficient_.depth(); ++k)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int i = 0; i < coefficient_.rows(); ++i)
	s1 += str_check_size(coeffformat % coefficient_(i,j,k), 21);
  return s1;
}

//-----------------------------------------------------------------------
/// Read a TRE string to create a RsmPolynomial.
///
/// Not all the fields are saved in a TRE. In practice, this isn't a
/// problem because we only use the extra fields when initially
/// fitting the polynomial. Once we are reading it from a TRE, the
/// data should be fixed. Because we have to pick something to set
/// these values to, we just set is_denominator_ to false, and max_order_
/// to -1.
//-----------------------------------------------------------------------

void RsmPolynomial::read_tre_string(std::istream& In)
{
  is_denominator_ = false;
  max_order_ = -1;
  int nx = read_size<int>(In, 1);
  int ny = read_size<int>(In, 1);
  int nz = read_size<int>(In, 1);
  int total_size = read_size<int>(In, 3);
  if(total_size != (nx + 1) * (ny + 1) * (nz + 1))
    throw Exception("Total size should be the product of nx, ny, and nz");
  coefficient_.resize(nx + 1, ny + 1, nz + 1);
  fitted_coefficent_size = total_size;
  // Seems odd, but for whatever reason this is stored in column major
  // form in a TRE rather than row major. So we need to flip the order here to
  // what looks "backwards"
  for(int k = 0; k < coefficient_.depth(); ++k)
    for(int j = 0; j < coefficient_.cols(); ++j)
      for(int i = 0; i < coefficient_.rows(); ++i)
	coefficient_(i,j,k) = read_size<double>(In, 21);
}
