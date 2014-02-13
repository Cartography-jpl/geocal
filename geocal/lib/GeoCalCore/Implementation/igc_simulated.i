// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "igc_simulated.h"
%}
%base_import(calc_raster)
%import "image_ground_connection.i"
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::IgcSimulated);
namespace GeoCal {
class IgcSimulated : public CalcRaster {
public:
  IgcSimulated(const boost::shared_ptr<ImageGroundConnection>& Igc,
	       const boost::shared_ptr<RasterImage>& Map_projected_image, 
	       int Avg_fact = -1,
	       bool Read_into_memory = true,
	       int Number_tile_line = -1,
	       int Number_tile_sample = -1,
	       double Fill_value = 0.0
	       );
  %python_attribute(igc, boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(avg_factor, int)
  %python_attribute(fill_value, double)
  %python_attribute(read_into_memory, bool)
  %python_attribute(map_projected_image, boost::shared_ptr<RasterImage>)
     %pickle_init(1, self.igc, self.map_projected_image,
	       self.avg_factor, self.read_into_memory, self.number_tile_line,
	       self.number_tile_sample, self.fill_value)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}
