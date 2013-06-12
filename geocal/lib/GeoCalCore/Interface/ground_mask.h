#ifndef GROUND_MASK_H
#define GROUND_MASK_H
#include "printable.h"
#include "ground_coordinate.h"
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This provides a GroundMask, which can be used to prevent doing some kind
  of processing. An example is a using a Land/Water mask to restrict
  image matching to land, or a cloud mask to restrict image matching
  to cloud free areas.

  The mask can look at specific GroundCoordinate points indicating if
  the that point is masked or not. You can also look at a region to
  see if the entire area is masked (e.g., avoid processing on this
  area because it is masked). 

  The region search is a bit loose, you give the corners of the area
  to search. The region selected depends on the underlying map
  projection used by the mask data - it might be a region in
  latitude/longitude, a region in UTM, or something else. The region
  search is useful only for small areas where these difference aren't
  very important. If you need detailed, accurate information about a
  region in a specific map projection then you should simply look at
  each individual point. The region is useful for the "don't bother
  looking here for tie points, look somewhere else" sorts of uses.

  If the region search returns true, then all of the region is masked.
  If this is false, then some of the region might be unmasked. Note
  that this is conservative in the true direction - a value of false
  doesn't promise that there is an unmasked pixel, just that we are
  pretty sure that there is one. Edge cases where a false value clips
  the corner of a region might have region search return false, while
  when you look at it pixel by pixel every point ends up being masked.

  Typically a Mask has an underlying RasterImage or something similar.
  A particular GroundCoordinate probably doesn't fall exactly on a
  pixel of the Mask. How we interpolate is defined by the particular
  derived class uses - a common approach is to mask if any of the 4
  neighboring pixels are masked (but check the derived class for
  details).

  This mask works in GroundCoordinates, cf. ImageMask.
*******************************************************************/

class GroundMask : public Printable<GroundMask> {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GroundMask() {}

//-----------------------------------------------------------------------
/// Indicate if a particular point is masked. If true, the point is
/// masked and should not be used in processing (e.g., don't do image
/// matching)
//-----------------------------------------------------------------------

  virtual bool mask(const GroundCoordinate& Gc) const = 0;

//-----------------------------------------------------------------------
/// Indicated if a region is all masked or not. See the discussion in
/// the comments of Mask for detailed discussion of the check.
//-----------------------------------------------------------------------

  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const = 0;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
};

/****************************************************************//**
  This is combines a set of masks into a single one. The combination
  just check each mask in turn, and if any of them indicates a point
  or a region is masked, then we return true.

  Note that this class handles the degenerate case of no masked. This
  is particular useful in handling situations where you might or might
  not have mask, e.g. a general routine that can take a mask but where
  you don't happen to have any masks in a particular case.

  When there are no masks, we return true.
*******************************************************************/

class CombinedGroundMask: public GroundMask {
public:
//-----------------------------------------------------------------------
/// Create a empty CombinedMask.
//-----------------------------------------------------------------------

  CombinedGroundMask() {}

//-----------------------------------------------------------------------
/// Create a CombinedMask using the given set of masks.
//-----------------------------------------------------------------------

  CombinedGroundMask(const std::vector<boost::shared_ptr<GroundMask> >& Ml)
    : mask_list(Ml) {}

  virtual ~CombinedGroundMask() {}

//-----------------------------------------------------------------------
/// Create a CombinedMask using the given set of masks, where we get
/// this from a general InputIterator.
//-----------------------------------------------------------------------

  template<typename It> CombinedGroundMask(It First, It Last)
    : mask_list(First, Last) {}

  virtual bool mask(const GroundCoordinate& Gc) const;
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Possibly empty set of masks to use.
//-----------------------------------------------------------------------

  std::vector<boost::shared_ptr<GroundMask> > mask_list;
};

}
#endif
