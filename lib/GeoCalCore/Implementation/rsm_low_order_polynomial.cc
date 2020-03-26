#include "rsm_low_order_polynomial.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"
#include "geocal_gsl_fit.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmLowOrderPolynomial::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmLowOrderPolynomial);
  ar & GEOCAL_NVP(pline)
    & GEOCAL_NVP(psamp)
    & GEOCAL_NVP_(min_line)
    & GEOCAL_NVP_(max_line)
    & GEOCAL_NVP_(min_sample)
    & GEOCAL_NVP_(max_sample)
    & GEOCAL_NVP_(nline_fit)
    & GEOCAL_NVP_(nsample_fit)
    & GEOCAL_NVP_(nheight_fit)
    & GEOCAL_NVP_(ignore_igc_error_in_fit);
}

GEOCAL_IMPLEMENT(RsmLowOrderPolynomial);
#endif

//-----------------------------------------------------------------------
/// Apply the rational polynomial to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RsmLowOrderPolynomial::image_coordinate
(const blitz::Array<double, 1>& X,
 const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const
{
  Range ra = Range::all();
  if(X.rows() != Y.rows() || X.rows() != Z.rows())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 2> res(2, X.rows());
  res(0, ra)  =  pline[0] +
    X * (pline[1] + pline[4] * X + pline[5] * Y + pline[6] * Z) +
    Y * (pline[2] + pline[7] * Y + pline[8] * Z) +
    Z * (pline[3] + pline[9] * Z);
  res(1, ra) =  psamp[0] +
    X * (psamp[1] + psamp[4] * X + psamp[5] * Y + psamp[6] * Z) +
    Y * (psamp[2] + psamp[7] * Y + psamp[8] * Z) +
    Z * (psamp[3] + psamp[9] * Z);
  return res;
}

//-----------------------------------------------------------------------
/// Apply the rational polynomial to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 3> RsmLowOrderPolynomial::image_coordinate
(const blitz::Array<double, 2>& X,
 const blitz::Array<double, 2>& Y, const blitz::Array<double, 2>& Z) const
{
  Range ra = Range::all();
  if(X.rows() != Y.rows() || X.rows() != Z.rows() ||
     X.cols() != Y.cols() || X.cols() != Z.cols())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 3> res(2, X.rows(), X.cols());
  res(0, ra, ra)  =  pline[0] +
    X * (pline[1] + pline[4] * X + pline[5] * Y + pline[6] * Z) +
    Y * (pline[2] + pline[7] * Y + pline[8] * Z) +
    Z * (pline[3] + pline[9] * Z);
  res(1, ra, ra) =  psamp[0] +
    X * (psamp[1] + psamp[4] * X + psamp[5] * Y + psamp[6] * Z) +
    Y * (psamp[2] + psamp[7] * Y + psamp[8] * Z) +
    Z * (psamp[3] + psamp[9] * Z);
  return res;
}

//-----------------------------------------------------------------------
/// Apply the rational polynomial to the given X, Y, and Z value. This
/// converts a whole array of points at once. The results have the
/// first dimension of size 2, the first value is the line and the
/// second the sample.
//-----------------------------------------------------------------------

blitz::Array<double, 4> RsmLowOrderPolynomial::image_coordinate
(const blitz::Array<double, 3>& X,
 const blitz::Array<double, 3>& Y, const blitz::Array<double, 3>& Z) const
{
  Range ra = Range::all();
  if(X.rows() != Y.rows() || X.rows() != Z.rows() ||
     X.cols() != Y.cols() || X.cols() != Z.cols() ||
     X.depth() != Y.depth() || X.depth() != Z.depth())
    throw Exception("X, Y, and Z need to be the same size");
  Array<double, 4> res(2, X.rows(), X.cols(), X.depth());
  res(0, ra, ra, ra)  =  pline[0] +
    X * (pline[1] + pline[4] * X + pline[5] * Y + pline[6] * Z) +
    Y * (pline[2] + pline[7] * Y + pline[8] * Z) +
    Z * (pline[3] + pline[9] * Z);
  res(1, ra, ra, ra) =  psamp[0] +
    X * (psamp[1] + psamp[4] * X + psamp[5] * Y + psamp[6] * Z) +
    Y * (psamp[2] + psamp[7] * Y + psamp[8] * Z) +
    Z * (psamp[3] + psamp[9] * Z);
  return res;
}

//-----------------------------------------------------------------------
/// Adjust the parameters of line and sample numerator and denominator
/// to fit the given data. Note that this uses the existing scale and
/// offset values, so you should make sure to set that first (e.g.,
/// call fit_offset_and_scale).
//-----------------------------------------------------------------------

void RsmLowOrderPolynomial::fit_data
(const std::vector<double>& Line,
 const std::vector<double>& Sample,
 const std::vector<double>& X,
 const std::vector<double>& Y,
 const std::vector<double>& Z)
{
  if(Line.size() != Sample.size() ||
     Line.size() != X.size() ||
     Line.size() != Y.size() ||
     Line.size() != Z.size())
    throw Exception("Line, Sample, X, Y and Z all need to be the same size");
  Array<double, 2> j(X.size(), 10);
  Array<double, 1> ln(X.size());
  Array<double, 1> smp(X.size());
  for(int i = 0; i < (int) X.size(); ++i) {
    ln(i) = Line[i];
    smp(i) = Sample[i];
    j(i,0) = 1;
    j(i,1) = X[i];
    j(i,2) = Y[i];
    j(i,3) = Z[i];
    j(i,4) = X[i] * X[i];
    j(i,5) = X[i] * Y[i];
    j(i,6) = X[i] * Z[i];
    j(i,7) = Y[i] * Y[i];
    j(i,8) = Y[i] * Z[i];
    j(i,9) = Z[i] * Z[i];
  }
  GslMatrix cov;
  GslVector ln_c;
  GslVector smp_c;
  double chisq;
  gsl_fit(j, ln, ln_c, cov, chisq);
  gsl_fit(j, smp, smp_c, cov, chisq);
  for(int i = 0; i < 10; ++i) {
    pline[i] = ln_c.blitz_array()(i);
    psamp[i] = smp_c.blitz_array()(i);
  }
}

//-----------------------------------------------------------------------
/// Generate a RsmLowOrderPolynomial that approximates the calculation
/// done by a ImageGroundConnection.
///
/// To support sections, you can pass in a restricted number of
/// line/samples to fit over.
//-----------------------------------------------------------------------

void RsmLowOrderPolynomial::fit
(const ImageGroundConnection& Igc,
 const CoordinateConverter& Cconv,
 double Min_height, double Max_height,
 int Min_line, int Max_line, int Min_sample,
 int Max_sample)
{
  min_line_ = Min_line;
  max_line_ = Max_line;
  min_sample_ = Min_sample;
  max_sample_ = Max_sample;
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
    throw Exception("Did not get any points for RsmLowOrderPolynomial fit");
  fit_data(line, sample, x, y, z);
}

static boost::format coeffformat("%|1$+21.14E|");

//-----------------------------------------------------------------------
/// Write out the polynomial data as a TRE string. Note that you don't
/// generally use this directly, rather this is used by
/// RsmMultiSection to create a RSMPIA or RSMGIA TRE.
///
/// Note also that the TRE has a fixed precision which is less than
/// the machine precision. Writing a RsmLowOrderPolynomial and then
/// reading it from a TRE does *not* in general give the exact same
/// RsmLowOrderPolynomial, rather just one that is close.
//-----------------------------------------------------------------------

std::string RsmLowOrderPolynomial::tre_string() const
{
  std::string res ="";
  for(int i = 0; i < 10; ++i)
    res += str_check_size(coeffformat % pline[i], 21);
  for(int i = 0; i < 10; ++i)
    res += str_check_size(coeffformat % psamp[i], 21);
  return res;
}

//-----------------------------------------------------------------------
/// Read a TRE string to create a RsmLowOrderPolynomial
///
/// Not all the fields are saved in a TRE. In practice, this isn't a
/// problem because we only use the extra fields when initially
/// fitting the polynomial. Once we are reading it from a TRE, the
/// data should be fixed. Because we have to pick something to set
/// these values to, we set min_line, max_line, min_sample, max_sample
/// all to 0 (the RsmMultiSection will fill this in with other TRE
/// information), and nline_fit, nsample_fit, nheight_fit,
/// ignore_igc_error_in_fit to the default values of
/// the constructor 
//-----------------------------------------------------------------------

void RsmLowOrderPolynomial::read_tre_string(std::istream& In)
{
  min_line_ = 0;
  max_line_ = 0;
  min_sample_ = 0;
  max_sample_ = 0;
  nline_fit_ = 10;
  nsample_fit_ = 10;
  nheight_fit_ = 10;
  ignore_igc_error_in_fit_ = false;
  for(int i = 0; i < 10; ++i)
    pline[i] = read_size<double>(In, 21);
  for(int i = 0; i < 10; ++i)
    psamp[i] = read_size<double>(In, 21);
}
