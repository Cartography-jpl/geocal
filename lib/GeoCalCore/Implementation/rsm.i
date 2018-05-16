// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm.h"
%}
%base_import(generic_object)
%import "coordinate_converter.i"
%import "rsm_base.i"
%import "rsm_id.i"
%import "rsm_direct_covariance.i"
%import "rsm_indirect_covariance.i"
%import "rsm_adjustable_parameter.i"
%import "image_ground_connection.i"

%geocal_shared_ptr(GeoCal::Rsm);

namespace GeoCal {
class Rsm : public GenericObject {
public:
  Rsm(const boost::shared_ptr<RsmId>& Rsm_id);
  Rsm(const boost::shared_ptr<RsmBase>& Rp,
      const boost::shared_ptr<CoordinateConverter>& Cconv);
  std::string print_to_string() const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate
    (const ImageCoordinate& Ic, const Dem& D) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate
    (const ImageCoordinate& Ic, double Z) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate_approx_height
    (const ImageCoordinate& Ic, double H) const;
  ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const;
  ImageCoordinate image_coordinate(double X, double Y, double Z) const;
  blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;

  void fit(const ImageGroundConnection& Igc, double Min_height,
	   double Max_height);
  void fill_in_ground_domain_vertex(double Min_height, double Max_height);
  void compare_igc(const ImageGroundConnection& Igc, int Number_line_spacing,
		   int Number_sample_spacing, double Height,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT,
		   blitz::Array<double, 2>& OUTPUT)
    const;
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
  %python_attribute_with_set(rsm_suport_data_edition, std::string);
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("Rsm")
