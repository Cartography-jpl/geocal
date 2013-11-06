#ifndef IGC_MAP_PROJECTED_H
#define IGC_MAP_PROJECTED_H
#include "image_ground_connection.h"
#include "calc_raster.h"

namespace GeoCal {

/****************************************************************//**
  The functionality that is common between IgcMapProjected and
  IgcMapProjectedMultiBand
*******************************************************************/

class IgcMapProjectedBase {
public:
  virtual ~IgcMapProjectedBase() {}
  const boost::shared_ptr<ImageGroundConnection>& igc_original() const
  { return igc_original_; }
  int avg_factor() const { return avg_factor_; }
  int grid_spacing() const { return grid_spacing_; }
  bool read_into_memory() const { return read_into_memory_; }
protected:
  IgcMapProjectedBase(const MapInfo& Mi, 
		      const boost::shared_ptr<ImageGroundConnection>& Igc,
		      int Grid_spacing,
		      int Avg_fact,
		      bool Read_into_memory);
  boost::shared_ptr<ImageGroundConnection> igc_original_;
  boost::shared_ptr<ImageGroundConnection> igc_;
  int avg_factor_;
  int grid_spacing_;
  MapInfo mi;
  bool read_into_memory_;
  mutable blitz::Array<double, 2> ic_line, ic_sample;
  void interpolate_ic(int Start_line, int Start_sample) const;
};

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

class IgcMapProjected : public CalcRaster, public IgcMapProjectedBase {
public:
  IgcMapProjected(const MapInfo& Mi, 
		  const boost::shared_ptr<ImageGroundConnection>& Igc,
		  int Grid_spacing = 1,
		  int Avg_fact = -1,
		  bool Read_into_memory = true,
		  int Number_tile_line = -1,
		  int Number_tile_sample = -1);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~IgcMapProjected() {}
  virtual void print(std::ostream& Os) const
  {
    Os << "IgcMapProjected:\n"
       << "  Avg_factor:   " << avg_factor() << "\n"
       << "  Grid spacing: " << grid_spacing() << "\n"
       << "  Map info:\n" 
       << map_info() << "\n"
       << "  Image ground connection:\n" 
       << *igc_ << "\n";
  }
protected:
  virtual void calc(int Lstart, int Sstart) const;
private:
  void calc_no_grid(int Lstart, int Sstart) const;
  void calc_grid(int Lstart, int Sstart) const;
};

}
#endif
