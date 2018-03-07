// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"
%{
#include "simple_dem.h"
%}
%base_import(dem)
%import "geodetic.i"

namespace GeoCal {
template<class G> class SimpleDemT : public Dem {
public:
  SimpleDemT(double H = 0);
  virtual double distance_to_surface(const GroundCoordinate& Gp) const;
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const;
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const;
  %python_attribute_with_set(h, double)
  %pickle_serialization();
};
}
%geocal_shared_ptr(GeoCal::SimpleDemT<GeoCal::Geodetic>);
%template(SimpleDem) GeoCal::SimpleDemT<GeoCal::Geodetic>;
// List of things "import *" will include
%python_export("SimpleDem")
