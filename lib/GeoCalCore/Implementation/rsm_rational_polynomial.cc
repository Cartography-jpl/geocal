#include "rsm_rational_polynomial.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"
#include "geocal_rpc.h"
#include "geocal_gsl_fit.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include <sstream>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmRationalPolynomial::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RsmBase)
    & GEOCAL_NVP_(line_offset)
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
    & GEOCAL_NVP_(sample_den)
    & GEOCAL_NVP_(nline_fit)
    & GEOCAL_NVP_(nsample_fit)
    & GEOCAL_NVP_(nheight_fit)
    & GEOCAL_NVP_(nsecond_pass_fit)
    & GEOCAL_NVP_(ignore_igc_error_in_fit);
  // Older version didn't have row_section_number_ or col_section_number_.
  if(version == 0) {
    ar & GEOCAL_NVP_(row_section_number)
      & GEOCAL_NVP_(col_section_number);
  }
  boost::serialization::split_member(ar, *this, version);
}

template<class Archive>
void RsmRationalPolynomial::save(Archive & ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void RsmRationalPolynomial::load(Archive & ar, const unsigned int version)
{
  // Older version didn't have row_section_number_ or col_section_number_.
  if(version == 0) {
    row_section_number_ = 1;
    col_section_number_ = 1;
  }
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
///
/// You can give the number of points to generate in the fit function
/// by giving the grid dimensions Nline_fit x Nsample_fit x
/// Nheight_fit. For Geodetic like coordinate systems this usually
/// gives pretty good coverage, but for something like
/// LocalRectangularCoordinate this kind of a grid doesn't cover the
/// full space. You can give the value for second pass through where
/// we make a X, Y, Z grid of points. This is often slower, but can be
/// necessary. If you set Nsecond_pass_fit to 0, then we skip this step
/// in fit. In fit we always ignore ImageGroundConnectionFailed
/// exceptions, but you can optionally ignore other errors in the Igc
/// by setting Ignore_igc_error_in_fit to true.
//-----------------------------------------------------------------------

RsmRationalPolynomial::RsmRationalPolynomial
(int Np_x, int Np_y, int Np_z, int Dp_x, int Dp_y,
 int Dp_z, int N_max_order, int D_max_order,int Nline_fit,
 int Nsample_fit, int Nheight_fit, int Nsecond_pass_fit,
 bool Ignore_igc_error_in_fit,
 int Row_section_number,
 int Col_section_number,
 const std::string& Image_identifier,
 const std::string& Rsm_support_data_edition
 )
 : RsmBase(Image_identifier, Rsm_support_data_edition),
     row_section_number_(Row_section_number),
     col_section_number_(Col_section_number),
     line_offset_(0),
     line_scale_(0),
     sample_offset_(0),
     sample_scale_(0),
     x_offset_(0),
     x_scale_(0),
     y_offset_(0),
     y_scale_(0),
     z_offset_(0),
     z_scale_(0),
     line_num_(Np_x, Np_y, Np_z, false, N_max_order),
     line_den_(Dp_x, Dp_y, Dp_z, true, D_max_order),
     sample_num_(Np_x, Np_y, Np_z, false, N_max_order),
     sample_den_(Dp_x, Dp_y, Dp_z, true, D_max_order),
     nline_fit_(Nline_fit),
     nsample_fit_(Nsample_fit),
     nheight_fit_(Nheight_fit),
     nsecond_pass_fit_(Nsecond_pass_fit),
     ignore_igc_error_in_fit_(Ignore_igc_error_in_fit)
{
}

//-----------------------------------------------------------------------
/// Apply the rational polynomial to the given X, Y, and Z value.
//-----------------------------------------------------------------------

ImageCoordinate RsmRationalPolynomial::image_coordinate
(double X, double Y, double Z) const
{
  double xs = (X - x_offset_) / x_scale_;
  double ys = (Y - y_offset_) / y_scale_;
  double zs = (Z - z_offset_) / z_scale_;
  double f1 = line_num_(xs,ys,zs);
  double f2 = line_den_(xs,ys,zs);
  double f3 = sample_num_(xs,ys,zs);
  double f4 = sample_den_(xs,ys,zs);
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
(const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
 const blitz::Array<double, 1>& Z) const
{
  Range ra = Range::all();
  Array<double, 1> xs(X.shape()), ys(Y.shape()), zs(Z.shape()),
    f1(X.shape()), f2(X.shape()), f3(X.shape()), f4(X.shape());
  xs = (X - x_offset_) / x_scale_;
  ys = (Y - y_offset_) / y_scale_;
  zs = (Z - z_offset_) / z_scale_;
  f1 = line_num_(xs,ys,zs);
  f2 = line_den_(xs,ys,zs);
  f3 = sample_num_(xs,ys,zs);
  f4 = sample_den_(xs,ys,zs);
  Array<double, 2> res(2, xs.rows());
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
(const blitz::Array<double, 2>& X, const blitz::Array<double, 2>& Y,
 const blitz::Array<double, 2>& Z) const
{
  Range ra = Range::all();
  Array<double, 2> xs(X.shape()), ys(Y.shape()), zs(Z.shape()),
    f1(X.shape()), f2(X.shape()), f3(X.shape()), f4(X.shape());
  xs = (X - x_offset_) / x_scale_;
  ys = (Y - y_offset_) / y_scale_;
  zs = (Z - z_offset_) / z_scale_;
  f1 = line_num_(xs,ys,zs);
  f2 = line_den_(xs,ys,zs);
  f3 = sample_num_(xs,ys,zs);
  f4 = sample_den_(xs,ys,zs);
  Array<double, 3> res(2, xs.rows(), xs.cols());
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
(const blitz::Array<double, 3>& X, const blitz::Array<double, 3>& Y,
 const blitz::Array<double, 3>& Z) const
{
  Range ra = Range::all();
  Array<double, 3> xs(X.shape()), ys(Y.shape()), zs(Z.shape()),
    f1(X.shape()), f2(X.shape()), f3(X.shape()), f4(X.shape());
  xs = (X - x_offset_) / x_scale_;
  ys = (Y - y_offset_) / y_scale_;
  zs = (Z - z_offset_) / z_scale_;
  f1 = line_num_(xs,ys,zs);
  f2 = line_den_(xs,ys,zs);
  f3 = sample_num_(xs,ys,zs);
  f4 = sample_den_(xs,ys,zs);
  Array<double, 4> res(2, xs.rows(), xs.cols(), xs.depth());
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

void RsmRationalPolynomial::fit_data
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
  Array<double, 2> ln_den_jac =
    line_den_.jacobian_fitted_coefficent(xs, ys, zs);
  Array<double, 2> smp_den_jac =
    sample_den_.jacobian_fitted_coefficent(xs, ys, zs);
  Array<double, 2> ln_num_jac =
    line_num_.jacobian_fitted_coefficent(xs, ys, zs);
  Array<double, 2> smp_num_jac =
    sample_num_.jacobian_fitted_coefficent(xs, ys, zs);
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
/// Generate a RsmRationalPolynomial that approximates the calculation
/// done by a ImageGroundConnection.  We determine that X, Y, and Z
/// range to use automatically to cover the range given by the
/// ImageGroundConnection.
///
/// To support sections, you can pass in a restricted number of
/// line/samples to fit over.
//-----------------------------------------------------------------------

void RsmRationalPolynomial::fit
(const ImageGroundConnection& Igc,
 const CoordinateConverter& Cconv,
 double Min_height, double Max_height,
 int Min_line, int Max_line, int Min_sample,
 int Max_sample)
{
  std::vector<double> line, sample, x, y, z;
  for(int i = 0; i < nline_fit_; ++i)
    for(int j = 0; j < nsample_fit_; ++j)
      for(int k = 0; k < nheight_fit_; ++k) {
	try {
	  double ln = Min_line + (Max_line - Min_line - 1.0) /
	    (nline_fit_ - 1.0) * i;
	  double smp = Min_sample + (Max_sample - Min_sample - 1.0) /
	    (nsample_fit_ - 1.0) * j;
	  double h = Min_height + (Max_height - Min_height) /
	    (nheight_fit_ - 1.0) * k;
	  boost::shared_ptr<GroundCoordinate> gc =
	    Igc.ground_coordinate_approx_height(ImageCoordinate(ln, smp), h);
	  line.push_back(ln);
	  sample.push_back(smp);
	  double xv, yv, zv;
	  Cconv.convert_to_coordinate(*gc, xv, yv, zv);
	  x.push_back(xv);
	  y.push_back(yv);
	  z.push_back(zv);
	} catch(const ImageGroundConnectionFailed&) {
	  // Ignore failures, just go to next point.
	} catch(...) {
	  if(!ignore_igc_error_in_fit_)
	    throw;
	}
      }
  if(line.size() == 0)
    throw Exception("Did not get any points for RsmRationalPolynomial fit");
  fit_offset_and_scale(Min_line, Max_line, Min_sample, Max_sample,
		       *std::min_element(x.begin(), x.end()),
		       *std::max_element(x.begin(), x.end()),
		       *std::min_element(y.begin(), y.end()),
		       *std::max_element(y.begin(), y.end()),
		       *std::min_element(z.begin(), z.end()),
		       *std::max_element(z.begin(), z.end()));
  // Go back through, and also make even spacing in x, y, and
  // z. Without this we can get a polynomial that acts badly at the
  // edge of the fitted range
  if(nsecond_pass_fit_ > 1) {
    for(int i = 0; i < nsecond_pass_fit_; ++i)
      for(int j = 0; j < nsecond_pass_fit_; ++j)
	for(int k = 0; k < nsecond_pass_fit_; ++k) {
	  double xv = min_x() + (max_x() - min_x()) / (nsecond_pass_fit_-1) * i;
	  double yv = min_y() + (max_y() - min_y()) / (nsecond_pass_fit_-1) * j;
	  double zv = min_z() + (max_z() - min_z()) / (nsecond_pass_fit_-1) * k;
	  boost::shared_ptr<GroundCoordinate> gc =
	    Cconv.convert_from_coordinate(xv, yv, zv);
	  ImageCoordinate ic;
	  bool success;
	  try {
	    Igc.image_coordinate_with_status(*gc, ic, success);
	    if(success) {
	      line.push_back(ic.line);
	      sample.push_back(ic.sample);
	      x.push_back(xv);
	      y.push_back(yv);
	      z.push_back(zv);
	    }
	  } catch(const ImageGroundConnectionFailed&) {
	    // Ignore failures, just go to next point.
	  } catch(...) {
	    if(!ignore_igc_error_in_fit_)
	      throw;
	  }
	}
  }
  fit_data(line, sample, x, y, z);
}

//-----------------------------------------------------------------------
/// This the jacobian of the line, sample with respect to X, Y, Z.
/// This is a 2x3 matrix.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmRationalPolynomial::image_coordinate_jacobian
(double X, double Y, double Z) const
{
  Range ra = Range::all();
  Array<double, 2> res(2, 3);
  double xs = (X - x_offset_) / x_scale_;
  double ys = (Y - y_offset_) / y_scale_;
  double zs = (Z - z_offset_) / z_scale_;
  double f1 = line_num_(xs,ys,zs);
  double f2 = line_den_(xs,ys,zs);
  double f3 = sample_num_(xs,ys,zs);
  double f4 = sample_den_(xs,ys,zs);
  Array<double, 1> f1_dv = line_num_.jacobian(xs,ys,zs);
  Array<double, 1> f2_dv = line_den_.jacobian(xs,ys,zs);
  Array<double, 1> f3_dv = sample_num_.jacobian(xs,ys,zs);
  Array<double, 1> f4_dv = sample_den_.jacobian(xs,ys,zs);
  Array<double, 1> v_scale(3);
  v_scale = 1/x_scale_, 1/y_scale_, 1/z_scale_;
  res(0,ra) = (f1_dv * f2 - f1 * f2_dv) / (f2 * f2) * line_scale_ * v_scale;
  res(1,ra) = (f3_dv * f4 - f3 * f4_dv) / (f4 * f4) * sample_scale_ * v_scale;
  return res;
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
     << "  DMax order:    " << line_den_.max_order() << "\n"
     << "  Nline fit:     " << nline_fit_ << "\n"
     << "  Nsample fit:   " << nsample_fit_ << "\n"
     << "  Nheight fit:   " << nheight_fit_ << "\n"
     << "  Nsecond pass fit: " << nsecond_pass_fit_ << "\n";
}

boost::format secformat("%|1$03d|%|2$03d|%|3$21s|%|4$21s|");
boost::format scaleformat("%|1$+21.14E|%|2$+21.14E|%|3$+21.14E|%|4$+21.14E|%|5$+21.14E|");

//-----------------------------------------------------------------------
/// Write to TRE string.
///
/// Note also that the TRE has a fixed precision which is less than
/// the machine precision. Writing a RsmRationalPolynomial and then
/// reading it from a TRE does *not* in general give the exact same
/// RsmRationalPolynomial, rather just one that is close.
///
/// Note that this is all the fields *except* the CETAG and CEL (the
/// front two). It is convenient to treat those special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

std::string RsmRationalPolynomial::tre_string() const
{
  std::string res = base_tre_string();
  // Don't fill in the row and column fit error
  std::string row_fit_error="";
  std::string col_fit_error="";
  res += str_check_size(secformat % row_section_number_ % col_section_number_
			% row_fit_error % col_fit_error,
			3 + 3 + 21 + 21);
  res += str_check_size(scaleformat % line_offset_ % sample_offset_ %
			x_offset_ % y_offset_ % z_offset_, 5 * 21);
  res += str_check_size(scaleformat % line_scale_ % sample_scale_ %
			x_scale_ % y_scale_ % z_scale_, 5 * 21);
  res += line_num_.tre_string();
  res += line_den_.tre_string();
  res += sample_num_.tre_string();
  res += sample_den_.tre_string();
  return res;
}

//-----------------------------------------------------------------------
/// Read a TRE string. Note that the TRE does not contain all the
/// fields we have in a RsmRationalPolynomial. However the fields that
/// aren't contained are ones used for fitting the RSM, so in practice
/// this doesn't matter. We just set the various fields to the default
/// values found in the constructor.
///
/// This should have all the TRE *except* for the front CETAG and CEL.
/// It is convenient to treat these fields as special. (We can
/// revisit this in the future if we need to).
//-----------------------------------------------------------------------

boost::shared_ptr<RsmRationalPolynomial>
RsmRationalPolynomial::read_tre_string(const std::string& Tre_in)
{
  boost::shared_ptr<RsmRationalPolynomial> res(new RsmRationalPolynomial);
  res->nline_fit_ = 20;
  res->nsample_fit_ = 20;
  res->nheight_fit_ = 20;
  res->nsecond_pass_fit_ = 20;
  res->ignore_igc_error_in_fit_ = false;
  std::stringstream in(Tre_in);
  res->base_read_tre_string(in);
  res->row_section_number_ = read_size<int>(in, 3);
  res->col_section_number_ = read_size<int>(in, 3);
  std::string trash = read_size<std::string>(in, 21);
  trash = read_size<std::string>(in, 21);
  res->line_offset_ = read_size<double>(in, 21);
  res->sample_offset_ = read_size<double>(in, 21);
  res->x_offset_ = read_size<double>(in, 21);
  res->y_offset_ = read_size<double>(in, 21);
  res->z_offset_ = read_size<double>(in, 21);
  res->line_scale_ = read_size<double>(in, 21);
  res->sample_scale_ = read_size<double>(in, 21);
  res->x_scale_ = read_size<double>(in, 21);
  res->y_scale_ = read_size<double>(in, 21);
  res->z_scale_ = read_size<double>(in, 21);
  res->line_num_.read_tre_string(in);
  res->line_den_.read_tre_string(in);
  res->sample_num_.read_tre_string(in);
  res->sample_den_.read_tre_string(in);
  check_end_of_stream(in);
  return res;
}
