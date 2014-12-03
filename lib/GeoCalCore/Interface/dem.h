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
#ifdef USE_BOOST_SERIALIZATON
  friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GenericObject);
  }
#endif
};
}
#endif
