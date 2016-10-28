#include "jia_sample.h"
#include "geocal_serialize_support.h"
#include <iostream>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void JiaSample::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(JiaSample);
  ar & GEOCAL_NVP_(blah);
}

GEOCAL_IMPLEMENT(JiaSample);
#endif

//-----------------------------------------------------------------------
/// Sample jia function.
//-----------------------------------------------------------------------

void JiaSample::jia_func() const
{
  std::cout << "Jia dummy function called\n";
}
