// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include <std_vector.i>
%include "geocal_common.i"

%{
#include "rolling_shutter_constant_time_table.h"
%}
%base_import(time_table)
%import "image_coordinate.i"
%import "frame_coordinate.i"
%geocal_shared_ptr(GeoCal::RollingShutterConstantTimeTable);

namespace GeoCal {
class RollingShutterConstantTimeTable : public TimeTable {
public:
  RollingShutterConstantTimeTable(Time Min_time, Time Max_time, 
				  double Time_space = 40.8e-3);
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  %python_attribute(time_space, double)
  %pickle_serialization();
};

}
// List of things "import *" will include
%python_export("RollingShutterConstantTimeTable")
