// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "simple_dem.h"
%}


%geocal_shared_ptr(SimpleDem);
namespace GeoCal {
class SimpleDem : public Dem {
public:
  SimpleDem(double H = 0);
  virtual double distance_to_surface(const GroundCoordinate& Gp) const;
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const;
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const;
  %python_attribute_with_set(h, double)
  %pickle_init(self.h)
};
}
