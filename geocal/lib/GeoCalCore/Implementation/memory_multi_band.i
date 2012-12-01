// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "memory_multi_band.h"
%}

%geocal_shared_ptr(MemoryMultiBand);

namespace GeoCal {
class MemoryMultiBand : public RasterImageMultiBandVariable {
public:
  MemoryMultiBand(const RasterImageMultiBand& R);
  %python_attribute(data, blitz::Array<int, 3>)
};
}
