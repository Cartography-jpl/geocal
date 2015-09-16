// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_time_table.h"
%}

%base_import(time_table)
%import "air_mspi_l1b1.i"

%geocal_shared_ptr(GeoCal::AirMspiTimeTable);
namespace GeoCal {
class AirMspiTimeTable : public MeasuredTimeTable {
public:
  AirMspiTimeTable(const std::string& L1b1_file_name, 
		   const std::string& Swath_to_use = "660-I",
		   int Min_l1b1_line = 0,
		   int Max_l1b1_line = -1);
  %python_attribute(l1b1_file_name, std::string);
  %python_attribute(l1b1_granule_id, std::string);
  %python_attribute(l1b1_file, boost::shared_ptr<AirMspiL1b1File>);
  %pickle_serialization();
};
}
