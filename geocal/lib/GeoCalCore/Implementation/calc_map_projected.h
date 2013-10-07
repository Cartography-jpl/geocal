#ifndef CALC_MAP_PROJECTED_H
#define CALC_MAP_PROJECTED_H
#include "raster_image_variable.h"
#include "image_ground_connection.h"
#include <boost/multi_array.hpp>
#include "calc_raster.h"

namespace GeoCal {
/****************************************************************//**
  This is a RasterImage that has been map projected by a ground
  to image calculation. There are several obvious variations of this -
  calculate by OrbitData (e.g., Argus), by an Ipi (e.g., Quickbird),
  by an RPC. This base class capture the common behavior.

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

class CalcMapProjected : public CalcRaster {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~CalcMapProjected() {}

  void write_image(RasterImage& Out, int Grid_spacing = 1) const;
  void write_multiple(const 
     std::vector<boost::shared_ptr<RasterImage> >& Out) const;
  void write_multiple(const 
    std::vector<boost::shared_ptr<RasterImage> >& Out, int Grid_spacing)
    const;
  virtual void print(std::ostream& Os) const
  {
    Os << "IgcMapProjected:\n"
       << "  Map info:   " << map_info() << "\n"
       << "  Image ground connection: " << *igc_ << "\n";
  }
protected:
//-----------------------------------------------------------------------
/// Constructor. Note that derived classes should call initialize.
//-----------------------------------------------------------------------

  CalcMapProjected(const MapInfo& Mi) : CalcRaster(Mi) { }

  void initialize(const boost::shared_ptr<ImageGroundConnection>& Igc, const 
		  int Avg_fact, bool Read_into_memory);
  virtual void calc(int Lstart, int Sstart) const;
private:
  int line_avg_;
  int samp_avg_;
  boost::shared_ptr<ImageGroundConnection> igc_;
                               ///< Underlying data, possibly averaged
                               ///using RasterAveraged.
  void interpolate_ic(int Start_line, int Start_sample, int Nline, int Nsamp,
		      boost::multi_array<double, 2>& Ic_line,
		      boost::multi_array<double, 2>& Ic_sample) const;
};

}
#endif
