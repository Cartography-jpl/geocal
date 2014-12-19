#ifndef AIR_MSPI_TIME_TABLE_H
#define AIR_MSPI_TIME_TABLE_H
#include "time_table.h"

namespace GeoCal {
/****************************************************************//**
  This is a time table based on AirMSPI L1B data.
*******************************************************************/

class AirMspiTimeTable : public MeasuredTimeTable {
public:
  AirMspiTimeTable(const std::string& L1b1_file_name, 
		   const std::string& Instrument_config_file_name);
  virtual ~AirMspiTimeTable() {}
  virtual void print(std::ostream& Os) const;
private:
  std::string l1b1_file_name;
  int refrow;
  int reference_row(const std::string& Instrument_config_file_name) const;
  void read_data();
};
}
#endif
