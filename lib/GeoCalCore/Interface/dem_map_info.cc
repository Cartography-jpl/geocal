#include "dem_map_info.h"
#include "geocal_serialize_support.h"
#include "planet_coordinate.h"
#include <cmath>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void DemMapInfo::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Dem);
  GEOCAL_BASE(DemMapInfo, Dem);
  ar & GEOCAL_NVP_(datum) & GEOCAL_NVP_(map_info) 
    & GEOCAL_NVP_(outside_dem_is_error);
  // Older version didn't have naif_code_. The default was to always
  // assume EARTH_NAIF_CODE.
  if(version > 0)
    ar & GEOCAL_NVP_(naif_code);
}

GEOCAL_IMPLEMENT(DemMapInfo);
#endif

//-----------------------------------------------------------------------
/// Return distance to surface directly above/below the given point.
/// Distance is in meters. Positive means Gp is above the surface, 
/// negative means below.
//-----------------------------------------------------------------------

double DemMapInfo::distance_to_surface(const GroundCoordinate& Gp) const
{
  return Gp.height_reference_surface() - height_reference_surface(Gp);
}

//-----------------------------------------------------------------------
/// Height relative to datum
//-----------------------------------------------------------------------

double DemMapInfo::height_datum(const GroundCoordinate& Gp) const
{
  double x, y;
  map_info_.coordinate(Gp, x, y);
  int xi = (int) floor(x);
  int yi = (int) floor(y);
  if(xi < 0 || xi + 1 >= map_info_.number_x_pixel() ||
     yi < 0 || yi + 1 >= map_info_.number_y_pixel()) {
    if(outside_dem_is_error_) {
      Exception e;
      e << "Height requested outside of the range of the Dem data "
	<< "xindex: " << xi << " (max " << map_info_.number_x_pixel() - 1
	<< ") yindex: " << yi << " (max " << map_info_.number_y_pixel() - 1
	<< ")";
      throw e;
    } else
      return -datum().undulation(Gp);
  }
  double e1 = elevation(yi, xi);
  double e2 = elevation(yi, xi + 1);
  double e3 = elevation(yi + 1, xi);
  double e4 = elevation(yi + 1, xi + 1);
  double t1 = e1 + (e2 - e1) * (x - xi);
  double t2 = e3 + (e4 - e3) * (x - xi);
  return t1 + (t2 - t1) * (y - yi);
}

double DemMapInfo::height_datum(const Geodetic& Gp) const
{
  double x, y;
  map_info_.coordinate(Gp, x, y);
  int xi = (int) floor(x);
  int yi = (int) floor(y);
  if(xi < 0 || xi + 1 >= map_info_.number_x_pixel() ||
     yi < 0 || yi + 1 >= map_info_.number_y_pixel()) {
    if(outside_dem_is_error_) {
      Exception e;
      e << "Height requested outside of the range of the Dem data "
	<< "xindex: " << xi << " (max " << map_info_.number_x_pixel() - 1
	<< ") yindex: " << yi << " (max " << map_info_.number_y_pixel() - 1
	<< ")";
      throw e;
    } else
      return -datum().undulation(Gp);
  }
  double e1 = elevation(yi, xi);
  double e2 = elevation(yi, xi + 1);
  double e3 = elevation(yi + 1, xi);
  double e4 = elevation(yi + 1, xi + 1);
  double t1 = e1 + (e2 - e1) * (x - xi);
  double t2 = e3 + (e4 - e3) * (x - xi);
  return t1 + (t2 - t1) * (y - yi);
}

//-----------------------------------------------------------------------
/// Return height of surface above/below the reference surface (e.g.,
/// WGS-84 for the earth). Positive means above, negative below. This is 
/// in meters.
//-----------------------------------------------------------------------

double DemMapInfo::height_reference_surface(const GroundCoordinate& Gp) const
{
  return height_datum(Gp) + datum().undulation(Gp);
}

//-----------------------------------------------------------------------
/// Specialization of height_reference_surface if GroundCoordinate is 
/// already a Geodetic. Since many of our DEMS are in geodetic
/// coordinate, this means that this can often be faster.
//-----------------------------------------------------------------------

double DemMapInfo::height_reference_surface(const Geodetic& Gp) const
{
  return height_datum(Gp) + datum().undulation(Gp);
}


// See base class for description
boost::shared_ptr<CartesianFixed> DemMapInfo::intersect
(const CartesianFixed& Cf,
 const CartesianFixedLookVector& Lv, double Resolution,
 double Max_height) const
{
  // Regardless of outside_dem_is_error_ value, we allow intersect to
  // explore areas outside of the DEM bounding box. However, the
  // solution found must satisfy the constraint imposed by
  // outside_dem_is_error_.
  bool outside_dem_is_error_save = outside_dem_is_error_;
  boost::shared_ptr<CartesianFixed> res;
  try {
    outside_dem_is_error_ = false;
    res = Dem::intersect(Cf, Lv, Resolution, Max_height);
  } catch(...) {
    outside_dem_is_error_ = outside_dem_is_error_save;
    throw;
  }
  outside_dem_is_error_ = outside_dem_is_error_save;
  // Side effect of this is to check outside_dem_is_error_ is
  // satisfied.
  height_reference_surface(*res);
  return res;			// Make it here, then things are ok.
}

//-----------------------------------------------------------------------
/// Return a GroundCoordinate on the surface directly above or below
/// the given point.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate> 
DemMapInfo::surface_point(const GroundCoordinate& Gp) const
{
  if(naif_code_ == Geodetic::EARTH_NAIF_CODE) {
    Geodetic g(Gp);
    return boost::shared_ptr<GroundCoordinate>(new 
     Geodetic(g.latitude(), g.longitude(), height_reference_surface(g)));
  }
  Planetocentric g(Gp);
  return boost::shared_ptr<GroundCoordinate>
    (new Planetocentric(g.latitude(), g.longitude(),
			height_reference_surface(g),
			g.naif_code()));
}

//-----------------------------------------------------------------------
/// Finish initialization of class. The flag Outside_dem_is_error is
/// used to indicate if we should treat requests outside of the Dem as
/// an error (throwing an exception), or just use a default value of a
/// height of 0.  
//-----------------------------------------------------------------------

void DemMapInfo::initialize(const boost::shared_ptr<Datum>& D, 
			    const MapInfo& M, 
			    bool Outside_dem_is_error)
{
  datum_ = D;
  map_info_ = M;
  outside_dem_is_error_ = Outside_dem_is_error;
  try {
    naif_code_ = M.ground_coordinate(0,0)->naif_code();
  } catch(const Exception& E) {
    // We have DEMs where 0,0 is actually invalid point. For example
    // etop02nobath.hlf cover the whole earth including the north pole.
    // 0,0 is actually an invalid value. So if 0,0 fails, try the center
    // as backup.
    naif_code_ = M.ground_coordinate(M.number_x_pixel() / 2,
				     M.number_y_pixel() / 2)->naif_code();
  }
}

//-----------------------------------------------------------------------
/// Height range, in meters relative to the reference surface. This is
/// for the area covered by the ULC to LRC. Note that this might be a
/// bit approximate, you might find a height out of this range. But
/// this should give a reasonable range to use for things like
/// generating an RSM etc. An optional "pad" can be given to extend
/// the range a bit to make sure we cover the DEM height range.
///
/// Note that for values outside of the DEM range we handle this
/// either as:
/// 1. If outside_dem_is_error() is false, we just treat this a height
///    of 0.
/// 2. If outside_dem_is_error() is true, we ignore points outside of
///    the range. This is because when creating the area we are
///    working with if we are near the edge of the DEM we could end up
///    going past the edge. In this case, we assume that the range
///    desired is the range actually in the DEM.
//-----------------------------------------------------------------------

void DemMapInfo::height_range
(const GroundCoordinate& Ulc,
 const GroundCoordinate& Lrc,
 double& Min_h, double& Max_h,
 double H_pad) const
{
  double x1, y1, x2, y2;
  map_info_.coordinate(Ulc, x1, y1);
  map_info_.coordinate(Lrc, x2, y2);
  int imin = floor(std::min(y1, y2));
  int imax = ceil(std::max(y1, y2));
  int jmin = floor(std::min(x1, x2));
  int jmax = ceil(std::max(x1, x2));
  Min_h = std::numeric_limits<double>::max();
  Max_h = std::numeric_limits<double>::min();
  bool had_point = false;
  for(int i = imin; i <= imax; ++i)
    for(int j = jmin; j <= jmax; ++j)
      if(i < 0 || i >= map_info_.number_y_pixel() ||
	 j < 0 || j >= map_info_.number_x_pixel()) {
	if(!outside_dem_is_error_) {
	  Min_h = std::min(0.0, Min_h);
	  Max_h = std::max(0.0, Max_h);
	  had_point = true;
	}
      } else {
	double h = elevation(i,j) + datum().undulation(*map_info_.ground_coordinate(i,j));
	Min_h = std::min(h, Min_h);
	Max_h = std::max(h, Max_h);
	had_point = true;
      }
  if(!had_point) {
    Min_h = 0;
    Max_h = 0;
  }
  Min_h -= H_pad;
  Max_h += H_pad;
}
