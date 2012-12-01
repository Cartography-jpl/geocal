// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "calc_map_projected.h"
%}

%geocal_shared_ptr(CalcMapProjected);

namespace GeoCal {
class CalcMapProjected : public RasterImageVariable {
public:
  void write_image(RasterImage& Out, int Grid_spacing = 1) const;
  void write_multiple(const 
     std::vector<boost::shared_ptr<RasterImage> >& Out) const;
  void write_multiple(const 
    std::vector<boost::shared_ptr<RasterImage> >& Out, int Grid_spacing)
    const;
  virtual void write(int Line, int Sample, int Val) = 0;
};
}
