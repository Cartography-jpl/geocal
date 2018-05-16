// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "apply_mask.h"
%}
%base_import(calc_raster)
%geocal_shared_ptr(GeoCal::ApplyMask);

namespace GeoCal {
class ApplyMask : public CalcRaster {
public:
  ApplyMask(const boost::shared_ptr<RasterImage>& Data,
	    const boost::shared_ptr<RasterImage>& M,
	    int Good_value = 0);
  %python_attribute(raw_data, boost::shared_ptr<RasterImage>)
  %python_attribute(mask, boost::shared_ptr<RasterImage>)
  %python_attribute(good_value, int)
  %pickle_init(1, self.raw_data, self.mask, self.good_value)
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

}

// List of things "import *" will include
%python_export("ApplyMask")
