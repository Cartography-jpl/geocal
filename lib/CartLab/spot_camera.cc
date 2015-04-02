#include "spot_camera.h"
#include "geocal_exception.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void SpotCamera::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Camera);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(Camera, WithParameter);
  GEOCAL_BASE(SpotCamera, Camera);
  ar & GEOCAL_NVP_(psi_x)
    & GEOCAL_NVP_(psi_y);
}

GEOCAL_IMPLEMENT(SpotCamera);
#endif

//-----------------------------------------------------------------------
/// Create a spot camera with the given PSI angles. Psi_x and Psi_y
/// need to be equal in size, and have at least two values.
///
/// The angles should be in radians.
///
/// We require that Psi_y be strictly increasing.
//-----------------------------------------------------------------------

SpotCamera::SpotCamera(const std::vector<double>& Psi_x, 
		       const std::vector<double>& Psi_y)
  : psi_x_(Psi_x),
    psi_y_(Psi_y)
{
  range_min_check((int) psi_x_.size(), 2);
  if(psi_x_.size() != psi_y_.size())
    throw Exception("psi_x and psi_y need to be the same size");
  for(int i = 1; i < number_sample(0); ++i)
    if(psi_y_[i] <= psi_y_[i - 1])
      throw Exception("psi_y must be strictly increasing");
}

//-----------------------------------------------------------------------
/// This is similar to frame_coordinate, except it only calculates the
/// line coordinate. For some camera models, it is more expensive to
/// calculate the sample number than the line (e.g., MISR camera needs
/// to use the pixel-theta measurements). For functions that only need
/// the line information, it is wasteful to do the full calculation. 
///
/// The default version of this function just calls frame_coordinate
/// and returns the line portion, but a derived class can override
/// this default with a faster way of calculation the line
/// information. 
//-----------------------------------------------------------------------

FrameCoordinate SpotCamera::frame_coordinate(const ScLookVector& Sl,
					     int Band) const
{
  range_check(Band, 0, number_band());
  // Pick a value to use for outside the range of the data.
  double psi_delta = psi_y_[1] - psi_y_[0];
  double psi_xdelta = psi_x_[1] - psi_x_[0];
  double xang = atan(-Sl.look_vector[1] / Sl.look_vector[2]);
  double yang = atan(Sl.look_vector[0] / Sl.look_vector[2]);
  FrameCoordinate fres;
  if(yang < psi_y_[0]) {
    fres.sample = (yang - psi_y_[0]) / psi_delta;
    fres.line = (xang - (psi_x_[0] + psi_xdelta * fres.sample)) / psi_delta;
  } else if(yang >= psi_y_[number_sample(Band) - 1]) {
    fres.sample = number_sample(Band) - 1 + 
      (yang - psi_y_[number_sample(Band) - 1]) / psi_delta;
    fres.line = (xang - 
		 (psi_x_[number_sample(Band) - 1] + psi_xdelta * 
		  (fres.sample - (number_sample(Band) - 1)))) / psi_delta;
  } else {
    int i = (int) (upper_bound(psi_y_.begin(), psi_y_.end(), yang) - 
		   psi_y_.begin()) - 1;
    fres.sample = (yang - psi_y_[i]) / (psi_y_[i + 1] - psi_y_[i]) + i;
    fres.line = (xang - 
		 (psi_x_[i] + (psi_x_[i + 1] - psi_x_[i]) * (fres.sample - i))) / 
      psi_delta;
  }
  return fres;
}

FrameCoordinateWithDerivative SpotCamera::frame_coordinate_with_derivative
(const ScLookVectorWithDerivative& Sl,
 int Band) const
{
  range_check(Band, 0, number_band());
  // Pick a value to use for outside the range of the data.
  double psi_delta = psi_y_[1] - psi_y_[0];
  double psi_xdelta = psi_x_[1] - psi_x_[0];
  AutoDerivative<double> xang = 
    std::atan(-Sl.look_vector[1] / Sl.look_vector[2]);
  AutoDerivative<double> yang = 
    std::atan(Sl.look_vector[0] / Sl.look_vector[2]);
  FrameCoordinateWithDerivative fres;
  if(yang < psi_y_[0]) {
    fres.sample = (yang - psi_y_[0]) / psi_delta;
    fres.line = (xang - (psi_x_[0] + psi_xdelta * fres.sample)) / psi_delta;
  } else if(yang >= psi_y_[number_sample(Band) - 1]) {
    fres.sample = number_sample(Band) - 1 + 
      (yang - psi_y_[number_sample(Band) - 1]) / psi_delta;
    fres.line = (xang - 
		 (psi_x_[number_sample(Band) - 1] + psi_xdelta * 
		  (fres.sample - (number_sample(Band) - 1)))) / psi_delta;
  } else {
    int i = (int) (upper_bound(psi_y_.begin(), psi_y_.end(), yang) - 
		   psi_y_.begin()) - 1;
    fres.sample = (yang - psi_y_[i]) / (psi_y_[i + 1] - psi_y_[i]) + i;
    fres.line = (xang - 
		 (psi_x_[i] + (psi_x_[i + 1] - psi_x_[i]) * (fres.sample - i))) / 
      psi_delta;
  }
  return fres;
}

//-----------------------------------------------------------------------
/// Convert from FrameCoordinate to ScLookVector. It is perfectly
/// allowable for F.line to be outside the range (0, number_line(band)
/// - 1) or for F.sample to be outside the range (0,
/// number_sample(band) - 1). The conversion will just act as if the
/// camera has infinite extent.
//-----------------------------------------------------------------------

ScLookVector SpotCamera::sc_look_vector(const FrameCoordinate& F, 
					int Band) const
{
  range_check(Band, 0, number_band());
  // Pick a value to use for outside the range of the data.
  double psi_delta = psi_y_[1] - psi_y_[0];
  double psi_xdelta = psi_x_[1] - psi_x_[0];
  double yang, xang;
  if(F.sample < 0) {
    yang = psi_y_[0] + psi_delta * F.sample;
    xang = psi_x_[0] + psi_xdelta * F.sample;
  } else if(F.sample >= number_sample(Band) - 1) {
    yang = psi_y_[number_sample(Band) - 1] + psi_delta * 
      (F.sample - (number_sample(Band) - 1));
    xang = psi_x_[number_sample(Band) - 1] + psi_xdelta * 
      (F.sample - (number_sample(Band) - 1));
  } else {
    int i = (int) floor(F.sample);
    yang = psi_y_[i] + (psi_y_[i + 1] - psi_y_[i]) * (F.sample - i);
    xang = psi_x_[i] + (psi_x_[i + 1] - psi_x_[i]) * (F.sample - i);
  }
  xang += psi_delta * F.line;
  return ScLookVector(-tan(yang), tan(xang), -1);
}

ScLookVectorWithDerivative SpotCamera::sc_look_vector_with_derivative
(const FrameCoordinateWithDerivative& F, 
 int Band) const
{
  range_check(Band, 0, number_band());
  // Pick a value to use for outside the range of the data.
  double psi_delta = psi_y_[1] - psi_y_[0];
  double psi_xdelta = psi_x_[1] - psi_x_[0];
  AutoDerivative<double> yang, xang;
  if(F.sample < 0) {
    yang = psi_y_[0] + psi_delta * F.sample;
    xang = psi_x_[0] + psi_xdelta * F.sample;
  } else if(F.sample >= number_sample(Band) - 1) {
    yang = psi_y_[number_sample(Band) - 1] + psi_delta * 
      (F.sample - (number_sample(Band) - 1));
    xang = psi_x_[number_sample(Band) - 1] + psi_xdelta * 
      (F.sample - (number_sample(Band) - 1));
  } else {
    int i = (int) floor(F.sample.value());
    yang = psi_y_[i] + (psi_y_[i + 1] - psi_y_[i]) * (F.sample - i);
    xang = psi_x_[i] + (psi_x_[i + 1] - psi_x_[i]) * (F.sample - i);
  }
  xang += psi_delta * F.line;
  AutoDerivative<double> t(-1);
  if(yang.number_variable() > 0) {
    t.gradient().resize(yang.number_variable());
    t.gradient() = 0;
  }
  return ScLookVectorWithDerivative(-std::tan(yang), std::tan(xang), t);
}


//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void SpotCamera::print(std::ostream& Os) const
{
  Os << "Spot Camera"
     << "   Number sample: " << number_sample(0);
}
