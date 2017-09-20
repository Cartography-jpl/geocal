#include "mola_dem_file.h"
#include "geocal_serialize_support.h"
#include "planet_coordinate.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void MolaDemFile::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GdalDem);
}

GEOCAL_IMPLEMENT(MolaDemFile);
#endif


double MolaDemFile::height_reference_surface(const GroundCoordinate& Gp) 
    const
{
  // The 3396000 value comes from the label in the PDS file, it is the
  // offset
  double radius = GdalDem::height_reference_surface(Gp) + 3396000;
  return radius - Planetocentric(Gp).planet_radius();
}

