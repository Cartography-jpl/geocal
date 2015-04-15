// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_time_table.h"
%}

%base_import(time_table)

%geocal_shared_ptr(GeoCal::AirMspiTimeTable);
namespace GeoCal {
class AirMspiTimeTable : public MeasuredTimeTable {
public:
  AirMspiTimeTable(const std::string& L1b1_file_name, 
		   const std::string& Instrument_config_file_name);
  %python_attribute(reference_row, int);
  %python_attribute(l1b1_file_name, std::string);
  %python_attribute(l1b1_granule_id, std::string);
  static int 
  reference_row_calc(const std::string& Instrument_config_file_name);
  %pickle_serialization();
};
}
