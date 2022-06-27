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
/// Calculate dz_dx and dz_dy, which is the first step in calculating
/// slope and aspect of the DEM.
///
/// This uses the second order ARCINFO method. We handle the grid
/// edges by just using the elevation as Y_index, X_index.
///
/// This is also the method used by richdem (a python package), which
/// references Horn, B.K.P., 1981. Hill shading and the reflectance
/// map. Proceedings of the IEEE 69,
/// 14–47. doi:10.1109/PROC.1981.11918 as the original source of this 
/// algorithm
//-----------------------------------------------------------------------

void DemMapInfo::gradient(int Y_index, int X_index,
		    double& dz_dx, double& dz_dy) const
{
  // Note that this isn't really too efficient. We can try to generate
  // a more optimized version of this in a derived class if needed.
  if(X_index < 0 || X_index + 1 >= map_info_.number_x_pixel() ||
     Y_index < 0 || Y_index + 1 >= map_info_.number_y_pixel()) {
    if(outside_dem_is_error_) {
      Exception e;
      e << "gradient requested outside of the range of the Dem data "
	<< "xindex: " << X_index << " (max " << map_info_.number_x_pixel() - 1
	<< ") yindex: " << Y_index << " (max " << map_info_.number_y_pixel() - 1
	<< ")";
      throw e;
    } else {
      dz_dx = 0;
      dz_dy = 0;
      return;
    }
  }
  double v[3][3];
  for(int i = -1; i < 2; ++i)
    for(int j = -1; j < 2; ++j) {
      int yi = Y_index + i;
      int xi = X_index + j;
      if(xi < 0 || xi + 1 >= map_info_.number_x_pixel() ||
	 yi < 0 || yi + 1 >= map_info_.number_y_pixel()) {
	v[i+1][j+1] = elevation(Y_index, X_index);
      } else {
	v[i+1][j+1] = elevation(yi, xi);
      }
    }
  double a = v[0][0];
  double b = v[0][1];
  double c = v[0][2];
  double d = v[1][0];
  // Unused, comment out just so we don't get a compiler warning
  //double e = v[1][1];
  double f = v[1][2];
  double g = v[2][0];
  double h = v[2][1];
  double k = v[2][2];
  dz_dx = ((c + 2*f + k) - (a + 2*d + g)) / 8 / map_info_.resolution_x();
  dz_dy = ((g + 2*h + k) - (a + 2*b + c)) / 8 / map_info_.resolution_y();
}

//-----------------------------------------------------------------------
/// Slope for the given point, as a Rise/Run
//-----------------------------------------------------------------------
double DemMapInfo::slope_riserun(int Y_index, int X_index) const
{
  double dz_dx, dz_dy;
  gradient(Y_index, X_index, dz_dx, dz_dy);
  return hypot(dz_dx, dz_dy);
}

//-----------------------------------------------------------------------
/// Aspect angle. This uses the Horn 1981 definition, in degrees
/// [0, 360). 0 is north, we increase in a clockwise fashion. Note for
/// perfectly flat areas (slope_riserun is 0) this isn't really
/// defined, but by convention we return 270 (directly west).
///
/// Aspect is the downslope direction for the maximum rate of change
/// in value for each pixel.
//-----------------------------------------------------------------------

double DemMapInfo::aspect(int Y_index, int X_index) const
{
  double dz_dx, dz_dy;
  gradient(Y_index, X_index, dz_dx, dz_dy);
  double d = 90 - atan2(dz_dy, -dz_dx) * Constant::rad_to_deg;
  if(d < 0)
    d += 360;
  return d;
}

//-----------------------------------------------------------------------
/// Frequently we want both the slope and aspect, this one function
/// saves a step and returns both.
///
/// There isn't really "one" way to handle slope and aspect for points
/// that don't lie on the DEM grid. MISR had a larger footprint and
/// calculated an average of slopes/aspect that fell in to the
/// footprint. Another approach is to do a bilinear interpolation.
///
/// This particular function just uses the nearest neighbor. Slope and
/// aspect are a bit approximate anyways, so this is probably as good
/// as any other approach if the DEM resolution is similar to your
/// pixel size.
//-----------------------------------------------------------------------

void DemMapInfo::slope_and_aspect_nearest_neighbor
(const GroundCoordinate& Gc, double& Slope_deg,
 double& Aspect_deg) const
{
  double x, y;
  map_info_.coordinate(Gc, x, y);
  int xi = (int) round(x);
  int yi = (int) round(y);
  double dz_dx, dz_dy;
  gradient(yi, xi, dz_dx, dz_dy);
  Slope_deg = atan(hypot(dz_dx, dz_dy)) * Constant::rad_to_deg;
  Aspect_deg = 90 - atan2(dz_dy, -dz_dx) * Constant::rad_to_deg;
  if(Aspect_deg < 0)
    Aspect_deg += 360;
}

//-----------------------------------------------------------------------
/// Frequently we want both the slope and aspect, this one function
/// saves a step and returns both.
///
/// There isn't really "one" way to handle slope and aspect for points
/// that don't lie on the DEM grid. MISR had a larger footprint and
/// calculated an average of slopes/aspect that fell in to the
/// footprint. Another approach is to do a bilinear interpolation.
///
/// This particular function just uses bilinear interpolation. 
//-----------------------------------------------------------------------

void DemMapInfo::slope_and_aspect
(const GroundCoordinate& Gc, double& Slope_deg,
 double& Aspect_deg) const
{
  double x, y;
  map_info_.coordinate(Gc, x, y);
  int xi = (int) x;
  int yi = (int) y;
  double dz_dx1, dz_dy1,
    dz_dx2, dz_dy2,
    dz_dx3, dz_dy3,
    dz_dx4, dz_dy4;
  gradient(yi, xi, dz_dx1, dz_dy1);
  gradient(yi, xi+1, dz_dx2, dz_dy2);
  gradient(yi+1, xi, dz_dx3, dz_dy3);
  gradient(yi+1, xi+1, dz_dx4, dz_dy4);
  double t1 = atan(hypot(dz_dx1, dz_dy1)) * Constant::rad_to_deg;
  double t2 = atan(hypot(dz_dx2, dz_dy2)) * Constant::rad_to_deg;
  double t3 = atan(hypot(dz_dx3, dz_dy3)) * Constant::rad_to_deg;
  double t4 = atan(hypot(dz_dx4, dz_dy4)) * Constant::rad_to_deg;
  double t5 = t1 + (t2 - t1) * (x - xi);
  double t6 = t3 + (t4 - t3) * (x - xi);
  Slope_deg = t5 + (t6 - t5) * (y - yi);

  t1 = 90 - atan2(dz_dy1, -dz_dx1) * Constant::rad_to_deg;
  if(t1 < 0)
    t1 += 360;
  t2 = 90 - atan2(dz_dy2, -dz_dx2) * Constant::rad_to_deg;
  if(t2 < 0)
    t2 += 360;
  t3 = 90 - atan2(dz_dy3, -dz_dx3) * Constant::rad_to_deg;
  if(t3 < 0)
    t3 += 360;
  t4 = 90 - atan2(dz_dy4, -dz_dx4) * Constant::rad_to_deg;
  if(t4 < 0)
    t4 += 360;
  // Special handling for data that might cross the 0/360 boundary
  if(t1 > 300 || t2 > 300 || t3 > 300 || t4 > 300) {
    // If one of the variables is near the top end, then convert any
    // data on the lower end to the 360-720 range.
    if(t1 < 100)
      t1 += 360;
    if(t2 < 100)
      t2 += 360;
    if(t3 < 100)
      t3 += 360;
    if(t4 < 100)
      t4 += 360;
  }
  t5 = t1 + (t2 - t1) * (x - xi);
  t6 = t3 + (t4 - t3) * (x - xi);
  Aspect_deg = t5 + (t6 - t5) * (y - yi);
  if(Aspect_deg >= 360)
    Aspect_deg -= 360;
  if(Aspect_deg < 0)
    Aspect_deg += 360;
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
