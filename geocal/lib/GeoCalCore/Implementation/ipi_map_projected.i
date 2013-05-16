// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "ipi_map_projected.h"
%}
%base_import(calc_map_projected)
%import "ipi.i"
%import "dem.i"
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::IpiMapProjected);
namespace GeoCal {
class IpiMapProjected : public CalcMapProjected {
public:
  IpiMapProjected(const MapInfo& Mi, const boost::shared_ptr<Ipi>& I,
		  const boost::shared_ptr<RasterImage>& R,
		  const boost::shared_ptr<Dem>& D,
		  int Avg_fact = -1,
		  bool Read_into_memory = true);
  IpiMapProjected(const MapInfo& Mi, const boost::shared_ptr<Ipi>& I,
		  const std::vector<boost::shared_ptr<RasterImage> >& R,
		  const boost::shared_ptr<Dem>& D, 
		  int Avg_fact = -1,
		  bool Read_into_memory = true);
  virtual void write(int Line, int Sample, int Val);
};
}
