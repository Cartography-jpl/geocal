#include "ray_caster.h"
#include "geocal_serialize_support.h"
#include "ecr.h"
#include "raster_image.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RayCaster::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RayCaster);
}

GEOCAL_IMPLEMENT(RayCaster);
#endif

//-----------------------------------------------------------------------
/// Frequently you are using a ray caster to simulate what would be
/// seen by a particular camera/measurement device. This takes the
/// results of a call to next_position() and uses it to look up what
/// the surface radiance would be. We average over the radiance for
/// the sub pixels and number of integration steps. Pixels that lie
/// outside the range of Surface_rad get assigned a fill value.
///
/// The returned array should be considered "owned" by this class,
/// code calling this class should copy this data if it wants to make
/// any modifications to the underlying data.
//-----------------------------------------------------------------------

blitz::Array<double, 2> RayCaster::next_radiance(const RasterImage& Surface_rad,
						 double Fill_value)
{
  blitz::Array<double, 6> np = next_position();
  rad.resize(np.rows(), np.cols());
  for(int i = 0; i < rad.rows(); ++i)
    for(int j = 0; j < rad.cols(); ++j) {
      int count = 0;
      double sum = 0.0;
      for(int i1 = 0; i1 < np.shape()[2]; ++i1)
	for(int i2 = 0; i2 < np.shape()[3]; ++i2)
	  for(int i3 = 0; i3 < np.shape()[4]; ++i3) {
	    Ecr e(np(i,j,i1,i2,i3,0),np(i,j,i1,i2,i3,1),np(i,j,i1,i2,i3,2));
	    ImageCoordinate ic = Surface_rad.coordinate(e);
	    if(ic.line >= 0 && ic.line < Surface_rad.number_line() - 1 &&
	       ic.sample >= 0 && ic.sample < Surface_rad.number_sample() - 1) {
	      ++count;
	      sum += Surface_rad.unchecked_interpolate(ic.line, ic.sample);
	    }
	  }
      if(count > 0)
	rad(i,j) = sum / count;
      else
	rad(i, j) = Fill_value;
    }
  return rad;
}
