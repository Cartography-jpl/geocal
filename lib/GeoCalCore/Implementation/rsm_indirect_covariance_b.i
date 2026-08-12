// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_indirect_covariance_b.h"
#include "rsm_id.h"
#include "coordinate_converter.h"  
#include "image_ground_connection.h"  
%}
%base_import(rsm_indirect_covariance)
%base_import(rsm_adjustable_parameter_b)
%geocal_shared_ptr(GeoCal::RsmIndirectCovarianceB);
%geocal_shared_ptr(GeoCal::RsmBSubgroup);
%geocal_shared_ptr(GeoCal::RsmBUnmodeledCovariance);
%import "local_rectangular_coordinate.i"
%import "image_ground_connection.i"
%import "rsm_id.i"

namespace GeoCal {
class RsmBSubgroup : public GenericObject {
public:
  RsmBSubgroup(const blitz::Array<double, 2> Cov,
	       int Time_correlation_domain_flag,
	       const blitz::Array<double, 2> Time_correlation);
  RsmBSubgroup(const blitz::Array<double, 2> Cov,
	       int Time_correlation_domain_flag,
	       double A, double alpha, double beta, double T);
  RsmBSubgroup();
  %python_attribute(number_parameter, int);
  %python_attribute(is_csm_correlation, bool);
  %python_attribute(covariance , blitz::Array<double, 2>);
  %python_attribute(time_correlation , blitz::Array<double, 2>);
  %python_attribute(time_correlation_domain_flag , int);
  std::string print_to_string() const;
  %pickle_serialization();
}; 

class RsmBUnmodeledCovariance : public GenericObject {
public:
  RsmBUnmodeledCovariance(const blitz::Array<double, 2> Cov,
			  const blitz::Array<double, 2> Row_time_correlation,
			  const blitz::Array<double, 2> Cow_time_correlation);
  RsmBUnmodeledCovariance(const blitz::Array<double, 2> Cov,
  	       double Row_A, double Row_alpha, double Row_beta, double Row_T,
	       double Col_A, double Col_alpha, double Col_beta, double Col_T);
  RsmBUnmodeledCovariance();
  %python_attribute(is_csm_correlation, bool);
  %python_attribute(covariance , blitz::Array<double, 2>);
  %python_attribute(row_time_correlation , blitz::Array<double, 2>);
  %python_attribute(col_time_correlation , blitz::Array<double, 2>);
  std::string print_to_string() const;
  %pickle_serialization();
}; 
  
class RsmIndirectCovarianceB : public RsmIndirectCovariance,
			       public RsmBParameterDesc {
public:
  RsmIndirectCovarianceB(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="");
  RsmIndirectCovarianceB(const ImageGroundConnection& Igc,
			  double H_min, double H_max,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="");
  std::string tre_string() const;
  static boost::shared_ptr<RsmIndirectCovarianceB>
  read_tre_string(const std::string& Tre_in);
  %python_attribute_with_set(covariance_version_date, std::string);
  %python_attribute(has_indirect_error_covariance, bool);
  %python_attribute(has_unmodeled_error_covariance, bool);
  %python_attribute(number_original_parameter, int);
  %python_attribute_with_set(subgroup, std::vector<boost::shared_ptr<RsmBSubgroup> >);
  %python_attribute_with_set(mapping_matrix, blitz::Array<double, 2>);
  %python_attribute_with_set(unmodeled_covariance, boost::shared_ptr<RsmBUnmodeledCovariance>);
  const RsmBSubgroup& subgroup_i(int I) const;
  void add_subgroup(const boost::shared_ptr<RsmBSubgroup>& V);
  void check_consistent() const;
  %pickle_serialization()
};
}

%template(Vector_RsmBSubgroup) std::vector<boost::shared_ptr<GeoCal::RsmBSubgroup> >;

// List of things "import *" will include
%python_export("RsmIndirectCovarianceB", "RsmBSubgroup", "Vector_RsmBSubgroup",
	       "RsmBUnmodeledCovariance")
