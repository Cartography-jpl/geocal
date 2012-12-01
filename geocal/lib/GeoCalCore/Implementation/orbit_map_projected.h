#ifndef ORBIT_MAP_PROJECTED_H
#define ORBIT_MAP_PROJECTED_H
#include "orbit.h"
#include "calc_map_projected.h"
#include <boost/multi_array.hpp>

namespace GeoCal {
/****************************************************************//**
  Note that this class is deprecated, it is redundant with
  IgcMapProjected just passing in a OrbitDataGroundConnection. But
  leave this is place now until we are ready to remove this.
  
  This is a RasterImage that has been map projected using 
  OrbitData and a Camera.

  To do this we do two steps:

  1. We calculate roughly what the difference in resolution is between
     the original data and final MapInfo. We do this by looking at the
     center pixel of the original data and the pixel +1 in line and
     sample. We then use RasterAveraged to average the original data
     to roughly the resolution of the final MapInfo. If the final
     MapInfo is near the same resolution as the original, or if it has
     a higher resolution, then we don't do any averaging.
     Alternatively, you can pass in the averaging factor.

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

class OrbitMapProjected : public CalcMapProjected {
public:
  OrbitMapProjected(const MapInfo& Mi, const boost::shared_ptr<OrbitData>& Od,
		    const boost::shared_ptr<RasterImage>& R,
		    const boost::shared_ptr<Camera>& C, 
		    const boost::shared_ptr<Dem>& D, int Band = 0,
		    int Avg_fact = -1,
		    bool Read_into_memory = true);
  OrbitMapProjected(const MapInfo& Mi, const boost::shared_ptr<OrbitData>& Od,
		    const std::vector<boost::shared_ptr<RasterImage> >& R,
		    const boost::shared_ptr<Camera>& C, 
		    const boost::shared_ptr<Dem>& D, int Band = 0,
		    int Avg_fact = -1,
		    bool Read_into_memory = true);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OrbitMapProjected() {}

  virtual void print(std::ostream& Os) const;
protected:
//-----------------------------------------------------------------------
/// Return calculated image coordinates for given ground
/// location. This should be the image coordinates of the averaged
/// data, i.e., you need to include line_avg and sample_avg in your
/// calculation. It is ok if this is out of the range of the image, we
/// check for that. In particular, for calculations that may fail for
/// out of range data (e.g., Ipi), you can just return
/// ImageCoordinate(-1, -1) (you should not trigger an exception).
//-----------------------------------------------------------------------

  virtual ImageCoordinate calc_image_coordinates(const GroundCoordinate& Gc) 
    const 
  {
    FrameCoordinate fc = od->frame_coordinate(Gc, *cam, band);
    return ImageCoordinate(fc.line / line_avg(), fc.sample / samp_avg()); 
  }
private:
  int band;
  boost::shared_ptr<OrbitData> od;
  boost::shared_ptr<Camera> cam;
};

}
#endif
