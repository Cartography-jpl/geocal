#ifndef AIR_MSPI_TIME_TABLE_H
#define AIR_MSPI_TIME_TABLE_H
#include "air_mspi_l1b1.h"
#include "time_table.h"

namespace GeoCal {
/****************************************************************//**
  This is a time table based on AirMSPI L1B data.
*******************************************************************/

class AirMspiTimeTable : public MeasuredTimeTable {
public:
  AirMspiTimeTable(const std::string& L1b1_file_name, 
		   const std::string& Swath_to_use = "660-I",
		   int Min_l1b1_line = 0,
		   int Max_l1b1_line = -1);
  virtual ~AirMspiTimeTable() {}
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// The file name we are using.
//-----------------------------------------------------------------------

  std::string l1b1_file_name() const { return l1b1_file_->file_name(); }

//-----------------------------------------------------------------------
/// The file granule id we are using.
//-----------------------------------------------------------------------

  std::string l1b1_granule_id() const { return l1b1_file_->granule_id(); }

//-----------------------------------------------------------------------
/// Underlying L1b1File.
//-----------------------------------------------------------------------

  const boost::shared_ptr<AirMspiL1b1File>& l1b1_file() const 
  { return l1b1_file_; } 
private:
  boost::shared_ptr<AirMspiL1b1File> l1b1_file_;
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
