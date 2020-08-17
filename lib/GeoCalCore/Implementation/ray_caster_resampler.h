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
  
  *Important* Res and Count_scratch should be initialized to 0 (e.g.,
  use fill_image). This class *does* not do this, because we may be
  called in parallel. It is just assumed by this class that these
  images have been initialized.

  Because the data may be large, we make use of a second raster image
  to maintain the counts. Note also that the primary raster image will
  possibly have multiple pixels placed in it, so it should have a
  large enough range to include this (so for example a int 8 imake may
  want to allow int 16 during the calculation).  You might need to
  copy the data to a final image of a smaller size afterwards - but
  since we would then need a scratch intermediate image there doesn't
  seem much of a downside to using this image.

  There are two steps involved:

  1. The initial ray casting
  2. Scaling the results with the count

  To support parallelization these two steps can be called to use a
  subset of the data. If you do this, you'll want to make sure the 
  RasterImage you supply can support parallel writing - e.g., 
  VicarLiteRasterImage with memory mapping.

  The subsetting should be done on Ray_caster (so like IgcRayCaster
  where we specify the start and number of lines).
*******************************************************************/

class RayCasterResampler: public Printable<RayCasterResampler> {
public:
  RayCasterResampler(const boost::shared_ptr<RayCaster>& Ray_caster,
		     const boost::shared_ptr<RasterImage>& Img,
		     const boost::shared_ptr<RasterImage>& Res,
		     const boost::shared_ptr<RasterImage>& Count_scratch);
  virtual ~RayCasterResampler() {}
  void ray_cast_step();
  void final_rad_step(int start_line, int end_line);
  const boost::shared_ptr<RayCaster>& ray_caster() const  { return ray_caster_;}
  const boost::shared_ptr<RasterImage>& image() const  { return img_;}
  const boost::shared_ptr<RasterImage>& res() const  { return res_;}
  const boost::shared_ptr<RasterImage>& count_scratch() const
  { return count_scratch_;}
  virtual void print(std::ostream& Os) const
  { Os << "RayCasterResampler"; }
private:
  boost::shared_ptr<RayCaster> ray_caster_;
  boost::shared_ptr<RasterImage> img_, res_, count_scratch_;
  RayCasterResampler() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(RayCasterResampler);
#endif
  
  
  
  
  

  
