// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_id.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"  
%}
%import "rsm_base.i"
%import "coordinate_converter.i"

%geocal_shared_ptr(GeoCal::RsmIdTiming);
%geocal_shared_ptr(GeoCal::RsmId);

namespace GeoCal {
class RsmIdTiming: public GenericObject {
public:
  RsmIdTiming(int Number_row_acquired_simulatenously,
	      int Number_col_acquired_simulatenously,
	      double Time_betweeen_adjacent_row_group,
	      double Time_betweeen_adjacent_col_group);
  %python_attribute_with_set(number_row_acquired_simultaneously, int);
  %python_attribute_with_set(number_col_acquired_simultaneously, int);
  %python_attribute_with_set(time_between_adjacent_row_group, double);
  %python_attribute_with_set(time_between_adjacent_col_group, double);
  virtual std::string print_to_string() const;
  %pickle_serialization()
};

class RsmId : public GenericObject {
public:
  RsmId(const boost::shared_ptr<RsmBase>& Base,
	const boost::shared_ptr<CoordinateConverter> &Cconv);
  virtual std::string print_to_string() const;
  %python_attribute_with_set(coordinate_converter, boost::shared_ptr<CoordinateConverter>);
  %python_attribute_with_set(image_identifier, std::string);
  %python_attribute_with_set(rsm_suport_data_edition, std::string);
  %python_attribute_with_set(image_sequence_identifier, std::string);
  %python_attribute_with_set(sensor_identifier, std::string);
  %python_attribute_with_set(sensor_type, std::string);
  %python_attribute_with_set(image_acquistion_time, boost::shared_ptr<Time>);
  %python_attribute_with_set(timing, boost::shared_ptr<RsmIdTiming>);
  std::string tre_string() const;
  static boost::shared_ptr<RsmId>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}
