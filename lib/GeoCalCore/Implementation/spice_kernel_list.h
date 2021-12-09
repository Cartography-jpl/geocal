#ifndef SPICE_KERNEL_LIST_H
#define SPICE_KERNEL_LIST_H
#include "printable.h"
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This maintains a list of spice kernels. This supports loading the
  kernels, and in particular it handles serializing this. This is
  useful in support of other Spice objects (e.g., SpicePlanetOrbit)
*******************************************************************/

class SpiceKernelList: public Printable<SpiceKernelList> {
public:
  SpiceKernelList(const std::vector<std::string>& Kernel_list)
    : kernel_list_(Kernel_list)
  { load_kernel(); }
  SpiceKernelList() {}
  virtual ~SpiceKernelList() {}
  const std::vector<std::string>& kernel_list() const { return kernel_list_;}
  void kernel_list(const std::vector<std::string>& Klist)
  { kernel_list_ = Klist; }
  void print(std::ostream& Os) const
  { Os << "SpiceKernelList\n"; }
private:
  std::vector<std::string> kernel_list_;
  void load_kernel();
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(SpiceKernelList);
#endif

