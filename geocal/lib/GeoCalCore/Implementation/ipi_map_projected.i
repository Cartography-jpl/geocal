// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "ipi_map_projected.h"
%}


%geocal_shared_ptr(IpiMapProjected);
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
