// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include <std_vector.i>
%include "common.i"

%{
#include "time_table.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%import "geocal_time.i"
%import "frame_coordinate.i"
%geocal_shared_ptr(GeoCal::TimeTable);
%geocal_shared_ptr(GeoCal::ConstantSpacingTimeTable);
%geocal_shared_ptr(GeoCal::MeasuredTimeTable);

namespace GeoCal {
class TimeTable : public GenericObject {
public:
  TimeTable();
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const = 0;
  virtual void time(const ImageCoordinate& Ic, Time &OUTPUT, 
		    FrameCoordinate &OUTPUT) const;
  %python_attribute(min_line, virtual int)
  %python_attribute(max_line, virtual int)
  %python_attribute(min_time, virtual Time)
  %python_attribute(max_time, virtual Time)
  std::string print_to_string() const;
};
class ConstantSpacingTimeTable : public TimeTable {
public:
  ConstantSpacingTimeTable(Time Min_time, Time Max_time, 
			   double Time_space = 40.8e-3);
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  %python_attribute(time_space, double)
  %pickle_init(1, self.min_time, self.max_time, self.time_space)
};

class MeasuredTimeTable : public TimeTable {
public:
   MeasuredTimeTable(const std::vector<Time>& Time_list,
		    int Min_line = 0);
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  %python_attribute(time_list, std::vector<Time>)
  %pickle_init(1, self.min_line, self.time_list)
};
}
