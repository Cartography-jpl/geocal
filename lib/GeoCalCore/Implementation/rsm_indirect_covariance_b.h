#ifndef RSM_INDIRECT_COVARIANCE_B_H
#define RSM_INDIRECT_COVARIANCE_B_H
#include "rsm_indirect_covariance.h"
#include "rsm_adjustable_parameter_b.h"

namespace GeoCal {
class RsmId;  // Forward declaration

/****************************************************************//**
  This handles a single "independent subgroup", basically a set of
  original parameters that has a given covariance and time behavior
*******************************************************************/
class RsmBSubgroup : public Printable<RsmBSubgroup> {
public:
  RsmBSubgroup(const blitz::Array<double, 2> Cov,
	       int Time_correlation_domain_flag,
	       const blitz::Array<double, 2> Time_correlation);
  RsmBSubgroup(const blitz::Array<double, 2> Cov,
	       int Time_correlation_domain_flag,
	       double A, double alpha, double beta, double T);
  RsmBSubgroup(const RsmBSubgroup& V)
    : cov_(V.covariance().copy()),
      time_correlation_(V.time_correlation().copy()),
      time_correlation_domain_flag_(V.time_correlation_domain_flag())
  {}
  RsmBSubgroup() {}
  virtual ~RsmBSubgroup() {}
  int number_parameter() const { return cov_.rows(); }
  bool is_csm_correlation() const { return time_correlation_.cols() == 4; }
  const blitz::Array<double, 2>& covariance() const {return cov_;}
  const blitz::Array<double, 2>& time_correlation() const
  {return time_correlation_;}
  int time_correlation_domain_flag() const
  { return time_correlation_domain_flag_ ;}
  std::string tre_string() const;
  void read_tre_string(std::istream& In);
  virtual void print(std::ostream& Os) const;
  const RsmBSubgroup& operator=(const RsmBSubgroup& V)
  {
    cov_.reference(V.covariance().copy());
    time_correlation_.reference(V.time_correlation().copy());
    time_correlation_domain_flag_ = V.time_correlation_domain_flag();
    return *this;
  }
private:
  blitz::Array<double, 2> cov_;
  blitz::Array<double, 2> time_correlation_;
  int time_correlation_domain_flag_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This handles the unmodeled covariance, this is just complicated
  enough to be worth pulling out.
*******************************************************************/
class RsmBUnmodeledCovariance : public Printable<RsmBUnmodeledCovariance> {
public:
  RsmBUnmodeledCovariance(const blitz::Array<double, 2> Cov,
			  const blitz::Array<double, 2> Row_time_correlation,
			  const blitz::Array<double, 2> Cow_time_correlation);
  RsmBUnmodeledCovariance(const blitz::Array<double, 2> Cov,
	       double Row_A, double Row_alpha, double Row_beta, double Row_T,
	       double Col_A, double Col_alpha, double Col_beta, double Col_T);
  RsmBUnmodeledCovariance(const RsmBUnmodeledCovariance& V)
    : cov_(V.covariance().copy()),
      row_time_correlation_(V.row_time_correlation().copy()),
      col_time_correlation_(V.col_time_correlation().copy())
  {}
  RsmBUnmodeledCovariance() : cov_(2,2) {}
  virtual ~RsmBUnmodeledCovariance() {}
  bool is_csm_correlation() const { return row_time_correlation_.cols() == 4; }
  const blitz::Array<double, 2>& covariance() const {return cov_;}
  const blitz::Array<double, 2>& row_time_correlation() const
  {return row_time_correlation_;}
  const blitz::Array<double, 2>& col_time_correlation() const
  {return col_time_correlation_;}
  std::string tre_string() const;
  void read_tre_string(std::istream& In);
  virtual void print(std::ostream& Os) const;
  const RsmBUnmodeledCovariance& operator=(const RsmBUnmodeledCovariance& V)
  {
    cov_.reference(V.covariance().copy());
    row_time_correlation_.reference(V.row_time_correlation().copy());
    col_time_correlation_.reference(V.col_time_correlation().copy());
    return *this;
  }
private:
  blitz::Array<double, 2> cov_;
  blitz::Array<double, 2> row_time_correlation_;
  blitz::Array<double, 2> col_time_correlation_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
  
/****************************************************************//**
  This is used to handle the RSM indirect covariance. This 
  supports RSMECB.
*******************************************************************/
class RsmIndirectCovarianceB : public RsmIndirectCovariance,
			       public virtual RsmBParameterDesc {
public:
  RsmIndirectCovarianceB(const std::string& Image_identifier="",
			const std::string& Rsm_support_data_edition="fake-1",
			const std::string& Triangulation_id="")
    : RsmIndirectCovariance(Image_identifier, Rsm_support_data_edition,
			    Triangulation_id),
      cvdate("20200707")
  { }
  RsmIndirectCovarianceB(const ImageGroundConnection& Igc,
			  double H_min, double H_max,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="");
  RsmIndirectCovarianceB(const RsmIndirectCovarianceB& V);
  virtual ~RsmIndirectCovarianceB() {}
  
  std::string tre_string() const;
  static boost::shared_ptr<RsmIndirectCovarianceB>
  read_tre_string(const std::string& Tre_in);
  virtual void print(std::ostream& Os) const;
  const std::string& covariance_version_date() const { return cvdate; }
  void covariance_version_date(const std::string& V) { cvdate = V; }
  bool has_indirect_error_covariance() const
  { return subgroup_.size() != 0; }
  bool has_unmodeled_error_covariance() const { return bool(ucov_); }
  virtual bool has_ground_coordinate_parameter() const
  { return RsmBParameterDesc::has_ground_coordinate_parameter(); }
  virtual int naif_code() const { return RsmBParameterDesc::naif_code(); }
  virtual void naif_code(int Naif_code)
  { RsmBParameterDesc::naif_code(Naif_code); }
  
  const std::vector<boost::shared_ptr<RsmBSubgroup> >& subgroup() const
  { return subgroup_; }
  void subgroup(const std::vector<boost::shared_ptr<RsmBSubgroup> >& V)
  { subgroup_ = V; }
  const RsmBSubgroup& subgroup_i(int I) const
  { range_check(I, 0, (int) subgroup_.size());
    return *subgroup_[I];
  }
  void add_subgroup(const boost::shared_ptr<RsmBSubgroup>& V)
  {
    subgroup_.push_back(V);
  }
  const boost::shared_ptr<RsmBUnmodeledCovariance>&
  unmodeled_covariance() const { return ucov_; }
  void unmodeled_covariance(const boost::shared_ptr<RsmBUnmodeledCovariance>& V)
  { ucov_ = V; }
  int number_original_parameter() const;
  const blitz::Array<double, 2> mapping_matrix() const
  { return m_; }
  void mapping_matrix(const blitz::Array<double, 2>& V)
  { m_.reference(V.copy()); }
  void check_consistent() const;
private:
  std::string cvdate;
  std::vector<boost::shared_ptr<RsmBSubgroup> > subgroup_;
  blitz::Array<double, 2> m_;
  boost::shared_ptr<RsmBUnmodeledCovariance> ucov_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmBSubgroup);
GEOCAL_EXPORT_KEY(RsmBUnmodeledCovariance);
GEOCAL_EXPORT_KEY(RsmIndirectCovarianceB);
#endif
