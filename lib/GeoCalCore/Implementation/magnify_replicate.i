// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "magnify_replicate.h"
%}
%base_import(calc_raster)
%geocal_shared_ptr(GeoCal::MagnifyReplicate);

namespace GeoCal {
class MagnifyReplicate : public CalcRaster {
public:
  MagnifyReplicate(const boost::shared_ptr<RasterImage>& Data,
		   int Magfactor, int Number_tile = 4);
  %python_attribute2(raw_data, raw_data_ptr, boost::shared_ptr<RasterImage>)
  %python_attribute(magfactor, int)
  %pickle_init(1, self.raw_data, self.magfactor, self.number_tile)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}

// List of things "import *" will include
%python_export("MagnifyReplicate")
