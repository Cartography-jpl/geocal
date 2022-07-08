#ifndef SPICE_PLANET_ORBIT_H
#define SPICE_PLANET_ORBIT_H
#include "orbit.h"
#include "spice_kernel_list.h"

namespace GeoCal {
/****************************************************************//**
  This is an orbit that uses a SPICE kernel as the underlying 
  implementation.

  This class is used with planet satellites. Compare this class with
  SpiceOrbit. These could perhaps be merged at some point, but
  for right now just keep these separate. Don't have enough experience
  with these classes yet to know what the interface should be like, or
  if these really could be merged.

  Note that this combined both the space craft coordinate system and
  conversion to frame coordinates. We could split this out into a 
  separate camera model, but there doesn't seem to be much of a reason
  to do this. We could revisit this if there is ever a need to. In
  practice, this means that something like a QuaternionCamera would
  have the identify Frame_to_sc quaternion.

  This class can be a bit on the slow side, you may want to consider
  wrapping it with OrbitListCache to cache the orbit data
  calculation.

  The Abcorr should be any of the strings spkezp
  (https://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/cspice/spkezp_c.html)
  accepts (e.g., "LT").

  Note that if you include the stellar correction (e.g., "LT+S") you
  should *not* also include the velocity aberration correction in
  e.g., QuaternionOrbitData. Stellar correction gives the "apparent"
  position, which already accounts for the aberration angle correction

  Also, it is important to note that the light time calculated is to
  the center of the Body, not the surface. If you are trying to find a
  intercept with something near the surface this can be considerably
  different (see sincpt_c vs spkezp_c in the SPICE documentation). So
  generally you don't want anything other than the default "NONE".
*******************************************************************/
class SpicePlanetOrbit : public Orbit {
public:
  SpicePlanetOrbit(const std::string& Target_name, 
		   const std::string& Spacecraft_reference_frame_name,
		   int Naif_id,
		   const std::string& Abcorr = "NONE");
  SpicePlanetOrbit(const std::string& Target_name, 
		   const std::string& Spacecraft_reference_frame_name,
		   const std::vector<std::string>& Kernel_list,
		   int Naif_id,
		   const std::string& Abcorr = "NONE");
  SpicePlanetOrbit(const std::string& Target_name, 
		   const std::string& Spacecraft_reference_frame_name,
		   const SpiceKernelList& Kernel_list,
		   int Naif_id,
		   const std::string& Abcorr = "NONE");
  virtual ~SpicePlanetOrbit() {}
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const
  { 
    // Right now don't handle derivatives. We can revisit this if
    // this becomes an issue.
    return orbit_data(T.value());
  }
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Set minimum time we want orbit for (important because kernels
/// only cover a particular time range).
//-----------------------------------------------------------------------
  
  void set_min_time(const Time& Tmin) {min_tm = Tmin;}

//-----------------------------------------------------------------------
/// Set maximum time we want orbit for (important because kernels
/// only cover a particular time range).
//-----------------------------------------------------------------------

  void set_max_time(const Time& Tmax) {max_tm = Tmax;}
  
//-----------------------------------------------------------------------
/// Body NAIF id.
//-----------------------------------------------------------------------

  int naif_id() const { return naif_id_; }

//-----------------------------------------------------------------------
/// Target name of spacecraft (e.g., "GLL", "MRO")
//-----------------------------------------------------------------------

  const std::string& target_name() const { return target_name_; }

//-----------------------------------------------------------------------
/// Name of spacecraft reference frame (e.g., "GLL_SCAN_PLANE", "MRO_CTX")
//-----------------------------------------------------------------------

  const std::string& spacecraft_reference_frame_name() const
  { return spacecraft_reference_frame_name_; }

//-----------------------------------------------------------------------
/// List of kernels that should be loaded with this class.
//-----------------------------------------------------------------------

  const SpiceKernelList& kernel_list() const { return kernel_list_;}

//-----------------------------------------------------------------------
/// The Aberration correction. Should be any of the strings spkezp
/// (https://naif.jpl.nasa.gov/pub/naif/toolkit_docs/C/cspice/spkezp_c.html)
/// accepts (e.g., "LT").
//-----------------------------------------------------------------------
  const std::string& aberration_correction() const {return abcorr_;}
  void aberration_correction(const std::string& Abcorr)
  {abcorr_ = Abcorr; }
protected:
  SpicePlanetOrbit() : abcorr_("NONE") {}
private:
  std::string target_name_;
  std::string spacecraft_reference_frame_name_;
  SpiceKernelList kernel_list_;
  int naif_id_;
  std::string abcorr_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(SpicePlanetOrbit);
GEOCAL_CLASS_VERSION(SpicePlanetOrbit, 1);
#endif
