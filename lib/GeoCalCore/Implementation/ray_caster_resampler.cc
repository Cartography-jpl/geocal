#include "ray_caster_resampler.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RayCasterResampler::serialize(Archive & ar,
				   const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(RayCasterResampler);
  ar & GEOCAL_NVP_(ray_caster)
    & GEOCAL_NVP_(mi);
}

GEOCAL_IMPLEMENT(RayCasterResampler);
#endif

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

RayCasterResampler::RayCasterResampler
(const boost::shared_ptr<RayCaster>& Ray_caster,
 const boost::shared_ptr<MapInfo>& Map_info)
: ray_caster_(Ray_caster),
  mi_(Map_info)
{
}

//-----------------------------------------------------------------------
/// Do ray cast step.
///
/// We fill in Res, which should be number_line x number_sample x
/// nsub_line x nsub_sample x nintegration_step. This gets filled with
/// the image line/sample in the map_info that each pixel goes. We
/// only fill in the portion covered by our ray_caster().
///
/// The Res will often be a mmap array created in python (to handle
/// large images).
///
/// This is a clumsy function, but this should be looked at as a low
/// level step used by the python wrapper.
///
/// Note the argument Res being passed as an Array rather than a
/// reference is actually correct. Normally we pass things in that
/// shouldn't change from python. In this case, we really do want to
/// change. But we don't want a nonconst version of Array& in SWIG
/// because this is almost always an error (just in this case it isn't).
//-----------------------------------------------------------------------

void RayCasterResampler::ray_cast_step(blitz::Array<int, 6> Res)
{
  if(ray_caster_->start_position() + ray_caster_->number_position() > Res.extent(0))
    throw Exception("Res isn't large enough to hold results");
  if(ray_caster_->shape(1) != Res.extent(1) ||
     ray_caster_->shape(2) != Res.extent(2) ||
     ray_caster_->shape(3) != Res.extent(3) ||
     ray_caster_->shape(4) != Res.extent(4) ||
     Res.extent(5) != 2)
    throw Exception("Res isn't the size needed by ray_cast_step");
  while(!ray_caster_->last_position()) {
    blitz::Array<double, 6> t = ray_caster_->next_position();
    boost::array<double, 3> pv;
    for(int i = 0; i < t.shape()[0]; ++i)
      for(int j = 0; j < t.shape()[1]; ++j)
	for(int k1 = 0; k1 < t.shape()[2]; ++k1)
	  for(int k2 = 0; k2 < t.shape()[3]; ++k2)
	    for(int k3 = 0; k3 < t.shape()[4]; ++k3) {
	      pv[0] = t(i,j,k1,k2,k3,0);
	      pv[1] = t(i,j,k1,k2,k3,1);
	      pv[2] = t(i,j,k1,k2,k3,2);
	      boost::shared_ptr<CartesianFixed> p =
		ray_caster_->cartesian_fixed()->create(pv);
	      ImageCoordinate ic;
	      mi_->coordinate(*p, ic.sample, ic.line);
	      Res(i + ray_caster_->current_position(), j, k1, k2, k3, 0) =
	       	round(ic.line);
	      Res(i + ray_caster_->current_position(), j, k1, k2, k3, 1) =
	       	round(ic.sample);
	    }
  }
}

