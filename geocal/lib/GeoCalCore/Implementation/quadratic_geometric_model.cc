#include "quadratic_geometric_model.h"
#include "geocal_exception.h"
#include "geocal_gsl_fit.h"
#include "ostream_pad.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor. This creates an identity transformation.
//-----------------------------------------------------------------------

QuadraticGeometricModel::QuadraticGeometricModel
(FitType Ft,
 double Magnify_line, 
 double Magnify_sample)
  : trans(12), mag_ln(Magnify_line), mag_smp(Magnify_sample), ft(Ft)
{
  trans = 1,0,0,0,0,0,0,1,0,0,0,0;
}

//-----------------------------------------------------------------------
/// Constructor. The transform gives the coefficients of the quadratic
/// transform, it should be length 12. The transformation is:
/// x = trans(0)*px+trans(1)*py+trans(2)+trans(3)*px*px+
///     trans(4)*py*py+trans(5)*px*py
/// y = trans(6)*px+trans(7)*py+trans(8)+trans(9)*px*px+
///     trans(10)*py*py+trans(11)*px*py
//-----------------------------------------------------------------------

QuadraticGeometricModel::QuadraticGeometricModel
( const blitz::Array<double, 1>& Transformation,
  FitType Ft,
  double Magnify_line, 
  double Magnify_sample
)
  : trans(Transformation.copy()), mag_ln(Magnify_line), mag_smp(Magnify_sample),
    ft(Ft)
{
  if(trans.rows() != 12)
    throw Exception("Transformation needs to be size 12");
}

ImageCoordinate QuadraticGeometricModel::image_coordinate(
const ImageCoordinate& Resampled_ic) const
{
  double px = Resampled_ic.line * mag_ln;
  double py = Resampled_ic.sample * mag_smp;
  ImageCoordinate res;
  res.line = trans(0)*px+trans(1)*py+trans(2)+trans(3)*px*px+
    trans(4)*py*py+trans(5)*px*py;
  res.sample = trans(6)*px+trans(7)*py+trans(8)+trans(9)*px*px+
    trans(10)*py*py+trans(11)*px*py;
  return res;
}

//-----------------------------------------------------------------------
/// Fit the transformation in the least squares sense to match the
/// given set of tiepoints.
//-----------------------------------------------------------------------

void QuadraticGeometricModel::fit_transformation(const GeometricTiePoints& Tp)
{
  Range ra = Range::all();
  Array<double, 2> x = Tp.x();
  Array<double, 2> y = Tp.y();
  Array<double, 1> px(x(ra, 0));
  Array<double, 1> py(x(ra, 1));
  Array<double, 2> mat(x.rows(), (fit_type() == LINEAR ? 3 : 6));
  mat(ra, 0) = px;
  mat(ra, 1) = py;
  mat(ra, 2) = 1;
  if(fit_type() == QUADRATIC) {
    mat(ra, 3) = px * px;
    mat(ra, 4) = py*py;
    mat(ra, 5) = py*px;
  }
  GslVector gsl_ln(y(ra,0));
  GslVector gsl_smp(y(ra,1));
  GslMatrix gsl_mat(mat);
  GslVector coeff;
  double chisq;
  GslMatrix cov;
  gsl_fit(gsl_mat, gsl_ln, coeff, cov, chisq);
  if(fit_type() == LINEAR) {
    trans(Range(0, 2)) = coeff.blitz_array();
    trans(Range(3, 5)) = 0;
  } else
    trans(Range(0, 5)) = coeff.blitz_array();
  gsl_fit(gsl_mat, gsl_smp, coeff, cov, chisq);
  if(fit_type() == LINEAR) {
    trans(Range(6, 8)) = coeff.blitz_array();
    trans(Range(9, 11)) = 0;
  } else
    trans(Range(6, 11)) = coeff.blitz_array();
}

// Print to stream.
void QuadraticGeometricModel::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "QuadraticGeometricModel:\n"
     << "  fit_type: " << (fit_type() == LINEAR ? "LINEAR" : "QUADRATIC") 
     << "\n"
     << "  magnify_line: " << magnify_line() << "\n"
     << "  magnify_sample: " << magnify_sample() << "\n"
     << "  transformation: \n";
  opad << transformation() << "\n";
  opad.strict_sync();
}


