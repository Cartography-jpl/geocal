#include "image_ground_connection.h"
#include "simple_dem.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. As a convenience, if Img_mask or Ground_mask are null
/// pointer, we replace them with an empty CombinedImageMask or
/// CombinedGroundMask.
//-----------------------------------------------------------------------
ImageGroundConnection::ImageGroundConnection
(const boost::shared_ptr<Dem>& d, 
 const boost::shared_ptr<RasterImage>& Img, 
 const std::string& Title,
 const boost::shared_ptr<ImageMask>& Img_mask,
 const boost::shared_ptr<GroundMask>& Ground_mask)
: dem_(d), image_(Img), title_(Title), image_mask_(Img_mask),
  ground_mask_(Ground_mask)
{
  if(!image_mask_)
    image_mask_.reset(new CombinedImageMask);
  if(!ground_mask_)
    ground_mask_.reset(new CombinedGroundMask);
}

//-----------------------------------------------------------------------
/// Find a MapInfo that covers the ground coordinate of this 
/// ImageGroundConnection. We calculate the ground coordinate of the
/// four corners, then find the MapInfo that covers those corners,
/// optionally adding a boundary in map pixels.  Depending on the
/// actual warping involved, this may or may not fully cover the edges
/// of the image.
//-----------------------------------------------------------------------

MapInfo ImageGroundConnection::cover(const MapInfo& Mi,
				     int Boundary) const {
  std::vector<boost::shared_ptr<GroundCoordinate> > pt;
  pt.push_back(ground_coordinate(ImageCoordinate(0, 0)));
  pt.push_back(ground_coordinate(ImageCoordinate(0, number_sample())));
  pt.push_back(ground_coordinate(ImageCoordinate(number_line(), 
						 number_sample())));
  pt.push_back(ground_coordinate(ImageCoordinate(number_line(), 0)));
  return Mi.cover(pt, Boundary);
}

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// X, Y, and Z components of the Ecr ground location. 
//-----------------------------------------------------------------------

blitz::Array<double, 2> ImageGroundConnection::image_coordinate_jac_ecr
(const Ecr& Gc) const
{
  // Default is just to do a numerical derivative.
  double eps = 0.1;
  blitz::Array<double, 2> res(2, 3);
  ImageCoordinate ic0 = image_coordinate(Gc);
  Ecr gcx(Gc);
  for(int i = 0; i < 3; ++i) {
    gcx.position[i] += eps;
    ImageCoordinate ic = image_coordinate(gcx);
    res(0, i) = (ic.line - ic0.line) / eps;
    res(1, i) = (ic.sample - ic0.sample) / eps;
    gcx.position[i] = Gc.position[i];
  }
  return res;
}

//-----------------------------------------------------------------------
/// Calculate the approximate resolution on the ground of a given
/// ImageCoordinate. This finds the intersection with the
/// reference surface for the given pixel, + 1 in the
/// line and sample direction. We find the difference in meters
/// between these points, and select the maximum value.
//-----------------------------------------------------------------------

double ImageGroundConnection::resolution_meter
(const ImageCoordinate& Ic) const
{
  boost::shared_ptr<GroundCoordinate> gc1 = ground_coordinate(Ic);
  boost::shared_ptr<GroundCoordinate> gc2 = 
    ground_coordinate(ImageCoordinate(Ic.line + 1, Ic.sample));
  boost::shared_ptr<GroundCoordinate> gc3 = 
    ground_coordinate(ImageCoordinate(Ic.line, Ic.sample + 1));
  double d1 = distance(*gc1, *gc2);
  double d2 = distance(*gc1, *gc3);
  return std::max(d1, d2);
}

//-----------------------------------------------------------------------
/// Variation of resolution_meter that find the resolution of the
/// center pixel.
//-----------------------------------------------------------------------

double ImageGroundConnection::resolution_meter() const
{
  return resolution_meter(ImageCoordinate(number_line() / 2.0, 
					  number_sample() / 2.0));
}
