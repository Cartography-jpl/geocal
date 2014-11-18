#include "datum_geoid96.h"
#include <stdlib.h>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. You can pass the datum file to read, but if you leave
/// this as blank we use the file ${AFIDS_VDEV_DATA}/EGM96_20_x100.HLF.
//-----------------------------------------------------------------------

DatumGeoid96::DatumGeoid96(const std::string& Fname)
 : fname(Fname)
{
  if(fname == "") {
    char* t = getenv("AFIDS_VDEV_DATA");
    if(!t)
      throw Exception("Need to either pass in the EGM96 file name or set the environment variable AFIDS_VDEV_DATA");
    fname = t + std::string("/EGM96_20_x100.HLF");
  }
  data.reset(new VicarRasterImage(fname));
}

//-----------------------------------------------------------------------
/// Return the undulation for a given location.
//-----------------------------------------------------------------------

double DatumGeoid96::undulation(const GroundCoordinate& Gc) const
{
  // Data is scaled by 100 to fit into a integer. So we multiple by
  // 0.01 to get back the orginal data.
  return data->interpolate(data->coordinate(Gc)) * 0.01;
}

double DatumGeoid96::undulation(const Geodetic& Gc) const
{
  // Data is scaled by 100 to fit into a integer. So we multiple by
  // 0.01 to get back the orginal data.
  return data->interpolate(data->coordinate(Gc)) * 0.01;
}

//-----------------------------------------------------------------------
/// Print a description of the class.
//-----------------------------------------------------------------------

void DatumGeoid96::print(std::ostream& Os) const
{
  Os << "Geoid 96 Datum\n"
     << "  File: " << fname << "\n";
}


