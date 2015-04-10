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
  AirMspiTimeTable(const std::string& L1b1_file_name, 
		   int Reference_row);
  virtual ~AirMspiTimeTable() {}
  virtual void print(std::ostream& Os) const;
  int reference_row() const { return refrow; }

//-----------------------------------------------------------------------
/// The file name we are using.
//-----------------------------------------------------------------------

  const std::string& l1b1_file_name() const { return l1b1_file_name_; }

//-----------------------------------------------------------------------
/// The file granule id we are using.
//-----------------------------------------------------------------------

  const std::string& l1b1_granule_id() const { return l1b1_granule_id_; }

  static int 
  reference_row_calc(const std::string& Instrument_config_file_name);
 
private:
  std::string l1b1_file_name_;
  std::string l1b1_granule_id_;
  int refrow;
  void read_data();
  AirMspiTimeTable() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}
GEOCAL_EXPORT_KEY(AirMspiTimeTable);
#endif
