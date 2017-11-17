#ifndef RSM_ID_H
#define RSM_ID_H
#include "rsm_base.h"
#include "geocal_time.h"

namespace GeoCal {
/****************************************************************//**
  RSM timing information. This is just a structure, but we put this
  together because we always either want all or none of these fields.
*******************************************************************/

class RsmIdTiming : public Printable<RsmIdTiming> {
public:
  RsmIdTiming(int Number_row_acquired_simulatenously,
	      int Number_col_acquired_simulatenously,
	      double Time_betweeen_adjacent_row_group,
	      double Time_betweeen_adjacent_col_group)
    : number_row_acquired_simultaneously_(Number_row_acquired_simulatenously),
      number_col_acquired_simultaneously_(Number_col_acquired_simulatenously),
      time_between_adjacent_row_group_(Time_betweeen_adjacent_row_group),
      time_between_adjacent_col_group_(Time_betweeen_adjacent_col_group)
  { }

  int number_row_acquired_simultaneously() const
  {return number_row_acquired_simultaneously_; }
  void number_row_acquired_simultaneously(int V)
  {number_row_acquired_simultaneously_ = V; }
  int number_col_acquired_simultaneously() const
  {return number_col_acquired_simultaneously_; }
  void number_col_acquired_simultaneously(int V)
  {number_col_acquired_simultaneously_ = V; }
  double time_between_adjacent_row_group() const
  { return time_between_adjacent_row_group_;}
  void time_between_adjacent_row_group(double V)
  { time_between_adjacent_row_group_ = V;}
  double time_between_adjacent_col_group() const
  { return time_between_adjacent_col_group_;}
  void time_between_adjacent_col_group(double V)
  { time_between_adjacent_col_group_ = V;}
  virtual void print(std::ostream& Os) const;
private:
  int number_row_acquired_simultaneously_, number_col_acquired_simultaneously_;
  double time_between_adjacent_row_group_, time_between_adjacent_col_group_;
  RsmIdTiming() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
  
/****************************************************************//**
  This provides additional information about the Rsm, it is what is
  used to populate 
*******************************************************************/

class RsmId : public Printable<RsmId> {
public:
  RsmId(const boost::shared_ptr<RsmBase>& Base,
	const boost::shared_ptr<CoordinateConverter> &Cconv)
    : image_identifier_(Base->image_identifier()),
      rsm_suport_data_edition_(Base->rsm_suport_data_edition()),
      cconv(Cconv)
  {
  }
  virtual ~RsmId() {}

  const boost::shared_ptr<CoordinateConverter>& coordinate_converter() const
  { return cconv; }
  void coordinate_converter(const boost::shared_ptr<CoordinateConverter>& V) 
  { cconv = V; }
  
//-----------------------------------------------------------------------
/// Image identification.
//-----------------------------------------------------------------------
  const std::string& image_identifier() const { return image_identifier_;}
  void image_identifier(const std::string& V) { image_identifier_ = V;}

//-----------------------------------------------------------------------
/// RSM Support Data Edition.
//-----------------------------------------------------------------------
  const std::string& rsm_suport_data_edition() const
  { return rsm_suport_data_edition_;}
  void rsm_suport_data_edition(const std::string& V)
  { rsm_suport_data_edition_ = V; }

//-----------------------------------------------------------------------
/// Image Sequence Identifier
//-----------------------------------------------------------------------

  const std::string& image_sequence_identifier() const
  { return image_sequence_identifier_;}
  void image_sequence_identifier(const std::string& V)
  { image_sequence_identifier_ = V; }

//-----------------------------------------------------------------------
/// Sensor Identifier
//-----------------------------------------------------------------------

  const std::string& sensor_identifier() const
  { return sensor_identifier_;}
  void sensor_identifier(const std::string& V)
  { sensor_identifier_ = V; }
  
//-----------------------------------------------------------------------
/// Sensor Type Identifier
//-----------------------------------------------------------------------

  const std::string& sensor_type() const
  { return sensor_type_;}
  void sensor_type(const std::string& V)
  { sensor_type_ = V; }
  
//-----------------------------------------------------------------------
/// Image Acquisition time. This might or might not be available, if
/// it isn't then the pointer is null.
//-----------------------------------------------------------------------

  const boost::shared_ptr<Time>& image_acquistion_time() const
  { return image_acquistion_time_;}
  void image_acquistion_time(const boost::shared_ptr<Time>& V)
  { image_acquistion_time_ = V; }

//-----------------------------------------------------------------------
/// Timing information
//-----------------------------------------------------------------------
  const boost::shared_ptr<RsmIdTiming> timing() const
  { return timing_; }
  void timing(const boost::shared_ptr<RsmIdTiming>& V)
  { timing_ = V; }
  
  std::string tre_string() const;
  static boost::shared_ptr<RsmId>
  read_tre_string(const std::string& Tre_in);
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<CoordinateConverter> cconv;
  std::string image_identifier_, rsm_suport_data_edition_,
    image_sequence_identifier_, sensor_identifier_,
    sensor_type_;
  boost::shared_ptr<Time> image_acquistion_time_;
  boost::shared_ptr<RsmIdTiming> timing_;
  RsmId() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmId);
GEOCAL_EXPORT_KEY(RsmIdTiming);
#endif
