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
    & GEOCAL_NVP_(res)
    & GEOCAL_NVP_(count_scratch);
}

GEOCAL_IMPLEMENT(RayCasterResampler);
#endif

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

RayCasterResampler::RayCasterResampler
(const boost::shared_ptr<RayCaster>& Ray_caster,
 const boost::shared_ptr<RasterImage>& Img,
 const boost::shared_ptr<RasterImage>& Res,
 const boost::shared_ptr<RasterImage>& Count_scratch)
: ray_caster_(Ray_caster),
  img_(Img), res_(Res),
  count_scratch_(Count_scratch)
{
}

//-----------------------------------------------------------------------
/// Do ray cast step.
//-----------------------------------------------------------------------

void RayCasterResampler::ray_cast_step()
{
  while(!ray_caster_->last_position()) {
    blitz::Array<double, 6> t = ray_caster_->next_position();
    for(int i = 0; i < t.shape()[0]; ++i)
      for(int j = 0; j < t.shape()[1]; ++j)
	for(int k1 = 0; k1 < t.shape()[2]; ++k1)
	  for(int k2 = 0; k2 < t.shape()[3]; ++k2)
	    for(int k3 = 0; k3 < t.shape()[4]; ++k3)
	      ;
  }
}

//-----------------------------------------------------------------------
/// Do final radiance calculation step (scaling by count) This is in
/// res() space.
//-----------------------------------------------------------------------

void RayCasterResampler::final_rad_step(int start_line, int end_line)
{
  for(int i = start_line; i < end_line && i < res_->number_line(); ++i)
    for(int j = 0; j < res_->number_sample(); ++j)
      if((*count_scratch_)(i, j) > 0) {
	int value = (*res_)(i, j) / (*count_scratch_)(i, j);
	res_->write(i, j, value);
      }
}
