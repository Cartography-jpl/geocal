#include "spice_dem.h"
#include "geocal_serialize_support.h"
#include "spice_helper.h"
#include "planet_coordinate.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class Archive>
void SpiceDem::serialize(Archive & ar, const unsigned int version) 
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Dem)
    & GEOCAL_NVP_(naif_code)
    & GEOCAL_NVP_(time)
    & GEOCAL_NVP_(kernel_list);
}

GEOCAL_IMPLEMENT(SpiceDem);
#endif

boost::shared_ptr<GroundCoordinate> 
SpiceDem::surface_point(const GroundCoordinate& Gp) const
{
  kernel_list_.load_kernel();
  if(Gp.naif_code() != naif_code_) {
    Exception e;
    e << "Gp has the wrong naif_code(). Got " << Gp.naif_code()
      << " but expected " << naif_code_;
    throw e;
  }
  return SpiceHelper::latsrf(naif_code_, time_, Gp.latitude(), Gp.longitude());
}
