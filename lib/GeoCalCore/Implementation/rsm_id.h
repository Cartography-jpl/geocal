#ifndef RSM_ID_H
#define RSM_ID_H
#include "rsm_base.h"
#include "geocal_time.h"
#include "ground_coordinate.h"
#include <boost/optional.hpp>

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
protected:
  RsmIdTiming() {}
private:
  int number_row_acquired_simultaneously_, number_col_acquired_simultaneously_;
  double time_between_adjacent_row_group_, time_between_adjacent_col_group_;
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
    : cconv(Cconv),
      image_identifier_(Base->image_identifier()),
      rsm_suport_data_edition_(Base->rsm_suport_data_edition()),
      ground_domain_vertex_(8),
      min_line_(-1), max_line_(-1), min_sample_(-1), max_sample_(-1)
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

//-----------------------------------------------------------------------
/// Ground domain vertex
//-----------------------------------------------------------------------

  const std::vector<boost::shared_ptr<GroundCoordinate> >&
  ground_domain_vertex() const { return ground_domain_vertex_;}
  std::vector<boost::shared_ptr<GroundCoordinate> >&
  ground_domain_vertex() { return ground_domain_vertex_;}
  void ground_domain_vertex(const std::vector<boost::shared_ptr<GroundCoordinate> >& V)
  {
    if(V.size() != 8)
      throw Exception("Ground domain vertex needs to be exactly 8 points");
    ground_domain_vertex_ = V;
  }

//-----------------------------------------------------------------------
/// Ground reference point.
//-----------------------------------------------------------------------
  const boost::shared_ptr<GroundCoordinate>& ground_reference_point() const
  { return ground_reference_point_; }
  void ground_reference_point(const boost::shared_ptr<GroundCoordinate>& V)
  { ground_reference_point_ = V; }

//-----------------------------------------------------------------------
/// Size of the full image
//-----------------------------------------------------------------------

  const boost::optional<int>& full_number_line() const
  { return full_number_line_; }
  void full_number_line(const boost::optional<int>& V)
  { full_number_line_ = V; }
  const boost::optional<int>& full_number_sample() const
  { return full_number_sample_; }
  void full_number_sample(const boost::optional<int>& V)
  { full_number_sample_ = V; }
  

//-----------------------------------------------------------------------
/// Range of image covered by Rsm
//-----------------------------------------------------------------------

  int min_line() const { return min_line_; }
  void min_line(int V) { min_line_ = V; }
  int min_sample() const { return min_sample_; }
  void min_sample(int V) { min_sample_ = V; }
  int max_line() const { return max_line_; }
  void max_line(int V) { max_line_ = V; }
  int max_sample() const { return max_sample_; }
  void max_sample(int V) { max_sample_ = V; }
  
  std::string tre_string() const;
  static boost::shared_ptr<RsmId>
  read_tre_string(const std::string& Tre_in);
  virtual void print(std::ostream& Os) const;
protected:
  RsmId() {}
private:
  boost::shared_ptr<CoordinateConverter> cconv;
  std::string image_identifier_, rsm_suport_data_edition_,
    image_sequence_identifier_, sensor_identifier_,
    sensor_type_;
  boost::shared_ptr<Time> image_acquistion_time_;
  boost::shared_ptr<RsmIdTiming> timing_;
  std::vector<boost::shared_ptr<GroundCoordinate> > ground_domain_vertex_;
  boost::shared_ptr<GroundCoordinate> ground_reference_point_;
  boost::optional<int> full_number_line_, full_number_sample_;
  int min_line_, max_line_, min_sample_, max_sample_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmId);
GEOCAL_EXPORT_KEY(RsmIdTiming);
#endif
