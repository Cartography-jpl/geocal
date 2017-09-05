#include "rsm_low_order_polynomial.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"
#include "geocal_gsl_fit.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RsmLowOrderPolynomial::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RsmLowOrderPolynomial);
  ar & GEOCAL_NVP(pline)
    & GEOCAL_NVP(psamp);
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
/// This routine always ignores ImageGroundConnectionFailed
/// exceptions, and just skips to the next point. But if we are using
/// python code for the ImageGroundConnection we can't translate
/// errors to ImageGroundConnectionFailed (this is a limitation of
/// SWIG). So you can optionally specify Ignore_error as true, in
/// which case we ignore *all* exceptions and just skip to the next
/// point.
///
/// We normally look at all image points when generating the
/// RsmLowOrderPolynomial. You can optionally specify
/// Skip_masked_point to skip all image points that are masked.
///
/// To support sections, you can pass in a restricted number of
/// line/samples to fit over.
//-----------------------------------------------------------------------

void RsmLowOrderPolynomial::fit
(const ImageGroundConnection& Igc,
 const CoordinateConverter& Cconv,
 double Min_height, double Max_height,
 int Min_line, int Max_line, int Min_sample,
 int Max_sample,
 int Nline, int Nsample, int Nheight,
 bool Skip_masked_point,
 bool Ignore_error)
{
  std::vector<double> line, sample, x, y, z;
  for(int i = 0; i < Nline; ++i)
    for(int j = 0; j < Nsample; ++j)
      for(int k = 0; k < Nheight; ++k) {
	try {
	  double ln = Min_line + (Max_line - Min_line) / (Nline - 1) * i;
	  double smp = Min_sample + (Max_sample - Min_sample) / (Nsample - 1) * j;
	  double h = Min_height + (Max_height - Min_height) / (Nheight - 1) * k;
	  if(Skip_masked_point && Igc.image_mask()->mask(ln, smp))
	    continue;
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
	  if(!Ignore_error)
	    throw;
	}
      }
  if(line.size() == 0)
    throw Exception("Did not get any points for RsmLowOrderPolynomial fit");
  fit_data(line, sample, x, y, z);
}
