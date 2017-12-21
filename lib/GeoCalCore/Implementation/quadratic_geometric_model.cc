#include "quadratic_geometric_model.h"
#include "geocal_exception.h"
#include "geocal_gsl_fit.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void QuadraticGeometricModel::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GeometricModel)
    & GEOCAL_NVP(tp)
    & GEOCAL_NVP(trans)
    & GEOCAL_NVP(inv_trans)
    & GEOCAL_NVP(mag_ln)
    & GEOCAL_NVP(mag_smp)
    & GEOCAL_NVP(ft);
  boost::serialization::split_member(ar, *this, version);
}

template<class Archive>
void QuadraticGeometricModel::save(Archive & ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void QuadraticGeometricModel::load(Archive & ar, const unsigned int version)
{
  // Connect class to tp as observer
  if(tp)
    tp->add_observer(*this);
}

GEOCAL_IMPLEMENT(QuadraticGeometricModel);
#endif

// Minimum number of tiepoints before fitting a Quadratic function. We
// fit a linear model even if fit type is Quadratic if we have fewer
// than this number of points.
const int QuadraticGeometricModel::min_tp_for_quadratic = 12;

//-----------------------------------------------------------------------
/// Constructor. This creates an identity transformation.
//-----------------------------------------------------------------------

QuadraticGeometricModel::QuadraticGeometricModel
(FitType Ft,
 double Magnify_line, 
 double Magnify_sample)
  : trans(12), inv_trans(12), mag_ln(Magnify_line), 
    mag_smp(Magnify_sample), ft(Ft)
{
  trans = 1,0,0,0,0,0,0,1,0,0,0,0;
  inv_trans = 1,0,0,0,0,0,0,1,0,0,0,0;
}

//-----------------------------------------------------------------------
/// Constructor. This fits the set of Tp, or creates an identity
/// transformation if Tp is null.
//-----------------------------------------------------------------------

QuadraticGeometricModel::QuadraticGeometricModel
(const boost::shared_ptr<GeometricTiePoints>& Tp,
 FitType Ft,
 double Magnify_line, 
 double Magnify_sample)
  : tp(Tp), trans(12), inv_trans(12), mag_ln(Magnify_line), 
    mag_smp(Magnify_sample), ft(Ft)
{
  trans = 1,0,0,0,0,0,0,1,0,0,0,0;
  inv_trans = 1,0,0,0,0,0,0,1,0,0,0,0;
  if(tp) {
    tp->add_observer(*this);
    tp->notify_update();
  }
}

ImageCoordinate QuadraticGeometricModel::original_image_coordinate
(const ImageCoordinate& Resampled_ic) const
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

ImageCoordinate QuadraticGeometricModel::resampled_image_coordinate
(const ImageCoordinate& Original_ic) const
{
  double px = Original_ic.line;
  double py = Original_ic.sample;
  ImageCoordinate res;
  res.line = inv_trans(0)*px+inv_trans(1)*py+inv_trans(2)+inv_trans(3)*px*px+
    inv_trans(4)*py*py+inv_trans(5)*px*py;
  res.sample = inv_trans(6)*px+inv_trans(7)*py+inv_trans(8)+inv_trans(9)*px*px+
    inv_trans(10)*py*py+inv_trans(11)*px*py;
  res.line /= mag_ln;
  res.sample /= mag_smp;
  return res;
}

//-----------------------------------------------------------------------
/// Fit the transformation in the least squares sense to match the
/// given set of tiepoints.
//-----------------------------------------------------------------------

void QuadraticGeometricModel::fit_transformation(const GeometricTiePoints& Tp)
{
  Array<double, 2> x = Tp.x();
  Array<double, 2> y = Tp.y();
  fit_single(x, y, trans);
  fit_single(y, x, inv_trans);
}

//-----------------------------------------------------------------------
/// transformation and inverse transformation fit the same way, so
/// pull this out into a simple function we call twice.
//-----------------------------------------------------------------------

void QuadraticGeometricModel::fit_single(const blitz::Array<double, 2>& x,
		const blitz::Array<double, 2>& y,
		blitz::Array<double, 1>& tr)
{
  bool do_quadratic = (fit_type() == QUADRATIC);
  // If we have too few points, do a quadratic fit even if we've been
  // requested to do a quadratic fit.
  if(x.rows() < min_tp_for_quadratic)
    do_quadratic = false;
  Range ra = Range::all();
  Array<double, 1> px(x(ra, 0));
  Array<double, 1> py(x(ra, 1));
  Array<double, 2> mat(x.rows(), (do_quadratic ? 6 : 3));
  mat(ra, 0) = px;
  mat(ra, 1) = py;
  mat(ra, 2) = 1;
  if(do_quadratic) {
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
  if(do_quadratic)
    tr(Range(0, 5)) = coeff.blitz_array();
  else {
    tr(Range(0, 2)) = coeff.blitz_array();
    tr(Range(3, 5)) = 0;
  }
  gsl_fit(gsl_mat, gsl_smp, coeff, cov, chisq);
  if(do_quadratic)
    tr(Range(6, 11)) = coeff.blitz_array();
  else {
    tr(Range(6, 8)) = coeff.blitz_array();
    tr(Range(9, 11)) = 0;
  }
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
  Os << "  inverse_transformation: \n";
  opad << inverse_transformation() << "\n";
  opad.strict_sync();
}


