#include "ground_mask.h"
#include <boost/foreach.hpp>
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Indicate if a particular point is masked. If true, the point is
/// masked and should not be used in processing (e.g., don't do image
/// matching)
//-----------------------------------------------------------------------

bool CombinedGroundMask::mask(const GroundCoordinate& Gc) const
{
  BOOST_FOREACH(boost::shared_ptr<GroundMask> m, mask_list) {
    if(m->mask(Gc))
      return true;
  }
  return false;
}

//-----------------------------------------------------------------------
/// Indicated if a region is all masked or not. See the discussion in
/// the comments of GroundMask for detailed discussion of the check.
//-----------------------------------------------------------------------

bool CombinedGroundMask::region_masked(const GroundCoordinate& Ulc, 
				const GroundCoordinate& Lrc) const
{
  BOOST_FOREACH(boost::shared_ptr<GroundMask> m, mask_list) {
    if(m->region_masked(Ulc, Lrc))
      return true;
  }
  return false;
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void CombinedGroundMask::print(std::ostream& Os) const
{
  if(mask_list.size() ==0)
    Os << "Empty CombinedGroundMask\n";
  else {
    Os << "CombinedGroundMask: \n";
    int i = 0;
    BOOST_FOREACH(boost::shared_ptr<GroundMask> m, mask_list) {
      Os << "  Mask " << ++i << ":\n" << *m;
    }
  }
}