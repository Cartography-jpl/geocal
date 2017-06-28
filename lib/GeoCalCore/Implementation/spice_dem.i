// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "spice_dem.h"
%}
%base_import(dem)
%import "spice_kernel_list.i"
%import "geocal_time.i"
%geocal_shared_ptr(GeoCal::SpiceDem);

namespace GeoCal {
class SpiceDem : public Dem {
public:
  SpiceDem(int Naif_id, const Time& Tm);
  SpiceDem(int Naif_id, const Time& Tm,
	   const std::vector<std::string>& Kernel_list);
  virtual double distance_to_surface(const GroundCoordinate& Gp) const;
  virtual double height_reference_surface(const GroundCoordinate& Gp) const;
  virtual boost::shared_ptr<GroundCoordinate> 
    surface_point(const GroundCoordinate& Gp) const;
  %python_attribute(kernel_list, SpiceKernelList);
  %python_attribute(naif_id, int);
  %python_attribute(time, Time);
  %python_attribute(spacecraft_reference_frame_name, std::string);
  %pickle_serialization();
};
}


