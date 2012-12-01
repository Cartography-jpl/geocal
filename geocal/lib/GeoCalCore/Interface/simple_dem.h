#ifndef SIMPLE_DEM_H
#define SIMPLE_DEM_H
#include "dem.h"
#include "geodetic.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple implementation of a Dem, intended primarily for use
  during testing. It find the height of a given point above the WGS84
  ellipsoid + fixed height, in geodetic coordinates.
*******************************************************************/

class SimpleDem : public Dem {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  SimpleDem(double H = 0) : h_(H) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~SimpleDem() {}

//-----------------------------------------------------------------------
/// Return height of surface above/below the reference surface (e.g.,
/// WGS-84 for the earth). Positive means above, negative below. This is 
/// in meters.
//-----------------------------------------------------------------------

  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const {return h_;}

  virtual double distance_to_surface(const GroundCoordinate& Gp) 
    const;
  virtual boost::shared_ptr<GroundCoordinate> 
    surface_point(const GroundCoordinate& Gp) const;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Simple Dem, height " << h_ << "m \n"; }

//-----------------------------------------------------------------------
/// Return height value used by this object.
//-----------------------------------------------------------------------

  double h() const {return h_;}

//-----------------------------------------------------------------------
/// Set height value used by this object.
//-----------------------------------------------------------------------

  void h(double Hnew) {h_ = Hnew;}
private:
  double h_;
};
}
#endif
