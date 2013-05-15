// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%{
#include "dem.h"
#include "simple_dem.h"
#include "srtm_dem.h"
#include "vicar_lite_file.h"
#include "vicar_dem.h"
#include "gdal_dem.h"
%}

%geocal_shared_ptr(GeoCal::Dem);

%shared_ptr_dynamic_list(GeoCal::Dem,
			 GeoCal::SimpleDem, 
			 GeoCal::SrtmDem,
			 GeoCal::VicarLiteDem,
			 GeoCal::VicarDem,
			 GeoCal::GdalDem);
namespace GeoCal {
class Dem {
public:
  virtual double distance_to_surface(const GroundCoordinate& Gp) const = 0;
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const = 0;
  boost::shared_ptr<CartesianFixed> intersect(const CartesianFixed& Cf,
      const CartesianFixedLookVector& Lv, double Resolution,
      double Max_height = 9000) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const = 0;
  std::string print_to_string() const;
};
}
