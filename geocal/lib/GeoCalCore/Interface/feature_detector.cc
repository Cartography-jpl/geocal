#include "feature_detector.h"
#include "sub_raster_image.h"
#include "null_deleter.h"
#include <cmath>
#include <boost/foreach.hpp>
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Print InterestPoint to a stream.
//-----------------------------------------------------------------------

void InterestPoint::print(std::ostream& Os) const
{
  Os << "Interest Point: " << image_coordinate 
     << " Weight: " << weight;
}


//-----------------------------------------------------------------------
/// Determine the a Image Coordinate for a single point. We pull this
/// function out because we have several similar interest_point_grid
/// functions that make use of this common functionality.
///
/// The Mask M may be present, or may be null indicating we have no
/// mask. We might or might not find a point (e.g., the entire area is
/// masked). If we don't then a null pointer is returned.
//-----------------------------------------------------------------------

boost::shared_ptr<ImageCoordinate> FeatureDetector::ip_grid
(const RasterImage& Img, const Mask* M,
 int i, int j, int Number_grid_line, int Number_grid_sample,
 int Border) const
{
  int lspace = (int) round((Img.number_line() - 2.0 * Border) / 
			   Number_grid_line);
  int sspace = (int) round((Img.number_sample() - 2.0 * Border) / 
			   Number_grid_sample);
  int sline = Border + lspace * i;
  int ssamp = Border + sspace * j;
  int eline = sline + lspace;
  int esamp = ssamp + sspace;
  if(eline > Img.number_line() - Border)
    eline = Img.number_line() - Border;
  if(esamp > Img.number_sample() - Border)
    esamp = Img.number_sample() - Border;
  if(M &&
     M->region_masked(*Img.ground_coordinate(ImageCoordinate(sline, ssamp)),
      *Img.ground_coordinate(ImageCoordinate(eline - 1, esamp - 1))))
    return boost::shared_ptr<ImageCoordinate>();
  SubRasterImage sub_img(const_pointer(Img),
			 sline, ssamp, eline - sline, esamp - ssamp);
  std::vector<InterestPoint> ip_list = interest_point_detect(sub_img);
  std::sort(ip_list.begin(), ip_list.end());
  for(int i = ((int) ip_list.size()) - 1; i >= 0; --i) {
    ImageCoordinate best_point = ip_list[i].image_coordinate;
    best_point.line += sline;
    best_point.sample += ssamp;
    if(!M ||
       !M->mask(*Img.ground_coordinate(best_point)))
      return 
	boost::shared_ptr<ImageCoordinate>(new ImageCoordinate(best_point));
  }
  return boost::shared_ptr<ImageCoordinate>();
}

//-----------------------------------------------------------------------
/// This is used to generate a set of interest points that can then be
/// used to do image matching over a whole raster image. We divide up
/// the raster image into Number_grid_line x Number_grid_sample boxes.
/// In each box, we select one point that has the largest weight as an
/// interest point. Since these points are typically used for image
/// matching, which can't be done too close to the edge of the image,
/// an optional border to exclude can be supplied. None of the points
/// returned will be within the border of the edge of the image. 
///
/// We exclude points that are marked by the Mask.
//-----------------------------------------------------------------------

std::vector<ImageCoordinate> 
FeatureDetector::interest_point_grid(const RasterImage& Img,
     const Mask& M,
     int Number_grid_line, int Number_grid_sample, int Border) const
{
  std::vector<ImageCoordinate> res;
  for(int i = 0; i < Number_grid_line; ++i)
    for(int j = 0; j < Number_grid_sample; ++j) {
      boost::shared_ptr<ImageCoordinate> r = 
	ip_grid(Img, &M, i, j, Number_grid_line, Number_grid_sample, Border);
      if(r)
	res.push_back(*r);
    }
  return res;
}

//-----------------------------------------------------------------------
/// Variation of interest_point_grid that does not use as Mask.
//-----------------------------------------------------------------------

std::vector<ImageCoordinate> 
FeatureDetector::interest_point_grid(const RasterImage& Img,
     int Number_grid_line, int Number_grid_sample, int Border) const
{
  std::vector<ImageCoordinate> res;
  for(int i = 0; i < Number_grid_line; ++i)
    for(int j = 0; j < Number_grid_sample; ++j) {
      boost::shared_ptr<ImageCoordinate> r = 
	ip_grid(Img, 0, i, j, Number_grid_line, 
		Number_grid_sample, Border);
      if(r)
	res.push_back(*r);
    }
  return res;
}

//-----------------------------------------------------------------------
/// Python much prefers getting shared_ptr back, so we have this
/// variation that wraps that. We also return only a single
/// point. This is a somewhat awkward interface, but it gets wrapped
/// into a interest_point_grid function in python, possibly doing
/// processing in parallel.
//-----------------------------------------------------------------------

boost::shared_ptr<ImageCoordinate>
FeatureDetector::interest_point_grid_ptr
(const RasterImage& Img,
 const Mask& M, int i, int j,
 int Number_grid_line, int Number_grid_sample, 
 int Border) const
{
  range_check(i, 0, Number_grid_line);
  range_check(j, 0, Number_grid_sample);
  return ip_grid(Img, &M, i, j, Number_grid_line, Number_grid_sample, Border);
}

//-----------------------------------------------------------------------
/// Python much prefers getting shared_ptr back, so we have this
/// variation that wraps that. We also return only a single
/// point. This is a somewhat awkward interface, but it gets wrapped
/// into a interest_point_grid function in python, possibly doing
/// processing in parallel.
//-----------------------------------------------------------------------

boost::shared_ptr<ImageCoordinate>
FeatureDetector::interest_point_grid_ptr
(const RasterImage& Img, int i, int j,
 int Number_grid_line, int Number_grid_sample, int Border) const
{
  range_check(i, 0, Number_grid_line);
  range_check(j, 0, Number_grid_sample);
  return ip_grid(Img, 0, i, j, Number_grid_line, Number_grid_sample, Border);
}
