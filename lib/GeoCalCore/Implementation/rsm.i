// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm.h"
%}
%base_import(generic_object)
%import "coordinate_converter.i"
%import "rsm_base.i"
%import "image_ground_connection.i"

%geocal_shared_ptr(GeoCal::Rsm);

namespace GeoCal {
class Rsm : public GenericObject {
public:
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
	   double Max_height,
	   int Nline = 20, int Nsample = 20, int Nheight = 20,
	   bool Skip_masked_point = false,
	   bool Ignore_error = false);
  %python_attribute(rsm_base, boost::shared_ptr<RsmBase>);
  %python_attribute(coordinate_converter,
		    boost::shared_ptr<CoordinateConverter>);
  
  %pickle_serialization()
};
}
