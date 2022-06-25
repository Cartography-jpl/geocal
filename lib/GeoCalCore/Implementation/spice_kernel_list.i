// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "spice_kernel_list.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::SpiceKernelList);

namespace GeoCal {
class SpiceKernelList : public GenericObject {
public:
  SpiceKernelList(const std::vector<std::string>& Kernel_list,
		  bool Skip_load=False);
  SpiceKernelList();
  void load_kernel(bool Download_from_isis_web_if_needed=true) const;
  %python_attribute_with_set(kernel_list, std::vector<std::string>);
  std::string print_to_string() const;
  %pickle_serialization();
};
}


// List of things "import *" will include
%python_export("SpiceKernelList")
