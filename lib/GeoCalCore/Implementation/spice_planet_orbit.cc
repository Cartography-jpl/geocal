#include "spice_planet_orbit.h"
#include "geocal_serialize_support.h"
#include "spice_helper.h"
#include "planet_coordinate.h"
#include <boost/filesystem.hpp>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class Archive>
void SpicePlanetOrbit::serialize(Archive & ar, const unsigned int version) 
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(target_name)
    & GEOCAL_NVP_(spacecraft_reference_frame_name)
    & GEOCAL_NVP_(kernel_list)
    & GEOCAL_NVP_(naif_id);
}

GEOCAL_IMPLEMENT(SpicePlanetOrbit);
#endif

//-----------------------------------------------------------------------
/// Create an orbit that has position information for the Target_name
/// (e.g., "GLL", "MRO"), and pointing information in the
/// Spacecraft_reference_frame_name (e.g., "GLL_SCAN_PLANE",
/// "MRO_CTX").
///
/// Note that the Target_name is of a body (e.g., something we have a
/// spk kernel for), while the Spacecraft_reference_frame_name is for
/// a frame (e.g., something we have a fk kernel for). In addition to
/// the frame definition, you'll generally need a C kernel file (ck
/// kernel) giving the orientation of the frame with the target.
//-----------------------------------------------------------------------

SpicePlanetOrbit::SpicePlanetOrbit
(const std::string& Target_name, 
 const std::string& Spacecraft_reference_frame_name,
 int Naif_id)
  : target_name_(Target_name),
    spacecraft_reference_frame_name_(Spacecraft_reference_frame_name),
    naif_id_(Naif_id)
{
}

//-----------------------------------------------------------------------
/// Create an orbit that has position information for the Target_name
/// (e.g., "GLL", "MRO"), and pointing information in the
/// Spacecraft_reference_frame_name (e.g., "GLL_SCAN_PLANE",
/// "MRO_CTX").
///
/// Note that the Target_name is of a body (e.g., something we have a
/// spk kernel for), while the Spacecraft_reference_frame_name is for
/// a frame (e.g., something we have a fk kernel for). In addition to
/// the frame definition, you'll generally need a C kernel file (ck
/// kernel) giving the orientation of the frame with the target.
///
/// In addition, this gives a list of kernels to load. This list is
/// included in this class, so if you load a serialized version it
/// will automatically load this given list of kernels.
///
/// Note we check first if the kernel as already been loaded. If it
/// has, then we don't load it a second time. Most of the time this is
/// what you want, but if you really do want to reload the kernel then
/// this class won't be doing what you want.
//-----------------------------------------------------------------------

SpicePlanetOrbit::SpicePlanetOrbit
(const std::string& Target_name, 
 const std::string& Spacecraft_reference_frame_name,
 const std::vector<std::string>& Kernel_list,
 int Naif_id)
  : target_name_(Target_name),
    spacecraft_reference_frame_name_(Spacecraft_reference_frame_name),
    kernel_list_(Kernel_list),
    naif_id_(Naif_id)
{
}

SpicePlanetOrbit::SpicePlanetOrbit
(const std::string& Target_name, 
 const std::string& Spacecraft_reference_frame_name,
 const SpiceKernelList& Kernel_list,
 int Naif_id)
  : target_name_(Target_name),
    spacecraft_reference_frame_name_(Spacecraft_reference_frame_name),
    kernel_list_(Kernel_list),
    naif_id_(Naif_id)
{
}

// Return orbit data
boost::shared_ptr<OrbitData> SpicePlanetOrbit::orbit_data(Time T) const
{
  kernel_list_.load_kernel();
  return PlanetFixed::orbit_data(target_name_, spacecraft_reference_frame_name_,
				 T, naif_id_);
}

// Print to stream
void SpicePlanetOrbit::print(std::ostream& Os) const
{
  Os << "SpicePlanetOrbit:\n"
     << "  Target Name:                " << target_name_ << "\n"
     << "  Spacecraft Reference Frame: " << spacecraft_reference_frame_name_
     << "\n"
     << "  Body:                       " << PlanetConstant::name(naif_id_);
}


