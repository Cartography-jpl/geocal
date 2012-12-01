// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "apply_mask.h"
%}

%geocal_shared_ptr(ApplyMask);

namespace GeoCal {
class ApplyMask : public CalcRaster {
public:
  ApplyMask(const boost::shared_ptr<RasterImage>& Data,
	    const boost::shared_ptr<RasterImage>& M,
	    int Good_value = 0);
  %python_attribute(raw_data, boost::shared_ptr<RasterImage>)
  %python_attribute(mask, boost::shared_ptr<RasterImage>)
  %python_attribute(good_value, int)
  %pickle_init(self.raw_data, self.mask, self.good_value)
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

}
