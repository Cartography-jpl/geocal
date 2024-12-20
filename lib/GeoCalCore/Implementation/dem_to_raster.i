// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "dem_to_raster.h"
#include "image_ground_connection.h"
%}
%base_import(calc_raster)
%geocal_shared_ptr(GeoCal::DemToRaster);
namespace GeoCal {
class DemToRaster : public CalcRaster {
public:
  DemToRaster(const boost::shared_ptr<Dem>& D, const MapInfo& Mi);
  %pickle_serialization();
protected:
  virtual void calc(int Lstart, int Sstart) const;
};
}

// List of things "import *" will include
%python_export("DemToRaster")
