// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "igc_map_projected.h"
%}


%geocal_shared_ptr(GeoCal::IgcMapProjected);
namespace GeoCal {
class IgcMapProjected : public CalcMapProjected {
public:
  IgcMapProjected(const MapInfo& Mi, 
		  const boost::shared_ptr<ImageGroundConnection>& Igc,
		  const boost::shared_ptr<RasterImage>& R,
		  int Avg_fact = -1,
		  bool Read_into_memory = true);
  IgcMapProjected(const MapInfo& Mi, 
		  const boost::shared_ptr<ImageGroundConnection>& Igc,
		  int Avg_fact = -1,
		  bool Read_into_memory = true);
  virtual void write(int Line, int Sample, int Val);
};
}
