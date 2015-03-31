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
/// MapInfo of underlying data.
//-----------------------------------------------------------------------

  const MapInfo& map_info() const {return map_info_;}

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
protected:

//-----------------------------------------------------------------------
/// Default constructor. Derived classes should call initialize before
/// exiting their constructor.
//-----------------------------------------------------------------------

  DemMapInfo() {}

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
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(DemMapInfo);
#endif
