// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "memory_multi_band.h"
%}

%geocal_shared_ptr(GeoCal::MemoryMultiBand);

namespace GeoCal {
class MemoryMultiBand : public RasterImageMultiBandVariable {
public:
  MemoryMultiBand(const RasterImageMultiBand& R);
  %python_attribute(data, blitz::Array<int, 3>)
};
}
