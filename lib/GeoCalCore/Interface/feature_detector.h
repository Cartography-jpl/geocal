#ifndef FEATURE_DETECTOR_H
#define FEATURE_DETECTOR_H
#include "image_coordinate.h"
#include "printable.h"
#include "raster_image.h"
#include "ground_mask.h"
#include <boost/operators.hpp>
#include <vector>

namespace GeoCal {

/****************************************************************//**
  This is an interest point, determined by a FeatureDetector.
*******************************************************************/

class InterestPoint : private boost::less_than_comparable<InterestPoint>,
		      public Printable<InterestPoint> {
public:
//-----------------------------------------------------------------------
/// Default constructor
//-----------------------------------------------------------------------

  InterestPoint() : image_coordinate(0, 0), weight(0) {}

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  InterestPoint(const ImageCoordinate &Ic, double Weight)
  : image_coordinate(Ic), weight(Weight) {}

//-----------------------------------------------------------------------
/// Image coordinate of interest point.
//-----------------------------------------------------------------------

  ImageCoordinate image_coordinate;

//-----------------------------------------------------------------------
/// Weight of interest point. The higher the weight, the "better" the
/// point is.
//-----------------------------------------------------------------------

  double weight;

  void print(std::ostream& Os) const;
};
/** \defgroup Miscellaneous Miscellaneous routines. **/

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Interest points have a natural order by ordering by weight, so
/// points with greater weight are "greater".
///
/// We define <=, >= and > in terms of this operator.
//-----------------------------------------------------------------------

inline bool operator<(const InterestPoint& I1, const InterestPoint& I2)
{ return I1.weight < I2.weight; }

/****************************************************************//**
  This class detects interest points in an image. An interest point 
  is one that is good for image matching by some sort of criteria.
  For example, a common feature detector is one that tries to detect
  corners, with the idea that these are good choices for doing image
  matching on.
*******************************************************************/

class FeatureDetector : public Printable<FeatureDetector> {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  FeatureDetector() { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~FeatureDetector() { }

//-----------------------------------------------------------------------
/// Go through a RasterImage, and return a list of InterstPoints for
/// it. The list isn't in any particular order, but note that
/// InterstPoints have a natural order by the weight, and you can sort
/// the points if desired using std::sort, or just pick at the "best"
/// point by std::max_element.
//-----------------------------------------------------------------------

  virtual std::vector<InterestPoint>
  interest_point_detect(const RasterImage& Img) const = 0;
  std::vector<ImageCoordinate> interest_point_grid(const RasterImage& Img,
     const GroundMask& M,					   
     int Number_grid_line, int Number_grid_sample, int Border = 0) const;
  std::vector<ImageCoordinate> interest_point_grid(const RasterImage& Img,
     int Number_grid_line, int Number_grid_sample, int Border = 0) const;

  boost::shared_ptr<ImageCoordinate>
  interest_point_grid_ptr(const RasterImage& Img,
			  const GroundMask& M, int i, int j,
			  int Number_grid_line, int Number_grid_sample, 
			  int Border = 0) const;
  boost::shared_ptr<ImageCoordinate>
  interest_point_grid_ptr(const RasterImage& Img,int i, int j, 
	  int Number_grid_line, int Number_grid_sample, 
	  int Border = 0) const;

  
//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
protected:
private:
  boost::shared_ptr<ImageCoordinate> 
  ip_grid(const RasterImage& Img, const GroundMask* M,
	  int i, int j, int Number_grid_line, int Number_grid_sample,
	  int Border) const;
};

}
#endif

