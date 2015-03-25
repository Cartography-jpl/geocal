#ifndef DEM_TO_RASTER_H
#define DEM_TO_RASTER_H
#include "calc_raster.h"
#include "dem.h"

namespace GeoCal {
/****************************************************************//**
  This provides DEM height data as a raster image. This is useful
  for doing things like writing out a raster image covering a DEM.
*******************************************************************/
class DemToRaster : public CalcRaster {
public:
  DemToRaster(const boost::shared_ptr<Dem>& D, const MapInfo& Mi)
    : CalcRaster(Mi), dem(D)
  {
  }
  virtual ~DemToRaster() {}
  virtual void print(std::ostream& Os) const { Os << "DemToRaster"; }
protected:
  virtual void calc(int Lstart, int Sstart) const
  {
    for(int i = 0; i < data.rows(); ++i)
      for(int j = 0; j < data.cols(); ++j)
	data(i, j) = dem->height_reference_surface(*ground_coordinate(i, j));
  }
private:
  boost::shared_ptr<Dem> dem;
};
}
#endif
