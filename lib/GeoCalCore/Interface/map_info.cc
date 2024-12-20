#include "map_info.h"
#include "geocal_exception.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"
#include <boost/foreach.hpp>
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void MapInfo::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(MapInfo);
  ar & GEOCAL_NVP_(conv) & GEOCAL_NVP_(number_x_pixel) 
    & GEOCAL_NVP_(number_y_pixel) & GEOCAL_NVP(param);
  // Older version didn't have is_point_. The default was to always
  // assume pixel is area.
  if(version > 0)
    ar & GEOCAL_NVP_(is_point);
}

GEOCAL_IMPLEMENT(MapInfo);
#endif

//-----------------------------------------------------------------------
/// Constructor. Note that ulc and lrc are for the bounding box of the
/// map, *not* for the center of the upper left hand or lower right hand
/// pixel.
///
/// Note that it is allowed for Number_x_pixel or Number_y_pixel to be
/// 0. This obviously isn't a very useful map, but it is nice for edge
/// cases for us to allow an empty map (e.g., intersection of two MapInfo
/// that don't actually intersect).
//-----------------------------------------------------------------------

MapInfo::MapInfo(const boost::shared_ptr<CoordinateConverter>& Conv, 
		 double Ulc_x, double Ulc_y, double Lrc_x, double Lrc_y, 
		 int Number_x_pixel, int Number_y_pixel, bool Is_point)
  : conv_(Conv),
    is_point_(Is_point),
    number_x_pixel_(Number_x_pixel),
    number_y_pixel_(Number_y_pixel),
    resolution_x_(-1),
    resolution_y_(-1),
    param(6)
{
  range_min_check(number_x_pixel_, 0);
  range_min_check(number_y_pixel_, 0);
  if(number_x_pixel_ > 0 &&
     number_y_pixel_ > 0) {
    double xspace = (Lrc_x - Ulc_x) / number_x_pixel_;
    double yspace = (Lrc_y - Ulc_y) / number_y_pixel_;
    param = Ulc_x, xspace, 0,
      Ulc_y, 0, yspace;
  } else {
    param = Ulc_x, 0, 0,
      Ulc_y, 0, 0;
    number_x_pixel_ = 0;
    number_y_pixel_ = 0;
  }
  latitude_range_correct();
}

//-----------------------------------------------------------------------
/// Constructor that takes the affine parameters. Note that the
/// parameters should be such that the ulc is at coordinates -0.5, 0.5.
/// This is the same as "area based pixels", if you are using Geotiff.
//-----------------------------------------------------------------------

MapInfo::MapInfo(const boost::shared_ptr<CoordinateConverter>& Conv, 
	  const blitz::Array<double, 1>& Param,
	  int Number_x_pixel, 
		 int Number_y_pixel, bool Is_point)
  : conv_(Conv),
    is_point_(Is_point),
    number_x_pixel_(Number_x_pixel),
    number_y_pixel_(Number_y_pixel),
    resolution_x_(-1),
    resolution_y_(-1),
    param(Param.copy())
{
  range_min_check(number_x_pixel_, 0);
  range_min_check(number_y_pixel_, 0);
  if(Param.rows() != 6)
    throw Exception("Param needs to have exactly 6 elements");
  if(number_x_pixel_ == 0 ||
     number_y_pixel_ == 0) {
    number_x_pixel_ = 0;
    number_y_pixel_ = 0;
  }
  latitude_range_correct();
}

//-----------------------------------------------------------------------
/// Determine pixel coordinates for the given ground coordinates. Note
/// that this routine can be called with ground coordiantes outside of
/// the bounding box of the map, it just returns pixel coordinates
/// outside of the map in that case.
//-----------------------------------------------------------------------

void MapInfo::coordinate(const GroundCoordinate& Gc, double& Pixel_x_index, 
		double& Pixel_y_index) const
{
  double x, y, height;
  conv_->convert_to_coordinate(Gc, x, y, height);
  coordinate_to_index(x, y, Pixel_x_index, Pixel_y_index);
}

void MapInfo::coordinate(const Geodetic& Gc, double& Pixel_x_index, 
		double& Pixel_y_index) const
{
  double x, y, height;
  conv_->convert_to_coordinate(Gc, x, y, height);
  coordinate_to_index(x, y, Pixel_x_index, Pixel_y_index);
}

//-----------------------------------------------------------------------
/// Create a MapInfo that has the same map projection and pixel
/// resolution as this one, but covers the given set of points. An
/// optional boundary can be given, this adds a boundary of the given
/// number of pixels on each edge of the resulting MapInfo.
//-----------------------------------------------------------------------

MapInfo MapInfo::cover(const 
  std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
  int boundary) const
{
  if(Pt.size() < 2)
    throw Exception("Need at least 2 points in MapInfo::cover");
  double min_x, max_x, min_y, max_y;
  double x,y;
  coordinate(*Pt[0], x, y);
  min_x = max_x = x;
  min_y = max_y = y;
  BOOST_FOREACH(const boost::shared_ptr<GroundCoordinate>& gp, Pt) {
    coordinate(*gp, x, y);
    min_x = std::min(min_x, x);
    max_x = std::max(max_x, x);
    min_y = std::min(min_y, y);
    max_y = std::max(max_y, y);
  }
  min_x = floor(min_x) - boundary;
  max_x = ceil(max_x) + boundary;
  min_y = floor(min_y) - boundary;
  max_y = ceil(max_y) + boundary;
  int nx = (int) (max_x - min_x + 1);
  int ny = (int) (max_y - min_y + 1);
  int xstart = (int) min_x;
  int ystart = (int) min_y;
  return subset(xstart, ystart, nx, ny);
}

//-----------------------------------------------------------------------
/// Convert pixel coordinates to ground coordinates, and place on
/// surface using DEM.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate> 
MapInfo::ground_coordinate(double Pixel_x_index, 
 	           double Pixel_y_index, const Dem& D) const
{
  return D.surface_point(*ground_coordinate(Pixel_x_index, Pixel_y_index));
}

//-----------------------------------------------------------------------
/// Convert pixel coordinates to ground coordinates.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate> 
MapInfo::ground_coordinate(double Pixel_x_index, double Pixel_y_index) const
{
  double x, y;
  index_to_coordinate(Pixel_x_index, Pixel_y_index, x, y);
  return conv_->convert_from_coordinate(x, y);
}

//-----------------------------------------------------------------------
/// This finds a rectangle box in the map projection and pixel size of
/// this class that contains the intersection with the second map info
/// Mi.
//-----------------------------------------------------------------------

MapInfo MapInfo::intersection(const MapInfo& Mi) const
{
  double x1, y1, x2, y2, x3, y3, x4, y4;
  coordinate(*Mi.ground_coordinate(0, 0), x1, y1);
  coordinate(*Mi.ground_coordinate(Mi.number_x_pixel(), 0),
	     x2, y2);
  coordinate(*Mi.ground_coordinate(Mi.number_x_pixel(), 
				   Mi.number_y_pixel()), x3, y3);
  coordinate(*Mi.ground_coordinate(0, Mi.number_y_pixel()), x4, y4);
  double xmin = std::max(std::min(std::min(std::min(x1, x2), x3), x4), 0.0);
  double xmax = std::min(std::max(std::max(std::max(x1, x2), x3), x4),
			 (double) number_x_pixel());
  double ymin = std::max(std::min(std::min(std::min(y1, y2), y3), y4), 0.0);
  double ymax = std::min(std::max(std::max(std::max(y1, y2), y3), y4),
			 (double) number_y_pixel());
  xmin = std::min(xmin, xmax);
  ymin = std::min(ymin, ymax);
  int nx = (int) round(xmax - xmin);
  int ny = (int) round(ymax - ymin);
  return subset((int) round(xmin), (int) round(ymin), nx, ny);
}

//-----------------------------------------------------------------------
/// This finds a rectangle box in the map projection and pixel size of
/// this class that contains the union with the second map info
/// Mi. Note we call this "map_union" because "union" is a reserved
/// word in C++.
//-----------------------------------------------------------------------

MapInfo MapInfo::map_union(const MapInfo& Mi) const
{
  double x1, y1, x2, y2, x3, y3, x4, y4;
  coordinate(*Mi.ground_coordinate(0, 0), x1, y1);
  coordinate(*Mi.ground_coordinate(Mi.number_x_pixel(), 0),
	     x2, y2);
  coordinate(*Mi.ground_coordinate(Mi.number_x_pixel(), 
				   Mi.number_y_pixel()), x3, y3);
  coordinate(*Mi.ground_coordinate(0, Mi.number_y_pixel()), x4, y4);
  double xmin = std::min(std::min(std::min(std::min(x1, x2), x3), x4), 0.0);
  double xmax = std::max(std::max(std::max(std::max(x1, x2), x3), x4),
			 (double) number_x_pixel());
  double ymin = std::min(std::min(std::min(std::min(y1, y2), y3), y4), 0.0);
  double ymax = std::max(std::max(std::max(std::max(y1, y2), y3), y4),
			 (double) number_y_pixel());
  xmin = std::min(xmin, xmax);
  ymin = std::min(ymin, ymax);
  int nx = (int) round(xmax - xmin);
  int ny = (int) round(ymax - ymin);
  return subset((int) round(xmin), (int) round(ymin), nx, ny);
}

//-----------------------------------------------------------------------
/// Return the approximate resolution of the MapInfo in meters. This looks
/// at the center of the map, + 1 in the X and Y direction. We
/// determine the distance between these points, and select the
/// maximum of them.
//-----------------------------------------------------------------------

double MapInfo::resolution_meter() const
{
  return std::max(resolution_x(), resolution_y());
}

//-----------------------------------------------------------------------
/// Return the approximate resolution of the MapInfo the X direction
/// in meters. This looks at the center of the map, + 1 in the X
/// direction. We determine the distance between these points.
//-----------------------------------------------------------------------

double MapInfo::resolution_x() const
{
  if(resolution_x_ < 0) {
    boost::shared_ptr<GroundCoordinate> gc1 =
      ground_coordinate(number_x_pixel() / 2, number_y_pixel() / 2);
    boost::shared_ptr<GroundCoordinate> gc2 =
      ground_coordinate(number_x_pixel() / 2 + 1, number_y_pixel() / 2);
    resolution_x_ = distance(*gc1, *gc2);
  }
  return resolution_x_;
}

//-----------------------------------------------------------------------
/// Return the approximate resolution of the MapInfo the Y direction
/// in meters. This looks at the center of the map, + 1 in the Y
/// direction. We determine the distance between these points.
//-----------------------------------------------------------------------

double MapInfo::resolution_y() const
{
  if(resolution_y_ < 0) {
    boost::shared_ptr<GroundCoordinate> gc1 =
      ground_coordinate(number_x_pixel() / 2, number_y_pixel() / 2);
    boost::shared_ptr<GroundCoordinate> gc2 =
      ground_coordinate(number_x_pixel() / 2, number_y_pixel() / 2 + 1);
    resolution_y_ = distance(*gc1, *gc2);
  }
  return resolution_y_;
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void MapInfo::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "Coordinate: \n";
  opad << coordinate_converter() << "\n";
  opad.strict_sync();
  Os << "ULC:       (" << ulc_x() << ", " << ulc_y() << ")\n"
     << "LRC:       (" << lrc_x() << ", " << lrc_y() << ")\n"
     << "Number:    (" << number_x_pixel() << ", "
     << number_y_pixel() << ")\n"
     << "Pixel:     " << (is_point() ? "Is point" : "Is area") << "\n";
}

//-----------------------------------------------------------------------
/// Return a MapInfo for a subset of this map info. Note that it is 
/// ok for x_index and y_index to be outside the range of the MapInfo,
/// so you could use this function to get a MapInfo for a larger area
/// or an area next to this one.
//-----------------------------------------------------------------------

MapInfo MapInfo::subset(int x_index, int y_index, int nx_pixel,
			int ny_pixel) const 
{
  MapInfo res(*this);
  index_to_coordinate(x_index - 0.5, y_index - 0.5, res.param(0), 
		      res.param(3));
  res.number_x_pixel_ = nx_pixel;
  res.number_y_pixel_ = ny_pixel;
  res.resolution_x_ = -1;
  res.resolution_y_ = -1;
  return res;
}

//-----------------------------------------------------------------------
/// Return a MapInfo for a subset of this map info. Note that it is 
/// ok for x_index and y_index to be outside the range of the MapInfo,
/// so you could use this function to get a MapInfo for a larger area
/// or an area next to this one.
//-----------------------------------------------------------------------

MapInfo MapInfo::subset(double x_index, double y_index, int nx_pixel,
			int ny_pixel) const 
{
  MapInfo res(*this);
  index_to_coordinate(x_index - 0.5, y_index - 0.5, res.param(0), 
		      res.param(3));
  res.number_x_pixel_ = nx_pixel;
  res.number_y_pixel_ = ny_pixel;
  res.resolution_x_ = -1;
  res.resolution_y_ = -1;
  return res;
}

//-----------------------------------------------------------------------
/// Create a MapInfo that has the same upper left corner, and a pixel
/// size scaled by the given number of line and samples per
/// pixel. Note that in general this won't exactly cover this MapInfo,
/// we trim lower left corner if needed to be the largest MapInfo that
/// fits in this MapInfo.
//-----------------------------------------------------------------------

MapInfo MapInfo::scale(double Number_x_per_pixel, 
		       double Number_y_per_pixel) const
{
  int nx_pixel = (int) floor(number_x_pixel() / Number_x_per_pixel);
  int ny_pixel = (int) floor(number_y_pixel() / Number_y_per_pixel);
  MapInfo res(*this);
  res.number_x_pixel_ = nx_pixel;
  res.number_y_pixel_ = ny_pixel;
  res.param(1) *= Number_x_per_pixel;
  res.param(2) *= Number_y_per_pixel;
  res.param(4) *= Number_x_per_pixel;
  res.param(5) *= Number_y_per_pixel;
  res.resolution_x_ = -1;
  res.resolution_y_ = -1;
  return res;
}

//-----------------------------------------------------------------------
/// We can read data sets that use longitude out of our convention of
/// -180.0 to 180.0. This is not an error, it is just different than
/// our convention. But in this case, we need to change to our convention.
//-----------------------------------------------------------------------

void MapInfo::latitude_range_correct()
{
  if(dynamic_cast<const GeodeticConverter*>(conv_.get()) &&
     param(0) >= 180)
    param(0) -= 360;
}
  

