// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "igc_msp.h"
%}
%base_import(image_ground_connection)
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::IgcMsp);
namespace GeoCal {
bool have_msp_supported();
  
class IgcMsp : public ImageGroundConnectionCopy {
public:
  IgcMsp(const std::string& Fname);
  IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& D,
	 int Image_index = 0);
  IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& D,
	 int Image_index,
	 const std::string& Plugin_name, const std::string& Model_name);
  %python_attribute(covariance, blitz::Array<double, 2>);
  blitz::Array<double, 2> IgcMsp::joint_covariance(const IgcMsp& igc2) const;
  static void msp_print_plugin_list();
  static void msp_register_plugin(const std::string& Plugin_name);
  static std::vector<std::string> msp_plugin_list();
  static std::vector<std::string> msp_model_list(const std::string& Plugin);
  static std::vector<std::string> image_ids(const std::string& Fname);
  blitz::Array<double, 1> sensor_velocity(const ImageCoordinate& Ic) const;
  std::string generate_rsm_tre(const std::string& Report = "",
			       const std::string& Rsm_config = "") const;
  void ground_coordinate_with_cov(const ImageCoordinate& Ic,
				  const blitz::Array<double, 2>& Ic_cov,
				  double H,
				  double H_var,
				  boost::shared_ptr<GroundCoordinate>& OUTPUT,
				  blitz::Array<double, 2>& OUTPUT) const;
  void ce90_le90(const ImageCoordinate& Ic, double H,
		 double& OUTPUT, double& OUTPUT) const;
  %python_attribute(family, std::string);
  %python_attribute(version, std::string);
  %python_attribute(model_name, std::string);
  %python_attribute(file_name, std::string);
  %python_attribute(image_index, int);
  %python_attribute(pedigree, std::string);
  %python_attribute(image_identifer, std::string);
  %python_attribute(sensor_identifer, std::string);
  %python_attribute(platform_identifer, std::string);
  %python_attribute(collection_identifer, std::string);
  %python_attribute(trajectory_identifer, std::string);
  %python_attribute(sensor_type, std::string);
  %python_attribute(sensor_mode, std::string);
  %python_attribute(reference_date_time, std::string);
  %pickle_serialization();
};

}

// List of things "import *" will include
%python_export("IgcMsp", "have_msp_supported")
