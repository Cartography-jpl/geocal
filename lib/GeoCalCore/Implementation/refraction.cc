#include "geocal_config.h"
#include "refraction.h"
#include "constant.h"
#include "ecr.h"
#include "geodetic.h"
#include "geocal_serialize_support.h"
#include <blitz/array.h>
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Refraction::serialize
(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Refraction);
  ar & GEOCAL_NVP(alt) & GEOCAL_NVP(lat) & GEOCAL_NVP(index_ref)
    & GEOCAL_NVP(k1) &  GEOCAL_NVP(k2) & GEOCAL_NVP(dens_fac);
}

GEOCAL_IMPLEMENT(Refraction);
#endif

//-----------------------------------------------------------------------
/// This sets up for a Refraction calculation. A representative
/// altitude (in meters) and latitude (in degrees) is given. This
/// doesn't depend very strongly on altitude and latitude, so unless
/// you using widely varying points just using the center of you scene
/// should be pretty much sufficient.
///
/// The index of refraction at the surface can be supplied if desired,
/// otherwise we calculate an approximate value.
//-----------------------------------------------------------------------
Refraction::Refraction(double Altitude, double Latitude, 
		       double Index_refraction_surface)
  : alt(Altitude), lat(Latitude)
{
  // degrees temperature decrease per meter altitude increase
  const double troporate = 0.0065;       
  // universal gas constant
  const double gas_const = 8314.3;
  // mean sea level acceleration of gravity 
  // OK to use near sea level to our level of accuracy
  const double g = 9.805;
  // mean molecular wt in troposphere based
  // on 80 parts dry air (wt 28.96) plus 1 part water vapor (wt 18.01)
  const double mean_molec = 28.825;

  // calculate height of tropopause (m), scale height, and surface 
  // temperature (K) at the given latitude.  Based on fits to pp.
  // 120-121 of Allen's astrophysical Quantities.
  double xlat = fabs(Constant::deg_to_rad * lat);
  double tropopause = 19873.3 - 11345.4 * xlat + 2312.9 * xlat * xlat;
  double surf_temp = 253.73 + 44.52 * cos(xlat);
  // calculate exponent for adiabatic law
  double dens_exponent = g * mean_molec/(gas_const * troporate) -1.0;
  //  ratio of temperature at altitude to sea level
  double temp_fac;               
  if (altitude() <= tropopause) {
    // adiabatic density law
    temp_fac = 1.0 - troporate * altitude() / surf_temp;
    dens_fac = exp(dens_exponent * log(temp_fac)); 
  } else {
    // isothermal atmos starting at tropopause
    temp_fac = 1.0 - troporate * tropopause / surf_temp;
    dens_fac = exp(dens_exponent * log(temp_fac) - (altitude() - tropopause) *
		   g * mean_molec/(gas_const * surf_temp * temp_fac)); 
  }

  if(Index_refraction_surface > 0)
    index_ref = Index_refraction_surface; // Take value from user if
					  // supplied
  else {
    // Otherwise calculate an approximate value.

    // Allen's approximate sea level index of refraction minus 1.00
    const double dindex_0 = 0.0002905;
    index_ref = 1.000 + dindex_0 * dens_fac;
  }

  // ratio of term in tan(z) cubed to that in tan(z)--adjusted from 0.00115 for
  // smoother fit to equation from A.A. supplement
  const double allenfac = 0.00117;
  const double mean_earth_a = 6371000.0;  // mean Earth radius
  double dens_scale = temp_fac * surf_temp / (troporate * dens_exponent);
  k1 = 1.0 / (1 + dens_scale / mean_earth_a);
  k2 = allenfac * k1;
}

//-----------------------------------------------------------------------
/// Displacement in meters for given space zenith angle in radians.
//-----------------------------------------------------------------------

double Refraction::displacement(double Space_zenith) const
{
  double surf_zen = surface_zenith(Space_zenith);
  double refrac;

  // changeover angle from Allen style approx. to one from Explanatory 
  // Supplement (about 83.9 degrees)
  const double change_z = 1.465;
  if(surf_zen < change_z) {
    // use formula from Allen, p. 124 for z(z') for angles < 1.465 rad (z').
    // Allen's eq. is refraction = 58.3"arc * tan(z') - 0.067 "arc * (tan(z'))^3
    // where ^ = exponentiation operator.  The formula is adapted here by noting
    // that the number 58.3 arc seconds is just the index of refraction, less
    // 1.0, and converted from radians to arc seconds.  ALLENFAC is the ratio of
    // 0.067 to 58.3 - relating the 2 terms - adjusted slightly upward from
    // 0.00115 to 0.00117 for a smoother transition to the A.A. Supplement
    // Reversing the transformation and scaling with air density, we
    // get:
    double tz = tan(surf_zen);
    refrac = (index_ref - 1.0) * (k1 * tz - k2 * tz * tz * tz);
  } else {
     // refraction according to Eq. 3.283-1, p. 144, Astron. Almanac Supplement
     // with elevation = H

     // peculiar combination of units due to A. A. Suppl. !

     double elevation = Constant::pi/2 - surf_zen;
     refrac = dens_fac * (0.01667*Constant::deg_to_rad) / 
       tan(elevation + 7.31*Constant::deg_to_rad/ 
	   (elevation*Constant::rad_to_deg + 4.4));
  }
  refrac = std::max(refrac, 0.0);
  return std::max(Space_zenith - (surf_zen + refrac), 0.0);
}

//-----------------------------------------------------------------------
/// Apply refraction. We take the position of the spacecraft and the
/// ground coordinates before apply refraction. This return the ground
/// coordinates accounting for refraction.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate>
Refraction::refraction_apply(const GroundCoordinate& Spacecraft_pos,
			     const GroundCoordinate& Gc_no_refraction) const
{
  firstIndex i1; secondIndex i2;
  Array<double, 1> ecr_dir(3);
  Ecr gc_ecr(Gc_no_refraction);
  Ecr sp_ecr(Spacecraft_pos);
  ecr_dir(0) = sp_ecr.position[0] - gc_ecr.position[0];
  ecr_dir(1) = sp_ecr.position[1] - gc_ecr.position[1];
  ecr_dir(2) = sp_ecr.position[2] - gc_ecr.position[2];

//-----------------------------------------------------------------------
// Convert to view azimuth and zenith.
//
// Equations for this conversion can be found in "Level 1
// Georectification and Registration Algorithm Theoretical Basis" JPL
// D-11532, Rev. B section 6.3.1
//-----------------------------------------------------------------------

  Geodetic gc_geodetic(Gc_no_refraction);
#ifdef HAVE_SINCOS
  double sinlat, coslat, sinlon, coslon;
  sincos(Constant::deg_to_rad * gc_geodetic.latitude(), &sinlat, &coslat);
  sincos(Constant::deg_to_rad * gc_geodetic.longitude(), &sinlon, &coslon);
#else
  double sinlat = sin(gc_geodetic.latitude() * Constant::deg_to_rad);
  double coslat = cos(gc_geodetic.latitude() * Constant::deg_to_rad);
  double sinlon = sin(gc_geodetic.longitude() * Constant::deg_to_rad);
  double coslon = cos(gc_geodetic.longitude() * Constant::deg_to_rad);
#endif
  Array<double, 2> tlc(3, 3);
  tlc  = -coslon * sinlat, -sinlon * sinlat,  coslat,
                  -sinlon,           coslon,       0,
         -coslon * coslat, -sinlon * coslat, -sinlat;
  Array<double, 1> ln_dir(3);
  ln_dir = sum(tlc(i1, i2) * ecr_dir(i2), i2);
  double space_zen = 
    Constant::pi - acos(ln_dir(2) / sqrt(sum(ln_dir * ln_dir)));
  double azimuth = atan2(ln_dir(1), ln_dir(0));
  double disp = displacement(space_zen);
  // This is from table 6-4 of the "Theoretical Basis of the SDP
  // Toolkit Geolocation Package for the ECS"
  // Note that this breaks down at the poles. We just ignore that
  // problem here, but if needed we revisit this.
  double delta_lat = disp * cos(azimuth) * Constant::rad_to_deg;
  double delta_lon = disp * sin(azimuth) / coslat * 
    Constant::rad_to_deg;
  return boost::shared_ptr<GroundCoordinate>
    (new Geodetic(gc_geodetic.latitude() + delta_lat,
		  gc_geodetic.longitude() + delta_lon,
		  gc_geodetic.height_reference_surface()));
}

//-----------------------------------------------------------------------
/// Reverse the refraction calculation. This returns a ground
/// coordinate so that when we feed it to refraction_apply we get back
/// the original ground coordinate. This is useful when trying to go
/// the other way, from the ground to the spacecraft, so we remove the
/// effects of refraction and then continue with a normal orbit/camera
/// model. 
///
/// Note for speed we don't calculate the exact reverse of
/// refraction_apply, but a close approximation. For a MISR DF
/// camera, the difference is only 2 cm. For most applications, the
/// difference shouldn't matter. But if we have an application where this
/// does, we'll want to replace this with a more exact calculation.
//-----------------------------------------------------------------------
  
boost::shared_ptr<GroundCoordinate>
Refraction::refraction_reverse(const GroundCoordinate& Spacecraft_pos,
		       const GroundCoordinate& Gc_with_refraction) const
{
  // Gc_with_refraction is pretty close to Gc_no_refraction, so we can
  // pretty much just run the same calculation as the refraction_apply
  // but switch the signs of the displacment. This isn't exactly the
  // inverse of refraction_apply, but it is pretty close.
  firstIndex i1; secondIndex i2;
  Array<double, 1> ecr_dir(3);
  Ecr gc_ecr(Gc_with_refraction);
  Ecr sp_ecr(Spacecraft_pos);
  ecr_dir(0) = sp_ecr.position[0] - gc_ecr.position[0];
  ecr_dir(1) = sp_ecr.position[1] - gc_ecr.position[1];
  ecr_dir(2) = sp_ecr.position[2] - gc_ecr.position[2];

//-----------------------------------------------------------------------
// Convert to view azimuth and zenith.
//
// Equations for this conversion can be found in "Level 1
// Georectification and Registration Algorithm Theoretical Basis" JPL
// D-11532, Rev. B section 6.3.1
//-----------------------------------------------------------------------

  Geodetic gc_geodetic(Gc_with_refraction);
#ifdef HAVE_SINCOS
  double sinlat, coslat, sinlon, coslon;
  sincos(Constant::deg_to_rad * gc_geodetic.latitude(), &sinlat, &coslat);
  sincos(Constant::deg_to_rad * gc_geodetic.longitude(), &sinlon, &coslon);
#else
  double sinlat = sin(gc_geodetic.latitude() * Constant::deg_to_rad);
  double coslat = cos(gc_geodetic.latitude() * Constant::deg_to_rad);
  double sinlon = sin(gc_geodetic.longitude() * Constant::deg_to_rad);
  double coslon = cos(gc_geodetic.longitude() * Constant::deg_to_rad);
#endif
  Array<double, 2> tlc(3, 3);
  tlc  = -coslon * sinlat, -sinlon * sinlat,  coslat,
                  -sinlon,           coslon,       0,
         -coslon * coslat, -sinlon * coslat, -sinlat;
  Array<double, 1> ln_dir(3);
  ln_dir = sum(tlc(i1, i2) * ecr_dir(i2), i2);
  double space_zen = 
    Constant::pi - acos(ln_dir(2) / sqrt(sum(ln_dir * ln_dir)));
  double azimuth = atan2(ln_dir(1), ln_dir(0));
  double disp = displacement(space_zen);
  // This is from table 6-4 of the "Theoretical Basis of the SDP
  // Toolkit Geolocation Package for the ECS"
  // Note that this breaks down at the poles. We just ignore that
  // problem here, but if needed we revisit this.
  double delta_lat = disp * cos(azimuth) * Constant::rad_to_deg;
  double delta_lon = disp * sin(azimuth) / coslat * 
    Constant::rad_to_deg;
  return boost::shared_ptr<GroundCoordinate>
    (new Geodetic(gc_geodetic.latitude() - delta_lat,
		  gc_geodetic.longitude() - delta_lon,
		  gc_geodetic.height_reference_surface()));
}

