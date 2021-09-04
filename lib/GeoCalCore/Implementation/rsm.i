// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm.h"
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%base_import(with_parameter)
%import "coordinate_converter.i"
%import "rsm_base.i"
%import "rsm_id.i"
%import "rsm_direct_covariance.i"
%import "rsm_indirect_covariance.i"
%import "rsm_adjustable_parameter.i"
%import "image_ground_connection.i"

%geocal_shared_ptr(GeoCal::Rsm);

namespace GeoCal {
class Rsm : public WithParameterNested {
public:
  Rsm(const boost::shared_ptr<RsmId>& Rsm_id);
  Rsm(const boost::shared_ptr<RsmBase>& Rp,
      const boost::shared_ptr<CoordinateConverter>& Cconv);
  std::string print_to_string() const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate
    (const ImageCoordinate& Ic, const Dem& D) const;
  boost::shared_ptr<GroundCoordinate> Rsm::ground_intersection
  (const ImageCoordinate& Ic, const Dem& D, double Z_accuracy=1e-2) const;
  boost::shared_ptr<GroundCoordinate> polish_intersection(const ImageCoordinate& Ic, const Dem& D, const GroundCoordinate& Surfp, double Z_accuracy=1e-2) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate_z
    (const ImageCoordinate& Ic, double Z) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate_approx_height
    (const ImageCoordinate& Ic, double H) const;
  void image_coordinate(const GroundCoordinate& Gc, ImageCoordinate& OUTPUT,
			bool& OUTPUT) const;
  void image_coordinate(double X, double Y, double Z, ImageCoordinate& OUTPUT,
			bool& OUTPUT) const;
  blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;
  blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;

  void fit(const ImageGroundConnection& Igc, double Min_height,
	   double Max_height);
  void fill_in_ground_domain_vertex(const ImageGroundConnection& Igc,
				    double Min_height, double Max_height);
  void compare_igc(const ImageGroundConnection& Igc, int Number_line_spacing,
		   int Number_sample_spacing, double Height,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT)
    const;
  blitz::Array<double, 2> mapping_matrix(const ImageGroundConnection& Igc,
					 double Min_height, double Max_height,
					 int Nline_fit = 20,
					 int Nsample_fit = 20,
					 int Nheight_fit = 20,
					 bool Ignore_igc_error_in_fit = false);
  bool check_zero_crossing(double Grid_spacing = 0.01) const;
  %python_attribute(rsm_id, boost::shared_ptr<RsmId>);
  %python_attribute_with_set(rsm_direct_covariance,
		    boost::shared_ptr<RsmDirectCovariance>);
  %python_attribute_with_set(rsm_indirect_covariance,
		    boost::shared_ptr<RsmIndirectCovariance>);
  %python_attribute_with_set(rsm_adjustable_parameter,
		    boost::shared_ptr<RsmAdjustableParameter>);
  %python_attribute_with_set(rsm_base, boost::shared_ptr<RsmBase>);
  %python_attribute_with_set(coordinate_converter,
		    boost::shared_ptr<CoordinateConverter>);
  %python_attribute_with_set(image_identifier, std::string);
  %python_attribute_with_set(rsm_support_data_edition, std::string);
  %python_attribute_with_set(naif_code, int);
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("Rsm")
