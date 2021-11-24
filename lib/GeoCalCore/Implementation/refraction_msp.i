// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "refraction_msp.h"
%}
%base_import(refraction)
%import "ground_coordinate.i"
%import "look_vector.i"
%geocal_shared_ptr(GeoCal::RefractionMsp);
namespace GeoCal {
class RefractionMsp : public Refraction {
public:
  RefractionMsp(double Wavelength_micrometer=0.6,
		double Temperature_kelvin = -1,
		double Pressure_millibar = -1);
  %python_attribute(wavelength, double);
  %python_attribute(temperature, double);
  %python_attribute(pressure, double)
  CartesianFixedLookVector refraction_calc
  (const GroundCoordinate& Spacecraft_pos,
   const GroundCoordinate& Gc_no_refraction) const;
  virtual boost::shared_ptr<GroundCoordinate>
  refraction_apply(const GroundCoordinate& Spacecraft_pos,
  		   const GroundCoordinate& Gc_no_refraction) const;
  virtual boost::shared_ptr<GroundCoordinate>
  refraction_reverse(const GroundCoordinate& Spacecraft_pos,
  		     const GroundCoordinate& Gc_with_refraction) const;
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("RefractionMsp")
