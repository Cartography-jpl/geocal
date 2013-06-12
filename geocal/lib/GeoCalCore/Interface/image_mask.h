#ifndef IMAGE_MASK_H
#define IMAGE_MASK_H
#include "printable.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This provides a ImageMask, which can be used to prevent doing some kind
  of processing. An example is a using a Cloud mask to restrict
  image matching to cloud free areas.

  The mask can look at specific ImageCoordinate points indicating if
  the that point is masked or not. You can also look at a Image area to
  see if any of the area is masked (e.g., avoid image matching on this
  area because it is masked). 

  This mask works in ImageCoordinates, cf. GroundMask.
*******************************************************************/

class ImageMask : public Printable<ImageMask> {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ImageMask() {}

//-----------------------------------------------------------------------
/// Indicate if a particular point is masked. If true, the point is
/// masked and should not be used in processing (e.g., don't do image
/// matching)
//-----------------------------------------------------------------------

  virtual bool mask(int Line, int Sample) const = 0;

  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
};

/****************************************************************//**
  This is combines a set of masks into a single one. The combination
  just check each mask in turn, and if any of them indicates a point
  or a area is masked, then we return true.

  Note that this class handles the degenerate case of no masked. This
  is particular useful in handling situations where you might or might
  not have mask, e.g. a general routine that can take a mask but where
  you don't happen to have any masks in a particular case.

  When there are no masks, we return true.
*******************************************************************/

class CombinedImageMask: public ImageMask {
public:
//-----------------------------------------------------------------------
/// Create a empty CombinedImageMask.
//-----------------------------------------------------------------------

  CombinedImageMask() {}

//-----------------------------------------------------------------------
/// Create a CombinedImageMask using the given set of masks.
//-----------------------------------------------------------------------

  CombinedImageMask(const std::vector<boost::shared_ptr<ImageMask> >& Ml)
    : mask_list(Ml) {}

  virtual ~CombinedImageMask() {}

//-----------------------------------------------------------------------
/// Create a CombinedImageMask using the given set of masks, where we get
/// this from a general InputIterator.
//-----------------------------------------------------------------------

  template<typename It> CombinedImageMask(It First, It Last)
    : mask_list(First, Last) {}

  virtual bool mask(int Line, int Sample) const;
  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Possibly empty set of masks to use.
//-----------------------------------------------------------------------

  std::vector<boost::shared_ptr<ImageMask> > mask_list;
};

}
#endif
