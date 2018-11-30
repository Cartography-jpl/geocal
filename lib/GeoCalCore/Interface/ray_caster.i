// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "ray_caster.h"
#include "raster_image.h"
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%import "raster_image.i"
%import "image_coordinate.i"
%import "map_info.i"
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::RayCaster);
namespace GeoCal {
class RayCaster: public GenericObject {
public:
  %python_attribute(start_position, int);
  %python_attribute(number_position, int);
  %python_attribute(last_position, bool);
  %python_attribute(current_position, int);
  virtual blitz::Array<double, 6> next_position() = 0;
  blitz::Array<double, 2> next_radiance(const RasterImage& Surface_rad,
					double Fill_value = 0.0);
  std::string print_to_string() const;
};
}

// List of things "import *" will include
%python_export("RayCaster")
