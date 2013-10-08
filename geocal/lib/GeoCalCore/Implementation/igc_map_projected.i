// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "igc_map_projected.h"
%}
%base_import(calc_raster)
%import "image_ground_connection.i"
%import "raster_image.i"
%import "map_info.i"
%geocal_shared_ptr(GeoCal::IgcMapProjected);
namespace GeoCal {
class IgcMapProjected : public CalcRaster {
public:
  IgcMapProjected(const MapInfo& Mi, 
		  const boost::shared_ptr<ImageGroundConnection>& Igc,
		  int Grid_spacing = 1,
		  int Avg_fact = -1,
		  bool Read_into_memory = true,
		  int Number_tile_line = -1,
		  int Number_tile_sample = -1);
  %python_attribute(igc_original, boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(avg_factor, int)
  %python_attribute(grid_spacing, int)
  %python_attribute(read_into_memory, bool)
  %pickle_init(1, self.map_info, self.igc_original, self.grid_spacing,
	       self.avg_factor, self.read_into_memory, self.number_tile_line,
	       self.number_tile_sample)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};
}
