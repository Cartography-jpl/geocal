#include "quadratic_geometric_model.h"
#include "geocal_exception.h"
#include "ostream_pad.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor. This creates an identity transformation.
//-----------------------------------------------------------------------

QuadraticGeometricModel::QuadraticGeometricModel
(double Magnify_line, 
 double Magnify_sample)
  : trans(12), mag_ln(Magnify_line), mag_smp(Magnify_sample)
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
  double Magnify_line, 
  double Magnify_sample
)
  : trans(Transformation.copy()), mag_ln(Magnify_line), mag_smp(Magnify_sample)
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

// Print to stream.
void QuadraticGeometricModel::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "QuadraticGeometricModel:\n"
     << "  magnify_line: " << magnify_line() << "\n"
     << "  magnify_sample: " << magnify_sample() << "\n"
     << "  transformation: \n";
  opad << transformation() << "\n";
  opad.strict_sync();
}


