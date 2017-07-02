// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "dem_map_info.h"
%}

%base_import(dem)
%import "geocal_datum.i"
%import "map_info.i"
%geocal_shared_ptr(GeoCal::DemMapInfo);

namespace GeoCal {
class DemMapInfo : public Dem {
public:
  %python_attribute2(datum, datum_ptr, boost::shared_ptr<Datum>)
  %python_attribute(map_info, MapInfo)
  virtual double distance_to_surface(const GroundCoordinate& Gp) const;
  double height_datum(const GroundCoordinate& Gp) const;
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const;
  %python_attribute(outside_dem_is_error, bool);
  %python_attribute(naif_code, int);
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const;
protected:
  virtual double elevation(int Y_index, int X_index) const = 0;
};
}
