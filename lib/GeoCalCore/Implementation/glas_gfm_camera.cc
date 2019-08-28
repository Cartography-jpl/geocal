#include "glas_gfm_camera.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include <cmath>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GlasGfmCamera::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(QuaternionCamera);
  ar & GEOCAL_NVP_(band_type) & GEOCAL_NVP_(band_wavelength)
    & GEOCAL_NVP_(band_index) & GEOCAL_NVP_(irepband)
    & GEOCAL_NVP_(isubcat) & GEOCAL_NVP_(sample_number_first)
    & GEOCAL_NVP_(delta_sample_pair) & GEOCAL_NVP_(field_alignment);
}


GEOCAL_IMPLEMENT(GlasGfmCamera);
#endif

  
GlasGfmCamera::GlasGfmCamera()
  : QuaternionCamera(boost::math::quaternion<double>(1,0,0,0),
		      1, 256, 0.00765 / 128, 0.00765 / 128,
		      1.41009182,
		      FrameCoordinate(0, 128),
		      QuaternionCamera::LINE_IS_Y,
		      QuaternionCamera::INCREASE_IS_NEGATIVE,
		      QuaternionCamera::INCREASE_IS_POSITIVE)  
{
  field_alignment_.resize(1, 4);
  field_alignment_ = -0.00765, 0, 0.00765, 0;
}
