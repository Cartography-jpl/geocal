// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_base.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"  
%}
%base_import(generic_object)
%import "image_coordinate.i"
%import "image_ground_connection.i"
%import "coordinate_converter.i"

%geocal_shared_ptr(GeoCal::RsmBase);

namespace GeoCal {
class RsmBase : public GenericObject {
public:
  RsmBase(const std::string& Image_identifier="",
	  const std::string& Rsm_support_data_edition="fake-1");
  virtual std::string print_to_string() const;
  virtual boost::shared_ptr<RsmBase> clone() const;
  virtual ImageCoordinate image_coordinate(double X, double Y, double Z) const;
  virtual blitz::Array<double, 2> image_coordinate
    (const blitz::Array<double, 1>& X,
     const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const;
  virtual blitz::Array<double, 3> image_coordinate
    (const blitz::Array<double, 2>& X,
     const blitz::Array<double, 2>& Y, const blitz::Array<double, 2>& Z) const;
  virtual blitz::Array<double, 4> image_coordinate
    (const blitz::Array<double, 3>& X,
     const blitz::Array<double, 3>& Y, const blitz::Array<double, 3>& Z) const;
  virtual blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;
  virtual void initial_guess(double Line, double Sample, double Z,
			     double& X_guess, double& Y_guess) const;
  virtual double initial_guess_z(double Line, double Sample) const;
  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample) = 0;
  static blitz::Array<double, 4> generate_data
  (const ImageGroundConnection& Igc,
   const CoordinateConverter& Cconv,
   double Min_x, double Max_x,
   double Min_y, double Max_y,
   double Min_z, double Max_z,
   int Numx, int Numy, int Numz);
  %python_attribute(min_line, int);
  %python_attribute(max_line, int);
  %python_attribute(min_sample, int);
  %python_attribute(max_sample, int);
  %python_attribute(min_x, double);
  %python_attribute(max_x, double);
  %python_attribute(min_y, double);
  %python_attribute(max_y, double);
  %python_attribute(min_z, double);
  %python_attribute(max_z, double);
  %python_attribute_with_set(image_identifier, std::string);
  %python_attribute_with_set(rsm_suport_data_edition, std::string);
  %python_attribute_with_set(row_section_number, int)
  %python_attribute_with_set(col_section_number, int)
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("RsmBase")
