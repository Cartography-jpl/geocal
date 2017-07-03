#include "spice_kernel_list.h"
#include "geocal_serialize_support.h"
#include "spice_helper.h"
#include <boost/foreach.hpp>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class Archive>
void SpiceKernelList::save(Archive & ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void SpiceKernelList::load(Archive & ar, const unsigned int version)
{
  load_kernel();
}

template<class Archive>
void SpiceKernelList::serialize(Archive & ar, const unsigned int version) 
{
  GEOCAL_GENERIC_BASE(SpiceKernelList);
  ar & GEOCAL_NVP_(kernel_list);
  boost::serialization::split_member(ar, *this, version);
}

GEOCAL_IMPLEMENT(SpiceKernelList);
#endif

void SpiceKernelList::load_kernel()
{
  BOOST_FOREACH(const std::string& f, kernel_list_) {
    boost::filesystem::path p(f);
    if(!SpiceHelper::kernel_loaded(p.filename().string()))
      SpiceHelper::add_kernel(f);
  }
}
