// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "refraction_sdp.h"
%}
%base_import(refraction)
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::RefractionSdp);
namespace GeoCal {
// Not sure why swig thinks this is abstract, but it does.
%feature("notabstract") RefractionSdp;
class RefractionSdp : public Refraction {
public:
  RefractionSdp(double Altitude, double Latitude, 
		double Index_refraction_surface = -1);
  %python_attribute(altitude, double)
  %python_attribute(latitude, double)
  %python_attribute_with_set(index_refraction_surface, double)
  double surface_zenith(double Space_zenith) const;
  double displacement(double Space_zenith) const;
  virtual CartesianFixedLookVector
  refraction_apply(const GroundCoordinate& Spacecraft_pos,
  		   const GroundCoordinate& Gc_no_refraction) const;
  virtual CartesianFixedLookVector
  refraction_reverse(const GroundCoordinate& Spacecraft_pos,
  		     const GroundCoordinate& Gc_with_refraction) const;
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("RefractionSdp")
