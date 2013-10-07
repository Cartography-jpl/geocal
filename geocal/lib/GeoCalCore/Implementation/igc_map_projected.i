// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "igc_map_projected.h"
%}
%base_import(calc_map_projected)
%import "image_ground_connection.i"
%import "raster_image.i"
%import "map_info.i"
%geocal_shared_ptr(GeoCal::IgcMapProjected);
namespace GeoCal {
class IgcMapProjected : public CalcMapProjected {
public:
  IgcMapProjected(const MapInfo& Mi, 
		  const boost::shared_ptr<ImageGroundConnection>& Igc,
		  int Avg_fact = -1,
		  bool Read_into_memory = true);
  virtual void write(int Line, int Sample, int Val);
};
}
