#ifndef SPICE_DEM_H
#define SPICE_DEM_H
#include "dem.h"
#include "planet_coordinate.h"
#include "geocal_time.h"
#include "spice_kernel_list.h"

namespace GeoCal {
/****************************************************************//**
  This class uses the SPICE function latsrf to use DSK files to 
  create a DEM.

  Note that this isn't particularly efficient. If you are doing to use
  this for a large number of points you may want to use DemToRaster or
  something similar to create a stand alone DEM file
*******************************************************************/
class SpiceDem: public Dem {
public:
  SpiceDem(int Naif_code, const Time& Tm)
    : naif_code_(Naif_code),
      time_(Tm)
  { }
  SpiceDem(int Naif_code, const Time& Tm,
	   const std::vector<std::string>& Kernel_list)
    : naif_code_(Naif_code),
      time_(Tm),
      kernel_list_(Kernel_list)
  { }
  virtual ~SpiceDem() {}
  virtual double distance_to_surface(const GroundCoordinate& Gp) const
  { return Gp.height_reference_surface() - height_reference_surface(Gp); }
  virtual double height_reference_surface(const GroundCoordinate& Gp) const
  { return Planetocentric(*surface_point(Gp)).height_reference_surface(); }
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const;
  virtual void height_range(const GroundCoordinate& Ulc,
			      const GroundCoordinate& Lrc,
			      double& Min_h, double& Max_h,
			      double H_pad = 10.0) const
  {
    // Not real easy for us to implement. We could probably come up
    // with something if this becomes an issue, but we don't use this
    // DEM a lot. For now, just treat this as an error. We can revisit
    // this if this becomes an issue
    throw Exception("Not implemented");
  }
  virtual void print(std::ostream& Os) const
  {
    Os << PlanetConstant::name(naif_code_) << " SpiceDem:\n"
       << "  Time: " << time_ << "\n";
  }
//-----------------------------------------------------------------------
/// List of kernels that should be loaded with this class.
//-----------------------------------------------------------------------

  const SpiceKernelList& kernel_list() const { return kernel_list_;}
  int naif_code() const {return naif_code_;}
  const Time& time() const {return time_;}
protected:
  SpiceDem() {}
private:
  int naif_code_;
  Time time_;
  SpiceKernelList kernel_list_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}  
GEOCAL_EXPORT_KEY(SpiceDem);
#endif
