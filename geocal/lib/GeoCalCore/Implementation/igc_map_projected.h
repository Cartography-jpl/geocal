#ifndef IGC_MAP_PROJECTED_H
#define IGC_MAP_PROJECTED_H
#include "image_ground_connection.h"
#include "calc_map_projected.h"

namespace GeoCal {
/****************************************************************//**
  This is a RasterImage that has been map projected using an
  ImageGroundConnection and a Camera.

  To do this we do two steps:

  1. We calculate roughly what the difference in resolution is between
     the original data and final MapInfo. We do this by looking at the
     center pixel of the original data and the pixel +1 in line and
     sample. We then use RasterAveraged to average the original data
     to roughly the resolution of the final MapInfo. If the final
     MapInfo is near the same resolution as the original, or if it has
     a higher resolution, then we don't do any averaging.
     Alternatively, you can pass in the averaging factor (include a
     value of 1 which turns this behavior off).
     
  2. We then interpolate the possibly averaged data to the final
     projection.

  It is ok if the final MapInfo contains areas outside of the original
  data. For any pixel outside of the original data, we just return a
  value of 0.

  This class calculates the data on the fly. Sometimes
  this is what you want, but if you are going to be using the
  resulting data a few times, you may want to use a
  MemoryRasterImage to generate a copy once and keep it in
  memory.
*******************************************************************/

class IgcMapProjected : public CalcMapProjected {
public:
  IgcMapProjected(const MapInfo& Mi, 
		  const boost::shared_ptr<ImageGroundConnection>& Igc,
		  const boost::shared_ptr<RasterImage>& R,
		  int Avg_fact = -1,
		  bool Read_into_memory = true);
  IgcMapProjected(const MapInfo& Mi, 
		  const boost::shared_ptr<ImageGroundConnection>& Igc,
		  int Avg_fact = -1,
		  bool Read_into_memory = true);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~IgcMapProjected() {}

  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<ImageGroundConnection> igc;
};

}
#endif
