#ifndef SPICE_KERNEL_LIST_H
#define SPICE_KERNEL_LIST_H
#include "printable.h"
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This maintains a list of spice kernels. This supports loading the
  kernels, and in particular it handles serializing this. This is
  useful in support of other Spice objects (e.g., SpicePlanetOrbit).

  Note that this maintains the list of kernels, to actually load them
  call load_kernel.

  This handles spiceinit style kernel names from ISIS, so things
  like $mro/kernels/fk/mro_v16.tf. We look for these first in
  the directory pointed to by environment ISISDATA, then in
  SPICECACHE (if defined) or ~/.spice_cache if not. If the kernel
  isn't found on the system, we attempt to download from the the
  ISIS data server (see
  https://github.com/USGS-Astrogeology/ISIS3#isis-spice-web-service).
*******************************************************************/

class SpiceKernelList: public Printable<SpiceKernelList> {
public:
  SpiceKernelList(const std::vector<std::string>& Kernel_list,
		  bool Skip_load=false)
    : kernel_list_(Kernel_list),
      loaded_(false)
  { if(!Skip_load)
      load_kernel();
  }
  SpiceKernelList() : loaded_(false) {}
  virtual ~SpiceKernelList() {}
  const std::vector<std::string>& kernel_list() const { return kernel_list_;}
  void load_kernel(bool Download_from_isis_web_if_needed=true) const;
  void kernel_list(const std::vector<std::string>& Klist)
  { kernel_list_ = Klist; }
  void print(std::ostream& Os) const;
private:
  std::vector<std::string> kernel_list_;
  mutable bool loaded_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(SpiceKernelList);
#endif

