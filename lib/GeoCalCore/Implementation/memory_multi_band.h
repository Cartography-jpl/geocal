#ifndef MEMORY_MULTI_BAND_H
#define MEMORY_MULTI_BAND_H
#include "raster_image_multi_band_variable.h"

namespace GeoCal {
/****************************************************************//**
  This reads a RasterImageMultiBand into memory, and allow access that
  memory. 
*******************************************************************/

class MemoryMultiBand : public RasterImageMultiBandVariable {
public:
  MemoryMultiBand(const RasterImageMultiBand& R);
  virtual ~MemoryMultiBand() {}

//-----------------------------------------------------------------------
/// This the underlying data. This is ordered by band, line, and sample.
//-----------------------------------------------------------------------

  const blitz::Array<int, 3>& data() const {return data_;}

  virtual void print(std::ostream& Os) const { Os << "MemoryRasterImageBand"; }
private:
  blitz::Array<int, 3> data_;
};

}
#endif
