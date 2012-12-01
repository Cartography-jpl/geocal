// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "time_table.h"
#include "quickbird_time_table.h"
%}
%geocal_shared_ptr(TimeTable);
%geocal_shared_ptr(ConstantSpacingTimeTable);

%shared_ptr_dynamic_list(GeoCal::TimeTable,
			 GeoCal::ConstantSpacingTimeTable,
			 GeoCal::QuickBirdTimeTable)

namespace GeoCal {
class TimeTable {
public:
  TimeTable();
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const = 0;
  virtual void time(const ImageCoordinate& Ic, Time &OUTPUT, 
		    FrameCoordinate &OUTPUT) const = 0;
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
  virtual void time(const ImageCoordinate& Ic, Time &OUTPUT, 
		    FrameCoordinate &OUTPUT) const;
  %python_attribute(time_space, double)
  %pickle_init(self.min_time, self.max_time, self.time_space)
};
}
