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
  SpiceDem(int Naif_id, const Time& Tm)
    : naif_id_(Naif_id),
      time_(Tm)
  { init(); }
  SpiceDem(int Naif_id, const Time& Tm,
	   const std::vector<std::string>& Kernel_list)
    : naif_id_(Naif_id),
      time_(Tm),
      kernel_list_(Kernel_list)
  {
    init();
  }
  virtual ~SpiceDem() {}
  virtual double distance_to_surface(const GroundCoordinate& Gp) const
  { return Gp.height_reference_surface() - height_reference_surface(Gp); }
  virtual double height_reference_surface(const GroundCoordinate& Gp) const
  { return Planetocentric(*surface_point(Gp)).height_reference_surface(); }
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const;
  virtual void print(std::ostream& Os) const
  {
    Os << PlanetConstant::name(naif_id_) << " SpiceDem:\n"
       << "  Time: " << time_ << "\n";
  }
//-----------------------------------------------------------------------
/// List of kernels that should be loaded with this class.
//-----------------------------------------------------------------------

  const SpiceKernelList& kernel_list() const { return kernel_list_;}
  int naif_id() const {return naif_id_;}
  const Time& time() const {return time_;}
private:
  void init();
  int naif_id_;
  Time time_;
  std::string body_name, fixed_frame_name;
  SpiceKernelList kernel_list_;
  SpiceDem() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}  
GEOCAL_EXPORT_KEY(SpiceDem);
#endif
