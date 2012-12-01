// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "magnify_replicate.h"
%}

%geocal_shared_ptr(MagnifyReplicate);

namespace GeoCal {
class MagnifyReplicate : public CalcRaster {
public:
  MagnifyReplicate(const boost::shared_ptr<RasterImage>& Data,
		   int Magfactor, int Number_tile = 4);
  %python_attribute2(raw_data, raw_data_ptr, boost::shared_ptr<RasterImage>)
  %python_attribute(magfactor, int)
  %pickle_init(self.raw_data, self.magfactor, self.number_tile)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}
