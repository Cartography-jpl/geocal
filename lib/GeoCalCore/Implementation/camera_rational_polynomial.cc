#include "camera_rational_polynomial.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void CameraRationalPolyomial::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(QuaternionCamera);
  ar & GEOCAL_NVP_(kappa) & GEOCAL_NVP_(kappa_inverse);
}


GEOCAL_IMPLEMENT(CameraRationalPolyomial);
#endif

void CameraRationalPolyomial::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "CameraRationalPolyomial:\n"
     << "  Nu: " << number_sample(0) << "\n"
     << "  Nv: " << number_line(0) << "\n"
     << "  Xi: " << sample_pitch() << "\n"
     << "  U0: " << principal_point(0).sample << "\n"
     << "  V0: " << principal_point(0).line << "\n"
     << "  Pld_q_c: " << frame_to_sc() << "\n"
     << "  Kappa: " << "\n";
  opad << kappa() << "\n";
  opad.strict_sync();
  Os << "  Kappa_inverse: " << "\n";
  opad << kappa_inverse() << "\n";
  opad.strict_sync();
}
