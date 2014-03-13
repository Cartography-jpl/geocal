#include "camera.h"
#include "geocal_matrix.h"
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
  blitz::Array<double, 2> rz(3, 3), rx(3, 3), ry(3, 3);
  blitz::Array<double, 2> rm(&r[0][0], blitz::shape(3, 3), 
			     blitz::neverDeleteData);
  double cbeta = cos(beta_);
  double cdelta = cos(delta_);
  double cepsilon = cos(epsilon_);
  double sbeta = sin(beta_);
  double sdelta = sin(delta_);
  double sepsilon = sin(epsilon_);
  rz =  cepsilon, sepsilon, 0,
       -sepsilon, cepsilon, 0,
               0,        0, 1;
  rx = 1,       0,      0,
       0,  cdelta, sdelta,
       0, -sdelta, cdelta;
  ry = cbeta, 0, -sbeta,
           0, 1,      0,
       sbeta, 0,  cbeta;
  using namespace blitz::tensor;
  blitz::Array<double, 2> t(sum(ry(i, k) * rz(k, j), k));
  rm = sum(rx(i, k) * t(k, j), k);
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
  boost::array<double, 3> t;
  mul(r, Sl.look_vector, t);
  FrameCoordinate fres;
  fres.line = (focal_ * t[0] / t[2]) / line_pitch_ + 
    number_line(Band) / 2.0; 
  fres.sample = (focal_ * t[1] / t[2]) / sample_pitch_ + 
    number_sample(Band) / 2.0;
  return fres;
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
  boost::array<double, 3> t;
  t[0] = (F.line - number_line(Band) / 2.0) * line_pitch_;
  t[1] = (F.sample - number_sample(Band) / 2.0) * sample_pitch_;
  t[2] = focal_;
  ScLookVector res;
  mul_t(r, t, res.look_vector);
  return res;
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
