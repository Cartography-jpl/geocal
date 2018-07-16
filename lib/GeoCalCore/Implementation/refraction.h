#ifndef REFRACTION_H
#define REFRACTION_H
#include "ground_coordinate.h"
#include <cmath>

namespace GeoCal {
/****************************************************************//**
  This calculates refraction. The algorithm used was take from the 
  SDP toolkit. The original code was written by Peter Noerdlinger as
  the SDP toolkit function PGS_CSC_SpaceRefract. The algorithm is
  described in detail in "Theoretical Basis of the SDP Toolkit
  Geolocation Package for the ECS".

  This depends on the index of refraction of air at the surface. We
  don't actually know this. There are various approximations, and the
  toolkit uses one described in the "Theoretical Basis of the SDP Toolkit
  Geolocation Package for the ECS".

  This class allows the index of refraction to simple be given. This
  allows us to just fit for this unknown using something like a
  simultanous bundle adjustment. Note that a reasonable range of
  values is something like 1.00026 to 1.00029, so you can vary from
  the initial prediction by a few 1e-5.
*******************************************************************/
class Refraction : public Printable<Refraction> {
public:
  Refraction(double Altitude, double Latitude, 
	     double Index_refraction_surface = -1);
  virtual ~Refraction() {}

//-----------------------------------------------------------------------
/// Altitude of reference point, in meters.
//-----------------------------------------------------------------------

  double altitude() const { return alt;}

//-----------------------------------------------------------------------
/// Latitude of reference point, in degrees.
//-----------------------------------------------------------------------

  double latitude() const {return lat;}

//-----------------------------------------------------------------------
/// Return the current index of refraction at the surface.
//-----------------------------------------------------------------------

  double index_refraction_surface() const {return index_ref;}

//-----------------------------------------------------------------------
/// Set the index of refraction.
//-----------------------------------------------------------------------
  
  void index_refraction_surface(double Index_refraction_surface)
  { index_ref = Index_refraction_surface; }

//-----------------------------------------------------------------------
/// Surface zenith angle in radians, given the space zenith angles.
//-----------------------------------------------------------------------

  double surface_zenith(double Space_zenith) const
  { return asin(sin(Space_zenith) / index_ref); }

  double displacement(double Space_zenith) const;
  boost::shared_ptr<GroundCoordinate>
  refraction_apply(const GroundCoordinate& Spacecraft_pos,
  		   const GroundCoordinate& Gc_no_refraction) const;
  boost::shared_ptr<GroundCoordinate>
  refraction_reverse(const GroundCoordinate& Spacecraft_pos,
  		     const GroundCoordinate& Gc_with_refraction) const;

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "Refraction\n"
       << "  Altitude: " << altitude() << " m\n"
       << "  Latitude: " << latitude() << " deg\n"
       << "  Index refraction at surface: "
       << index_refraction_surface() << "\n";
  }
protected:
  Refraction() {}
private:
  double alt, lat, index_ref;
  double k1, k2, dens_fac;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Refraction);
#endif
