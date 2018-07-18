#include "datum_geoid96.h"
#include "geocal_serialize_support.h"
#include <stdlib.h>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void DatumGeoid96::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(Datum);
  GEOCAL_BASE(DatumGeoid96, Datum);
  std::string fname = file_name();
  ar & GEOCAL_NVP(fname);
}

template<class Archive>
void DatumGeoid96::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Datum);
  GEOCAL_BASE(DatumGeoid96, Datum);
  std::string fname;
  ar & GEOCAL_NVP(fname);
  data.reset(new VicarRasterImage(fname));
}

GEOCAL_SPLIT_IMPLEMENT(DatumGeoid96);
#endif

//-----------------------------------------------------------------------
/// Constructor. You can pass the datum file to read, but if you leave
/// this as blank we use the file ${AFIDS_VDEV_DATA}/EGM96_20_x100.HLF.
//-----------------------------------------------------------------------

DatumGeoid96::DatumGeoid96(const std::string& Fname)
{
  std::string fname = Fname;
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
  // Data can be slightly out of range as we cross the dateline
  // (sample 0). Truncate this is we are slightly past the ends.
  ImageCoordinate ic = data->coordinate(Gc);
  if(ic.sample < 0)
    ic.sample = 0;
  if(ic.sample >= data->number_sample() - 1)
    ic.sample = data->number_sample() - 1 - 0.01;
  // Data is scaled by 100 to fit into a integer. So we multiple by
  // 0.01 to get back the orginal data.
  return data->interpolate(ic) * 0.01;
}

double DatumGeoid96::undulation(const Geodetic& Gc) const
{
  // Data can be slightly out of range as we cross the dateline
  // (sample 0). Truncate this is we are slightly past the ends.
  ImageCoordinate ic = data->coordinate(Gc);
  if(ic.sample < 0)
    ic.sample = 0;
  if(ic.sample >= data->number_sample() - 1)
    ic.sample = data->number_sample() - 1 - 0.01;
  // Data is scaled by 100 to fit into a integer. So we multiple by
  // 0.01 to get back the orginal data.
  return data->interpolate(ic) * 0.01;
}

//-----------------------------------------------------------------------
/// Print a description of the class.
//-----------------------------------------------------------------------

void DatumGeoid96::print(std::ostream& Os) const
{
  Os << "Geoid 96 Datum\n"
     << "  File: " << file_name() << "\n";
}


