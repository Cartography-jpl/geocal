#ifndef AIRMSPI_ORBIT_H
#define AIRMSPI_ORBIT_H
#include "orbit.h"		// Definition of Orbit
#include "gdal_raster_image.h"	// Definition of GdalRasterImage

namespace GeoCal {
/****************************************************************//**
  This reads the AirMSPI orbit file.

  As an implementation detail, we use GDAL to read the data. This
  isn't really an image, but by doing this we don't need to bother
  developing all the mechanism for reading/writing HDF files. May
  want to revisit this if we actually do develop that, since there 
  is no reason other than convenience to use GDAL here.
*******************************************************************/

class AirMspiOrbit {
public:
  AirMspiOrbit(const std::string& Fname);
private:
  GdalRasterImage data;
};
}
#endif

