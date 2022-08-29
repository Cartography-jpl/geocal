// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include <std_vector.i>
%include "geocal_common.i"

%{
#include "time_table.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%base_import(with_parameter)
%base_import(geocal_time)
%include "geocal_time_include.i"
%import "frame_coordinate.i"
%geocal_shared_ptr(GeoCal::TimeTable);
%geocal_shared_ptr(GeoCal::ConstantSpacingTimeTable);
%geocal_shared_ptr(GeoCal::ConstantSpacingFrameletTimeTable);
%geocal_shared_ptr(GeoCal::MeasuredTimeTable);

namespace GeoCal {
class TimeTable : public WithParameter {
public:
  TimeTable();
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const = 0;
  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const TimeWithDerivative T, 
				   const FrameCoordinateWithDerivative& F)
    const;
  virtual void time(const ImageCoordinate& Ic, Time &OUTPUT, 
		    FrameCoordinate &OUTPUT) const;
  virtual void time_with_derivative(const ImageCoordinateWithDerivative& Ic, 
			    TimeWithDerivative &OUTPUT, 
			    FrameCoordinateWithDerivative &OUTPUT) const;
  virtual void time_acquisition(const Time& T, const FrameCoordinate& Fc,
				Time& OUTPUT, Time& OUTPUT) const;
  %python_attribute(min_line, virtual int)
  %python_attribute(max_line, virtual int)
  %python_attribute(min_time, virtual Time)
  %python_attribute(max_time, virtual Time)
  %python_attribute_with_set_virtual(parameter, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name, virtual std::vector<std::string>);
  %python_attribute_with_set_virtual(parameter_subset, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative_subset, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name_subset, virtual std::vector<std::string>);
  %python_attribute(parameter_mask, virtual blitz::Array<bool, 1>);
  std::string print_to_string() const;
};
class ConstantSpacingTimeTable : public TimeTable {
public:
  ConstantSpacingTimeTable(Time Min_time, Time Max_time, 
			   double Time_space = 40.8e-3);
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  %python_attribute(time_space, double)
  %pickle_serialization();
};
class ConstantSpacingFrameletTimeTable : public TimeTable {
public:
  ConstantSpacingFrameletTimeTable(Time Min_time, Time Max_time,
				   int Framelet_size,
				   double Time_space);
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  %python_attribute(time_space, double)
  %python_attribute(framelet_size, int)
  %pickle_serialization();
};

class MeasuredTimeTable : public TimeTable {
public:
   MeasuredTimeTable(const std::vector<Time>& Time_list,
		    int Min_line = 0);
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const;
  %python_attribute(size_time_list, int)
  Time time_list(int i) const;
  %pickle_serialization();
};
}

// List of things "import *" will include
///
// Note Vector_Time is actually from geocal_time_include.i. Since this gets
// included in multiple files, we just pick one to export it

%python_export("TimeTable", "ConstantSpacingTimeTable", "MeasuredTimeTable",
	       "ConstantSpacingFrameletTimeTable",
	       "Vector_Time", "Vector_Time2")
