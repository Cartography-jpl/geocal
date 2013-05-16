// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "orbit_map_projected.h"
%}


%geocal_shared_ptr(GeoCal::OrbitMapProjected);
namespace GeoCal {
class OrbitMapProjected : public CalcMapProjected {
public:
  OrbitMapProjected(const MapInfo& Mi, const boost::shared_ptr<OrbitData>& Od,
		    const boost::shared_ptr<RasterImage>& R,
		    const boost::shared_ptr<Camera>& C, 
		    const boost::shared_ptr<Dem>& D,
		    int Band = 0, int Avg_fact = -1);
  OrbitMapProjected(const MapInfo& Mi, const boost::shared_ptr<OrbitData>& Od,
		    const std::vector<boost::shared_ptr<RasterImage> >& R,
		    const boost::shared_ptr<Camera>& C, 
		    const boost::shared_ptr<Dem>& D, int Band = 0,
		    int Avg_fact = -1);
  virtual void write(int Line, int Sample, int Val);
};
}
