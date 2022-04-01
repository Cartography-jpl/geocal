// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "igc_ray_caster.h"
%}
%base_import(ray_caster)
%import "image_ground_connection.i"
%geocal_shared_ptr(GeoCal::IgcRayCaster);
namespace GeoCal {
class IgcRayCaster: public RayCaster {
public:
  IgcRayCaster(const boost::shared_ptr<ImageGroundConnection>& Igc,
	       int Start_line = 0,
	       int Number_line = -1,
	       int Number_integration_step = 2, double Resolution = 100,
	       double Max_height = 10e3,
	       int Start_sample = 0,
	       int Number_sample = -1,
	       bool Include_path_distance = false
	       );
  virtual blitz::Array<double, 6> next_position();
  %python_attribute_with_set(number_sub_line, int);
  %python_attribute_with_set(number_sub_sample, int);
  %python_attribute_with_set(number_integration_step, int);
  %python_attribute_with_set(include_path_distance, bool);
  %python_attribute(start_sample, int);
  %python_attribute(number_sample, int);
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("IgcRayCaster")
