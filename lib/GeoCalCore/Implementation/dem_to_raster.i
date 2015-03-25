// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "dem_to_raster.h"
%}
%base_import(calc_raster)
%geocal_shared_ptr(GeoCal::DemToRaster);
namespace GeoCal {
class DemToRaster : public CalcRaster {
public:
  DemToRaster(const boost::shared_ptr<Dem>& D, const MapInfo& Mi);
protected:
  virtual void calc(int Lstart, int Sstart) const;
};
}



