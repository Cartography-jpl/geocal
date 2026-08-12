#include "spice_kernel_list.h"
#include "geocal_serialize_support.h"
#include "spice_helper.h"
#include "boost_pcall.h"
#include <boost/filesystem.hpp>
#include <boost/make_shared.hpp>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class Archive>
void SpiceKernelList::serialize(Archive & ar, const unsigned int version) 
{
  GEOCAL_GENERIC_BASE(SpiceKernelList);
  ar & GEOCAL_NVP_(kernel_list);
}

GEOCAL_IMPLEMENT(SpiceKernelList);
#endif

void SpiceKernelList::load_kernel(bool Download_from_isis_web_if_needed) const
{
  if(loaded_)
    // Skip if have already been loaded
    return;
  // Check to see if all the kernels are already on the system
  bool need_kernel = false;
  std::vector<std::string> missing_kernel;
  for(auto f : kernel_list_)
    if(!boost::filesystem::exists(f)) {
      need_kernel = true;
      missing_kernel.push_back(f);
    }
  if(need_kernel) {
    if(!Download_from_isis_web_if_needed) {
      Exception e("We have kernels not found on the system, but aren't downloading from isis web.");
      e << "Need the kernel(s):\n";
      for(auto f : missing_kernel)
	e << "   " << f << "\n";
      throw e;
    }
    boost::shared_ptr<SpiceKernelList> klist2 =
      boost_pcall<SpiceKernelList, SpiceKernelList>("boost_spice_kernel_list",
			    boost::make_shared<SpiceKernelList>(*this));
    klist2->load_kernel(false);
    loaded_ = true;
    return;
  }
  for(auto f : kernel_list_)
    if(!SpiceHelper::kernel_loaded(f))
      SpiceHelper::add_kernel(f);
  loaded_ = true;
}

void SpiceKernelList::print(std::ostream& Os) const
{
  Os << "SpiceKernelList:\n";
  for(auto f : kernel_list_)
    Os << "   " << f << "\n";
}
  
