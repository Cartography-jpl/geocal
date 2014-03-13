#include "camera.h"
#include "geocal_quaternion.h"
#include "geocal_exception.h"
#include <blitz/array.h>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// This creates SimpleCamera with the given data. The
/// default data is for the nominal MISR DF, red band.
//-----------------------------------------------------------------------
SimpleCamera::SimpleCamera
(double Beta, double Delta, double Epsilon, double Focal,
 double line_pitch, double sample_pitch, int Number_line, int Number_sample)
  : beta_(Beta), delta_(Delta), epsilon_(Epsilon), focal_(Focal),
    line_pitch_(line_pitch), sample_pitch_(sample_pitch), 
    nline(Number_line),
    nsample(Number_sample) 
{
  frame_to_sc = quat_rot("ZYX", epsilon_, beta_, delta_);
}

//-----------------------------------------------------------------------
/// This converts from ScLookVector to FrameCoordinate for a given
/// band. Note that the FrameCoordinate may be outside of the range
/// (0, number_line(band) - 1), (0, number_sample(band) - 1), this
/// just means the look vector is not with the active portion of the
/// camera (i.e., it is not seen).
//-----------------------------------------------------------------------

FrameCoordinate SimpleCamera::frame_coordinate(const ScLookVector& Sl, 
							int Band) const
{
  range_check(Band, 0, number_band());
  boost::math::quaternion<double> fv = 
    conj(frame_to_sc) * Sl.look_quaternion() * frame_to_sc;
  FrameCoordinate fc;
  fc.line = number_line(Band) / 2.0 +
    focal_ * (fv.R_component_2() / fv.R_component_4()) / 
    line_pitch_;
  fc.sample = number_sample(Band) / 2.0 +
    focal_ * (fv.R_component_3() / fv.R_component_4()) / 
    sample_pitch_;
  return fc;
}

//-----------------------------------------------------------------------
/// Convert from FrameCoordinate to ScLookVector. It is perfectly
/// allowable for F.line to be outside the range (0, number_line(band)
/// - 1) or for F.sample to be outside the range (0,
/// number_sample(band) - 1). The conversion will just act as if the
/// camera has infinite extent.
//-----------------------------------------------------------------------

ScLookVector SimpleCamera::sc_look_vector(const FrameCoordinate& F, 
						   int Band) const
{
  range_check(Band, 0, number_band());
  ScLookVector sl((F.line - number_line(Band) / 2.0) * line_pitch_,
		  (F.sample - number_sample(Band) / 2.0) * sample_pitch_,
		  focal_);
  sl.look_quaternion(frame_to_sc * sl.look_quaternion() * 
		     conj(frame_to_sc));
  return sl;
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void SimpleCamera::print(std::ostream& Os) const
{
  Os << "SimpleCamera:\n"
     << " beta:          " << beta_ * Constant::rad_to_deg << " deg\n"
     << " delta:         " << delta_ * Constant::rad_to_deg << " deg\n"
     << " epsilon:       " << epsilon_ * Constant::rad_to_deg << " deg\n"
     << " Focal Length:  " << focal_ / 1e-3 << " mm\n"
     << " Line Pitch:    " << line_pitch_ / 1e-6 << " micrometer\n"
     << " Sample Pitch:  " << sample_pitch_ / 1e-6 << " micrometer\n"
     << " Number Line:   " << nline << "\n"
     << " Number Sample: " << nsample << "\n";
}
