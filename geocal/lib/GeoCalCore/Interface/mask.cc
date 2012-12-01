#include "mask.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Indicate if a particular point is masked. If true, the point is
/// masked and should not be used in processing (e.g., don't do image
/// matching)
//-----------------------------------------------------------------------

bool CombinedMask::mask(const GroundCoordinate& Gc) const
{
  for(std::vector<boost::shared_ptr<Mask> >::size_type i = 0;
      i < mask_list.size(); ++i)
    if(mask_list[i]->mask(Gc))
      return true;
  return false;
}

//-----------------------------------------------------------------------
/// Indicated if a region is all masked or not. See the discussion in
/// the comments of Mask for detailed discussion of the check.
//-----------------------------------------------------------------------

bool CombinedMask::region_masked(const GroundCoordinate& Ulc, 
				const GroundCoordinate& Lrc) const
{
  for(std::vector<boost::shared_ptr<Mask> >::size_type i = 0;
      i < mask_list.size(); ++i)
    if(mask_list[i]->region_masked(Ulc, Lrc))
      return true;
  return false;
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void CombinedMask::print(std::ostream& Os) const
{
  if(mask_list.size() ==0)
    Os << "Empty CombinedMask\n";
  else {
    Os << "CombinedMask: \n";
    for(std::vector<boost::shared_ptr<Mask> >::size_type i = 0;
	i < mask_list.size(); ++i)
      Os << "  Mask " << i + 1 << ":\n" << *(mask_list[i]);
  }
}
