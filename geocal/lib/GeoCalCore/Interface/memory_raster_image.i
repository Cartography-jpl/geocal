// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "memory_raster_image.h"
%}

%geocal_shared_ptr(MemoryRasterImage);

namespace GeoCal {
class MemoryRasterImage : public RasterImageVariable {
public:
  MemoryRasterImage(int Number_line, int Number_sample);
  MemoryRasterImage(const RasterImage& Img, int Number_line_to_read = -1, 
	int Number_sample_to_read = -1);
  MemoryRasterImage(const MapInfo& Mi);
  virtual void write(int Line, int Sample, int Val);
};

}
