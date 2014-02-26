#ifndef MEMORY_DEM_H
#define MEMORY_DEM_H
#include "dem_map_info.h"
#include <boost/multi_array.hpp>

namespace GeoCal {
/****************************************************************//**
  This is a DemMapInfo that has everything stored in memory. This 
  Dem can be updated, and we can also supply the jacobian with respect
  to the Dem height values.
*******************************************************************/

class MemoryDem : public DemMapInfo {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  MemoryDem(const boost::shared_ptr<Datum>& D, const MapInfo& M, 
	    bool Outside_dem_is_error = false)
    : DemMapInfo(D, M, Outside_dem_is_error),
      elevation_data(boost::extents[M.number_y_pixel()][M.number_x_pixel()])
  {  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MemoryDem() {}

//-----------------------------------------------------------------------
/// Return height in meters relative to datum(). Note that the call is
/// in line, sample order, which means Y and then X.
//-----------------------------------------------------------------------

  virtual double elevation(int Y_index, int X_index) const
  {
    return elevation_data[Y_index][X_index];
  }

//-----------------------------------------------------------------------
/// Height in meters.
//-----------------------------------------------------------------------

  boost::multi_array<double, 2> elevation_data;

//-----------------------------------------------------------------------
/// Jacobian of how height_reference_surface would vary if we varied
/// the underlying elevation_data. This fills in a vector. We
/// index the parameters like a 1 dimensional flattened version of
/// elevation_data, plus an optional offset (used if these are only a
/// some of the parameters you are using).
///
/// You can pass in any kind of vector, as long as we can
/// assign to it by V[i] = b. One example is gmm::wsvector<double>.
//-----------------------------------------------------------------------

  template<class T> void jacobian(const GroundCoordinate& Gc, T V,
				  int parameter_offset = 0) const
  {
    double x, y;
    map_info().coordinate(Gc, x, y);
    int xi = (int) floor(x);
    int yi = (int) floor(y);
    if(xi < 0 || xi + 1 >= map_info().number_x_pixel() ||
       yi < 0 || yi + 1 >= map_info().number_y_pixel()) {
      if(outside_dem_is_error()) {
	Exception e;
	e << "Height requested outside of the range of the Dem data "
	  << "xindex: " << xi << " (max " << map_info().number_x_pixel() - 1
	  << ") yindex: " << yi << " (max " << map_info().number_y_pixel() - 1
	  << ")";
	throw e;
      } else
	return;			// No dependence
    }
    // We have 
    // e1 = elevation(yi, xi);
    // e2 = elevation(yi, xi + 1);
    // e3 = elevation(yi + 1, xi);
    // e4 = elevation(yi + 1, xi + 1);
    // This gives:
    int p1 = yi * elevation_data.shape()[1] + xi + parameter_offset;
    int p2 = p1 + 1;
    int p3 = p1 + elevation_data.shape()[1];
    int p4 = p3 + 1;
    // We have the equations:
    // t1 = e1 + (e2 - e1) * (x - xi)
    // t2 = e3 + (e4 - e3) * (x - xi)
    // r = t1 + (t2 - t1) * (y - yi)
    //
    // So,
    // dr/de1 = (1 - (y - yi)) * dt1/de1 = (1 - (y - yi)) * (1 - (x - xi))
    // dr/de2 = (1 - (y - yi)) * dt1/de2 = (1 - (y - yi)) * (x - xi)
    // dr/de3 = (y - yi) * dt2 / de3 = (y - yi) * (1 - (x - xi))
    // dr/de4 = (y - yi) * dt2 / de4 = (y - yi) * (x - xi)

    V[p1] = (1 - (y - yi)) * (1 - (x - xi));
    V[p2] = (1 - (y - yi)) * (x - xi);
    V[p3] = (y - yi) * (1 - (x - xi));
    V[p4] = (y - yi) * (x - xi);
  }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Memory DEM:\n"
       << "  Map info:\n"
       << map_info()
       << "  Datum:\n"
       << datum()
       << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }
};
}
#endif
