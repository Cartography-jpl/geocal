#ifndef DEM_H
#define DEM_H
#include "ground_coordinate.h"
#include <boost/shared_ptr.hpp>
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  This class gives an interface to a digital elevation model (DEM).
*******************************************************************/

class Dem : public Printable<Dem> {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  Dem() {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Dem() {}

//-----------------------------------------------------------------------
/// Return distance to surface directly above/below the given point.
/// Distance is in meters. Positive means Gp is above the surface, 
/// negative means below.
//-----------------------------------------------------------------------

  virtual double distance_to_surface(const GroundCoordinate& Gp) const = 0;

//-----------------------------------------------------------------------
/// Height range, in meters relative to the reference surface. This is
/// for the area covered by the ULC to LRC. Note that this might be a
/// bit approximate, you might find a height out of this range. But
/// this should give a reasonable range to use for things like
/// generating an RSM etc. An optional "pad" can be given to extend
/// the range a bit to make sure we cover the DEM height range.
//-----------------------------------------------------------------------

  virtual void height_range(const GroundCoordinate& Ulc,
			    const GroundCoordinate& Lrc,
			    double& Min_h, double& Max_h,
			    double H_pad = 10.0) const = 0;
  
//-----------------------------------------------------------------------
/// Return height of surface above/below the reference surface (e.g.,
/// WGS-84 for the earth). Positive means above, negative below. This is 
/// in meters.
//-----------------------------------------------------------------------

  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const = 0;

  virtual boost::shared_ptr<CartesianFixed> intersect(const CartesianFixed& Cf,
      const CartesianFixedLookVector& Lv, double Resolution,
      double Max_height = 9000) const;

  virtual boost::shared_ptr<CartesianFixed> 
  intersect_start_length (const CartesianFixed& Cf,
			  const CartesianFixedLookVector& Lv, 
			  double Resolution, double Start_length) const;

//-----------------------------------------------------------------------
/// Return a GroundCoordinate on the surface directly above or below
/// the given point.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const = 0;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(Dem);
#endif
