#ifndef IMAGE_MASK_H
#define IMAGE_MASK_H
#include "printable.h"
#include "image_coordinate.h"
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
  virtual bool mask_ic(const ImageCoordinate& Ic) const;
  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This gives an ImageMask that is an offset of a given one. This would
  be used for example when an image is subsetted.
*******************************************************************/

class OffsetImageMask: public ImageMask {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  OffsetImageMask(const boost::shared_ptr<ImageMask> Im_original,
		  double Line_offset, double Sample_offset)
    : im_(Im_original), line_offset_(Line_offset),
      sample_offset_(Sample_offset)
  {
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------
  virtual ~OffsetImageMask() {}

  virtual bool mask(int Line, int Sample) const
  { return im_->mask(Line - line_offset_, Sample = sample_offset_); }
  virtual bool mask_ic(const ImageCoordinate& Ic) const
  { return im_->mask(Ic.line - line_offset_, Ic.sample - sample_offset_); }
    virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const
  { return im_->area_any_masked(Line - line_offset_, Sample - sample_offset_,
				Number_line, Number_sample); }

  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Original ImageMask
//-----------------------------------------------------------------------
  
  const boost::shared_ptr<ImageMask>& original_image_mask()
    const { return im_;}

//-----------------------------------------------------------------------
/// Return line offset.
//-----------------------------------------------------------------------

  double line_offset() const { return line_offset_;}

//-----------------------------------------------------------------------
/// Return line offset.
//-----------------------------------------------------------------------

  double sample_offset() const { return sample_offset_;}
private:
  boost::shared_ptr<ImageMask> im_;
  double line_offset_, sample_offset_;
  OffsetImageMask() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
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
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}
GEOCAL_EXPORT_KEY(ImageMask);
GEOCAL_EXPORT_KEY(OffsetImageMask);
GEOCAL_EXPORT_KEY(CombinedImageMask);
#endif
