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
		     const boost::shared_ptr<MapInfo>& Map_info);
  void ray_cast_step(blitz::Array<int, 6> Ray_cast_res);
  void final_rad_step(const RasterImage& Input_img,
		      blitz::Array<int, 6> Ray_cast_res,
		      blitz::Array<int, 2> Rad_res,
		      blitz::Array<int, 2> Scratch_count);
  void final_rad_step_double(const RasterImage& Input_img,
			     blitz::Array<int, 6> Ray_cast_res,
			     blitz::Array<double, 2> Rad_res,
			     blitz::Array<int, 2> Scratch_count);
  %python_attribute(ray_caster, boost::shared_ptr<RayCaster>);
  %python_attribute(map_info, boost::shared_ptr<MapInfo>);
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("RayCasterResampler")
