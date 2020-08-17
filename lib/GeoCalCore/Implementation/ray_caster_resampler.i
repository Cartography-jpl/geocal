// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "ray_caster_resampler.h"
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%import "raster_image.i"
%import "ray_caster.i"

%geocal_shared_ptr(GeoCal::RayCasterResampler);
namespace GeoCal {
class RayCasterResampler: public GenericObject {
public:
  RayCasterResampler(const boost::shared_ptr<RayCaster>& Ray_caster,
		     const boost::shared_ptr<RasterImage>& Img,
		     const boost::shared_ptr<RasterImage>& Res,
		     const boost::shared_ptr<RasterImage>& Count_scratch);
  void ray_cast_step();
  void final_rad_step(int start_line, int end_line);
  %python_attribute(ray_caster, boost::shared_ptr<RayCaster>);
  %python_attribute(image, boost::shared_ptr<RasterImage>);
  %python_attribute(res, boost::shared_ptr<RasterImage>);
  %python_attribute(count_scratch, boost::shared_ptr<RasterImage>);
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("RayCasterResampler")
