#ifndef DEM_MAP_INFO_H
#define DEM_MAP_INFO_H
#include "dem.h"
#include "map_info.h"
#include "geocal_datum.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  This handles a Dem from a file with a MapInfo.

  A very common Dem is one where the height is a geodetic height
  relative to a Datum on a regular grid in some map projection (e.g.,
  a GeoTiff file containing elevations on a latitude/longitude grid).
  This class implements surface_gp and distance_to_surface in terms
  of a derived class supplied "elevation" function.

  We do a bilinear interpolation to get heights in between the values
  given by elevation.

  Depending on what we are doing, calling with a location outside of
  the underlying DEM data might be ok, or might be something that
  should trigger an error. If requested, this class can just return a
  height_reference_surface of 0 everywhere outside of the Dem (so
  height_datum is -datum().undulation).

  Derived classes need to call the constructor or initialize(), and
  fill in the elavation() function.

*******************************************************************/
class DemMapInfoOffset;
class DemMapInfo : public Dem {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~DemMapInfo() {}

//-----------------------------------------------------------------------
/// Datum height is relative to.
//-----------------------------------------------------------------------

  const Datum& datum() const {return *datum_;}

//-----------------------------------------------------------------------
/// Pointer to datum
//-----------------------------------------------------------------------

  const boost::shared_ptr<Datum> datum_ptr() const {return datum_;}

//-----------------------------------------------------------------------
/// We need special handling for maps that are crossing the
/// dateline. For most projections this isn't an issue - we just pick
/// one that doesn't do anything in particular crossing the dateline. 
/// But for the special case of a Geodetic map projection, we flip
/// signs so the point with longitude -180 is right next to 179.999.
/// To handle this, we can change the map projection to work with a
/// Geodetic360. This is really the same map projection, we just label
/// the longitude differently.
///
/// Note this is a noop if the coordinate converter is already a
/// Geodetic360Converter, and changes if it is a
/// GeodeticConverter. Any other projection will cause an exception to
/// be thrown.
//-----------------------------------------------------------------------

  virtual void change_to_geodetic360()
  {
    map_info_.change_to_geodetic360();
  }

//-----------------------------------------------------------------------
/// Change back to -180 to 180
//-----------------------------------------------------------------------

  virtual void change_to_geodetic()
  {
    map_info_.change_to_geodetic();
  }

//-----------------------------------------------------------------------
/// Simple test to see if we are Geodetic360, sometimes class need
/// special handling for this
//-----------------------------------------------------------------------

  bool is_geodetic_360() const { return map_info_.is_geodetic_360(); }
  
//-----------------------------------------------------------------------
/// MapInfo of underlying data.
//-----------------------------------------------------------------------

  const MapInfo& map_info() const {return map_info_;}

  virtual void height_range(const GroundCoordinate& Ulc,
			      const GroundCoordinate& Lrc,
			      double& Min_h, double& Max_h,
			      double H_pad = 10.0) const;
  virtual double distance_to_surface(const GroundCoordinate& Gp) const;
  double height_datum(const GroundCoordinate& Gp) const;
  double height_datum(const Geodetic& Gp) const;
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const;
  double height_reference_surface(const Geodetic& Gp) 
    const;

  virtual boost::shared_ptr<CartesianFixed> intersect(const CartesianFixed& Cf,
      const CartesianFixedLookVector& Lv, double Resolution,
      double Max_height = 9000) const;

//-----------------------------------------------------------------------
/// If true, then calling with values outside the range of area of the
/// DEM triggers an exception, otherwise we just return a height of 0.
//-----------------------------------------------------------------------
  bool outside_dem_is_error() const {return outside_dem_is_error_; }
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const;
  int naif_code() const {return naif_code_;}


  double slope_riserun(int Y_index, int X_index) const;

//-----------------------------------------------------------------------
/// Slope for the given point, as a percentage.
//-----------------------------------------------------------------------
  double slope_percentage(int Y_index, int X_index) const
  { return slope_riserun(Y_index, X_index) * 100.0; }

//-----------------------------------------------------------------------
/// Slope for the given point, as radians.
//-----------------------------------------------------------------------
  double slope_radian(int Y_index, int X_index) const
  { return atan(slope_riserun(Y_index, X_index)); }

//-----------------------------------------------------------------------
/// Slope for the given point, as degrees.
//-----------------------------------------------------------------------
  double slope_degree(int Y_index, int X_index) const
  { return slope_radian(Y_index, X_index) * Constant::rad_to_deg; }
  double aspect(int Y_index, int X_index) const;
  
  void gradient(int Y_index, int X_index, double& dz_dx, double& dz_dy) const;
  void slope_and_aspect(const GroundCoordinate& Gc,
			double& Slope_deg, double& Aspect_deg) const;
  void slope_and_aspect_nearest_neighbor(const GroundCoordinate& Gc,
	 double& Slope_deg, double& Aspect_deg) const;
protected:

//-----------------------------------------------------------------------
/// Default constructor. Derived classes should call initialize before
/// exiting their constructor.
//-----------------------------------------------------------------------

  DemMapInfo() : naif_code_(Geodetic::EARTH_NAIF_CODE) {}

//-----------------------------------------------------------------------
/// Constructor. The flag Outside_dem_is_error is used to indicate if
/// we should treat requests outside of the Dem as an error (throwing
/// an exception), or just use a default value of a
/// height_reference_surface of 0.  
//-----------------------------------------------------------------------

  DemMapInfo(const boost::shared_ptr<Datum>& D, const MapInfo& M, 
	     bool Outside_dem_is_error = false)
  { initialize(D, M, Outside_dem_is_error); }

//-----------------------------------------------------------------------
/// Return height in meters relative to datum(). Note that the call is
/// in line, sample order, which means Y and then X. Regardless of the
/// setting of outside_dem_is_error_, this will never get called with
/// values outside the range of MapInfo, i.e., the derived class
/// doesn't need any special handling for values outside of its range. 
//-----------------------------------------------------------------------

  virtual double elevation(int Y_index, int X_index) const = 0;

  void initialize(const boost::shared_ptr<Datum>& D, const MapInfo& M, 
		  bool Outside_dem_is_error = false);
  friend class DemMapInfoOffset; // Allows this class access to the
				 // protected member elevation.
  boost::shared_ptr<Datum> datum_; ///< Datum height is relative to.
  MapInfo map_info_;		   ///< Map info used to look up
				   ///elevation values.
  mutable bool outside_dem_is_error_;	   ///< If false, height requests
				   ///outside of underlying Dem will
				   ///return 0, otherwise throw an
				   ///exception.
  int naif_code_;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(DemMapInfo);
GEOCAL_CLASS_VERSION(DemMapInfo, 1);
#endif
