#ifndef RAY_CASTER_RESAMPLER_H
#define RAY_CASTER_RESAMPLER_H
#include "raster_image.h"
#include "ray_caster.h"

namespace GeoCal {
/****************************************************************//**
  This is used to project data to the surface using a RayCaster. This
  is similar to IgcMapProjected, except rather than going from the
  ground to the image we go from the image to the ground. Depending on
  the type of ImageGroundConnection we are using, one or the other
  direction can be much faster.

  This class is really designed to be wrapped in python. Although it
  could be used from C++, it would probably be a bit awkward. We can
  always rework this if needed, but I expect the primary use of this
  will be programs like igc_project.
  
  There are two steps involved:

  1. The initial ray casting, determining where each pixel goes in
     the final map.
  2. Resampling the data to create the final radiance data.

  Step 1 can be done in parallel to speed things up. We currently have
  step 2 needing to be done serially, I'm not sure that it could be
  done in parallel.

  For step 1, The subsetting should be done on Ray_caster (so like
  IgcRayCaster where we specify the start and number of
  lines). ray_cast_step() steps through all the positions in the ray
  caster.
*******************************************************************/

class RayCasterResampler: public Printable<RayCasterResampler> {
public:
  RayCasterResampler(const boost::shared_ptr<RayCaster>& Ray_caster,
		     const boost::shared_ptr<MapInfo>& Map_info);
  virtual ~RayCasterResampler() {}
  void ray_cast_step(blitz::Array<int, 6> Ray_cast_res);
  void final_rad_step(const RasterImage& Input_img,
		      blitz::Array<int, 6> Ray_cast_res,
		      blitz::Array<int, 2> Rad_res,
		      blitz::Array<int, 2> Scratch_count);
  const boost::shared_ptr<RayCaster>& ray_caster() const  { return ray_caster_;}
  const boost::shared_ptr<MapInfo>& map_info() const  { return mi_;}
  virtual void print(std::ostream& Os) const
  { Os << "RayCasterResampler\n"; }
private:
  boost::shared_ptr<RayCaster> ray_caster_;
  boost::shared_ptr<MapInfo> mi_;
  RayCasterResampler() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(RayCasterResampler);
#endif
  
  
  
  
  

  
