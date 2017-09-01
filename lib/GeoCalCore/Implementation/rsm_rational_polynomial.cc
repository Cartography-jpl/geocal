#include "rsm_rational_polynomial.h"
#include "geocal_serialize_support.h"
#include "geocal_rpc.h"
#include "geocal_gsl_fit.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmRationalPolynomial::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmRationalPolynomial);
  ar & GEOCAL_NVP_(line_offset)
    & GEOCAL_NVP_(line_scale)
    & GEOCAL_NVP_(sample_offset)
    & GEOCAL_NVP_(sample_scale)    
    & GEOCAL_NVP_(x_offset)
    & GEOCAL_NVP_(x_scale)    
    & GEOCAL_NVP_(y_offset)
    & GEOCAL_NVP_(y_scale)    
    & GEOCAL_NVP_(z_offset)
    & GEOCAL_NVP_(z_scale)
    & GEOCAL_NVP_(line_num)
    & GEOCAL_NVP_(line_den)
    & GEOCAL_NVP_(sample_num)
    & GEOCAL_NVP_(sample_den);
}

GEOCAL_IMPLEMENT(RsmRationalPolynomial);
#endif

//-----------------------------------------------------------------------
/// Constructor. You indicated the order of the polynomial in each
/// dimension for both the numerator and denominator. This particular
/// constructor uses the same dimensions for both the line and the
/// sample, there is no actual requirement in a Rsm that this be the
/// case (although it is pretty common).
///
/// You can limit the over all order of cross terms by supplying a
/// max_order for the numerator, denominator, or both. This holds all 
/// cross terms with a total order > max to 0. The default is to not
/// restrict the cross terms.
//-----------------------------------------------------------------------

RsmRationalPolynomial::RsmRationalPolynomial
(int Np_x, int Np_y, int Np_z, int Dp_x, int Dp_y,
 int Dp_z, int N_max_order, int D_max_order)
  : line_num_(Np_x, Np_y, Np_z, false, N_max_order),
    line_den_(Dp_x, Dp_y, Dp_z, true, D_max_order),
    sample_num_(Np_x, Np_y, Np_z, false, N_max_order),
    sample_den_(Dp_x, Dp_y, Dp_z, true, D_max_order)
{
}

//-----------------------------------------------------------------------
/// Apply the rational polynomial to the given X, Y, and Z value.
//-----------------------------------------------------------------------

ImageCoordinate RsmRationalPolynomial::image_coordinate
(double Xin, double Yin, double Zin) const
{
  double x = (Xin - x_offset_) / x_scale_;
  double y = (Yin - y_offset_) / y_scale_;
  double z = (Zin - z_offset_) / z_scale_;
  double f1 = line_num_(x,y,z);
  double f2 = line_den_(x,y,z);
  double f3 = sample_num_(x,y,z);
  double f4 = sample_den_(x,y,z);
  return ImageCoordinate(f1 / f2 * line_scale_ + line_offset_,
			 f3 / f4 * sample_scale_ + sample_offset_);
}

//-----------------------------------------------------------------------
/// Apply the rational polynomial to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmRationalPolynomial::image_coordinate
(const blitz::Array<double, 1>& Xin, const blitz::Array<double, 1>& Yin,
 const blitz::Array<double, 1>& Zin) const
{
  Range ra = Range::all();
  Array<double, 1> x(Xin.shape()), y(Yin.shape()), z(Zin.shape()),
    f1(Xin.shape()), f2(Xin.shape()), f3(Xin.shape()), f4(Xin.shape());
  x = (Xin - x_offset_) / x_scale_;
  y = (Yin - y_offset_) / y_scale_;
  z = (Zin - z_offset_) / z_scale_;
  f1 = line_num_(x,y,z);
  f2 = line_den_(x,y,z);
  f3 = sample_num_(x,y,z);
  f4 = sample_den_(x,y,z);
  Array<double, 2> res(2, x.rows());
  res(0, ra) = f1 / f2 * line_scale_ + line_offset_;
  res(1, ra) = f3 / f4 * sample_scale_ + sample_offset_;
  return res;
}

//-----------------------------------------------------------------------
/// Apply the rational polynomial to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 3> RsmRationalPolynomial::image_coordinate
(const blitz::Array<double, 2>& Xin, const blitz::Array<double, 2>& Yin,
 const blitz::Array<double, 2>& Zin) const
{
  Range ra = Range::all();
  Array<double, 2> x(Xin.shape()), y(Yin.shape()), z(Zin.shape()),
    f1(Xin.shape()), f2(Xin.shape()), f3(Xin.shape()), f4(Xin.shape());
  x = (Xin - x_offset_) / x_scale_;
  y = (Yin - y_offset_) / y_scale_;
  z = (Zin - z_offset_) / z_scale_;
  f1 = line_num_(x,y,z);
  f2 = line_den_(x,y,z);
  f3 = sample_num_(x,y,z);
  f4 = sample_den_(x,y,z);
  Array<double, 3> res(2, x.rows(), x.cols());
  res(0, ra, ra) = f1 / f2 * line_scale_ + line_offset_;
  res(1, ra, ra) = f3 / f4 * sample_scale_ + sample_offset_;
  return res;
}

//-----------------------------------------------------------------------
/// Apply the rational polynomial to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 4> RsmRationalPolynomial::image_coordinate
(const blitz::Array<double, 3>& Xin, const blitz::Array<double, 3>& Yin,
 const blitz::Array<double, 3>& Zin) const
{
  Range ra = Range::all();
  Array<double, 3> x(Xin.shape()), y(Yin.shape()), z(Zin.shape()),
    f1(Xin.shape()), f2(Xin.shape()), f3(Xin.shape()), f4(Xin.shape());
  x = (Xin - x_offset_) / x_scale_;
  y = (Yin - y_offset_) / y_scale_;
  z = (Zin - z_offset_) / z_scale_;
  f1 = line_num_(x,y,z);
  f2 = line_den_(x,y,z);
  f3 = sample_num_(x,y,z);
  f4 = sample_den_(x,y,z);
  Array<double, 4> res(2, x.rows(), x.cols(), x.depth());
  res(0, ra, ra, ra) = f1 / f2 * line_scale_ + line_offset_;
  res(1, ra, ra, ra) = f3 / f4 * sample_scale_ + sample_offset_;
  return res;
}

//-----------------------------------------------------------------------
/// Set a RsmPolynomial to match from a Rpc.
/// This is really only useful for testing, comparing against our RPC code
//-----------------------------------------------------------------------

void RsmRationalPolynomial::set_rpc_coeff(const Rpc& R)
{
  Rpc rpc = R.rpc_type_b();
  z_offset_ = rpc.height_offset;
  z_scale_ = rpc.height_scale;
  y_offset_ = rpc.latitude_offset;
  y_scale_ = rpc.latitude_scale;
  x_offset_ = rpc.longitude_offset;
  x_scale_ = rpc.longitude_scale;
  line_offset_ = rpc.line_offset;
  line_scale_ = rpc.line_scale;
  sample_offset_ = rpc.sample_offset;
  sample_scale_ = rpc.sample_scale;

  line_num_.set_rpc_coeff(rpc.line_numerator);
  line_den_.set_rpc_coeff(rpc.line_denominator);
  sample_num_.set_rpc_coeff(rpc.sample_numerator);
  sample_den_.set_rpc_coeff(rpc.sample_denominator);
}

//-----------------------------------------------------------------------
/// Find the scale and offset values to cover the given range of data.
//-----------------------------------------------------------------------

void RsmRationalPolynomial::fit_offset_and_scale
(double Min_line, double Max_line, double Min_sample,
 double Max_sample, double Min_x, double Max_x,
 double Min_y, double Max_y, double Min_z,
 double Max_z)
{
  line_offset_ = (Max_line + Min_line) / 2.0;
  line_scale_ = (Max_line - Min_line) / 2.0;
  sample_offset_ = (Max_sample + Min_sample) / 2.0;
  sample_scale_ = (Max_sample - Min_sample) / 2.0;
  x_offset_ = (Max_x + Min_x) / 2.0;
  x_scale_ = (Max_x - Min_x) / 2.0;
  y_offset_ = (Max_y + Min_y) / 2.0;
  y_scale_ = (Max_y - Min_y) / 2.0;
  z_offset_ = (Max_z + Min_z) / 2.0;
  z_scale_ = (Max_z - Min_z) / 2.0;
}

//-----------------------------------------------------------------------
/// Adjust the parameters of line and sample numerator and denominator
/// to fit the given data. Note that this uses the existing scale and
/// offset values, so you should make sure to set that first (e.g.,
/// call fit_offset_and_scale).
//-----------------------------------------------------------------------

void RsmRationalPolynomial::fit
(const std::vector<double>& Line,
 const std::vector<double>& Sample,
 const std::vector<double>& X,
 const std::vector<double>& Y,
 const std::vector<double>& Z)
{
  firstIndex i1; secondIndex i2;
  Range ra = Range::all();
  if(Line.size() != Sample.size() ||
     Line.size() != X.size() ||
     Line.size() != Y.size() ||
     Line.size() != Z.size())
    throw Exception("Line, Sample, X, Y and Z all need to be the same size");
  Array<double, 1> ln_lhs(Line.size());
  Array<double, 1> smp_lhs(Line.size());
  Array<double, 1> xs(Line.size());
  Array<double, 1> ys(Line.size());
  Array<double, 1> zs(Line.size());
  for(int i = 0; i < (int) Line.size(); ++i) {
    xs(i) = (X[i] - x_offset_) / x_scale_;
    ys(i) = (Y[i] - y_offset_) / y_scale_;
    zs(i) = (Z[i] - z_offset_) / z_scale_;
    ln_lhs(i) = (Line[i] - line_offset_) / line_scale_;
    smp_lhs(i) = (Sample[i] - sample_offset_) / sample_scale_;
  }
  Array<double, 2> ln_den_jac = line_den_.jacobian(xs, ys, zs);
  Array<double, 2> smp_den_jac = sample_den_.jacobian(xs, ys, zs);
  Array<double, 2> ln_num_jac = line_num_.jacobian(xs, ys, zs);
  Array<double, 2> smp_num_jac = sample_num_.jacobian(xs, ys, zs);
  Array<double, 2> ln_jac(ln_den_jac.rows(), ln_den_jac.cols() +
			  ln_num_jac.cols());
  Array<double, 2> smp_jac(smp_den_jac.rows(), smp_den_jac.cols() +
			   smp_num_jac.cols());
  ln_jac(ra, Range(0, ln_num_jac.cols() - 1)) = ln_num_jac;
  ln_jac(ra, Range(ln_num_jac.cols(), Range::toEnd)) =
    -ln_lhs(i1) * ln_den_jac;
  smp_jac(ra, Range(0, smp_num_jac.cols() - 1)) = smp_num_jac;
  smp_jac(ra, Range(smp_num_jac.cols(), Range::toEnd)) =
    -smp_lhs(i1) * smp_den_jac;
  GslMatrix cov;
  GslVector ln_c;
  GslVector smp_c;
  double chisq;
  gsl_fit(ln_jac, ln_lhs, ln_c, cov, chisq);
  gsl_fit(smp_jac, smp_lhs, smp_c, cov, chisq);
  line_num_.fitted_coefficent(ln_c.blitz_array()(Range(0,
						       ln_num_jac.cols()-1)));
  line_den_.fitted_coefficent(ln_c.blitz_array()(Range(ln_num_jac.cols(),
						       Range::toEnd)));
  sample_num_.fitted_coefficent(smp_c.blitz_array()(Range(0,
					       smp_num_jac.cols()-1)));
  sample_den_.fitted_coefficent(smp_c.blitz_array()(Range(smp_num_jac.cols(),
					       Range::toEnd)));
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void RsmRationalPolynomial::print(std::ostream& Os) const
{
  Os << "RsmRationalPolynomial:\n"
     << "  Line offset:   " << line_offset_ << "\n"
     << "  Line scale:    " << line_scale_ << "\n"
     << "  Sample offset: " << sample_offset_ << "\n"
     << "  Sample scale:  " << sample_scale_ << "\n"
     << "  X offset:      " << x_offset_ << "\n"
     << "  X scale:       " << x_scale_ << "\n"
     << "  Y offset:      " << y_offset_ << "\n"
     << "  Y scale:       " << y_scale_ << "\n"
     << "  Z offset:      " << z_offset_ << "\n"
     << "  Z scale:       " << z_scale_ << "\n"
     << "  Np x:          " << line_num_.coefficient().rows()-1 << "\n"
     << "  Np y:          " << line_num_.coefficient().cols()-1 << "\n"
     << "  Np z:          " << line_num_.coefficient().depth()-1 << "\n"
     << "  NMax order:    " << line_num_.max_order() << "\n"
     << "  Dp x:          " << line_den_.coefficient().rows()-1 << "\n"
     << "  Dp y:          " << line_den_.coefficient().cols()-1 << "\n"
     << "  Dp z:          " << line_den_.coefficient().depth()-1 << "\n"
     << "  DMax order:    " << line_den_.max_order() << "\n";
}
