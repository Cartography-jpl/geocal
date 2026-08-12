#include "geocal_config.h"
#include "refraction_msp.h"
#include "constant.h"
#include "ecr.h"
#include "geodetic.h"
#include "geocal_serialize_support.h"
#include "geocal_matrix.h"
#include "wgs84_constant.h"
#include <blitz/array.h>
#include <cmath>
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RefractionMsp::serialize
(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Refraction)
    & GEOCAL_NVP_(wavelength) & GEOCAL_NVP_(temperature)
    & GEOCAL_NVP_(pressure);
}

GEOCAL_IMPLEMENT(RefractionMsp);
#endif

// Some constants that we use in the calcuation

// Gas constant for air, in m^2 / (s^2 / K)
const double rho = 287.053;

// Polytropic index for tropopause. This is unitless
const double n = 4.256;

// TODO - Put comment here on what D is.
// Not sure what this is. in (s^2 * K) / m^2
const double D = 1 / (rho * (n + 1));

// Default absolute temperature at ellipsoid. This is just the average
// temperature over the earth. In K
const double t_default_ellipsoid = 294;

// Default pressure at ellipsoid, in millibar
const double p_default_ellipsoid = 1006;

// Default density at ellipsoid, in 100kg / m^3
const double d_default_ellipsoid =
  p_default_ellipsoid / (rho * t_default_ellipsoid);

// Gravitational constant, in m^2 / s^2. This is mass of the earth
// without atmosphere (see
// https://ahrs.readthedocs.io/en/stable/constants.html, called
// EARTH_GM_1 there).

const double gm_prime = 3.986000982e14;

inline double sqr(double x) { return x * x; }

namespace GeoCal {
  
double refraction_adapter(double x, void* params) 
{
  RefractionMsp* rmsp = static_cast<RefractionMsp*>(params);
  return rmsp->f_r(x);
}
  
}

//-----------------------------------------------------------------------
/// Constructor. You can supply the wavelength, temperature, and
/// pressure of the ground point if you have that information. Otherwise
/// the algorithm will use defaults.  
//-----------------------------------------------------------------------
  
RefractionMsp::RefractionMsp
(double Wavelength_micrometer,
 double Temperature_kelvin,
 double Pressure_millibar)
  : wavelength_(Wavelength_micrometer),
    temperature_(Temperature_kelvin),
    pressure_(Pressure_millibar),
    quad_table(0)
{
  // The MSP library uses a fixed 10 point gaussian quadrature for
  // its integrals. Not sure how important it is to use the fixed
  // versus a more generic integration, but no reason not to match
  // what it expects. There is a performance advantage to knowing the
  // number of quadrature points ahead of time.
  quad_table = gsl_integration_glfixed_table_alloc(10);
}

RefractionMsp::~RefractionMsp()
{
  if(quad_table)
    gsl_integration_glfixed_table_free(quad_table);
  quad_table = 0;
}

//-----------------------------------------------------------------------
/// Calculate a look vector, corrected for refraction. The calculation
/// to add refraction is almost the same as the reverse, so we just
/// pass in a flag to indicate which direction we should go
//-----------------------------------------------------------------------

CartesianFixedLookVector RefractionMsp::refraction_calc
(const GroundCoordinate& Spacecraft_pos,
 const GroundCoordinate& Gc_before_correction,
 bool Forward_calc) const
{
  // We use these in a few places, so as an optimization calculate
  // once and save
  Gc_before_correction.lat_lon_height(lat, lon ,hrefsurf);
  
  // The integration is split into target to tropopause, tropopause to
  // stratopause, stratopause to sensor. The documentation refers to
  // this as rp, r1, r2, ro (where r is radius from earth center in
  // meters).

  rp = r_calc(lat, hrefsurf);
  r1 = r_tropopause(Gc_before_correction);
  r2 = r_stratopause(Gc_before_correction);
  double lat_s, lon_s, h_s;
  Spacecraft_pos.lat_lon_height(lat_s, lon_s, h_s);
  ro = r_calc(lat_s, h_s);

  // Also need the ellipsoid surface radius, to calculate temperature
  // and pressure (default values are at ellipsoid, need to translate
  // to Gc_before_correction
  double re = r_ellipsoid(Gc_before_correction);

  // Snell's Law spherical symmetric structure.
  CartesianFixedLookVector lv(Spacecraft_pos, Gc_before_correction);
  auto ro_v = Spacecraft_pos.convert_to_cf();
  s = ro * sqrt(1 - sqr(dot(lv.direction(), ro_v->position) / ro));
  // Angle between light direction and ro_v.
  double a_o = acos(-dot(lv.direction(), ro_v->position) / ro);


  // Calculate temperature and pressure at the target point,
  // if not given. Use to calculate the density at the target point
  if(temperature_ > 0)
    t_rp = temperature_;
  else
    t_rp = t_default_ellipsoid + delta_temperature(rp, re);

  // Units of d_rp is 100kg / m^3
  if(pressure_ > 0)
    d_rp = pressure_ / (rho * t_rp);
  else
    d_rp = d_default_ellipsoid *
      d_scale_factor_tropopause(rp, re, t_default_ellipsoid);

  // Density function changes as we go from transition from
  // troposphere to stratosphere. Save the value at r1
  d_r1 = d_rp * d_scale_factor_tropopause(r1, rp, t_rp);
  t_r1 = temperature_r(r1);

  // Wrap f_r up so we can pass it to GSL library.
  gsl_function gf;
  gf.function = &refraction_adapter;
  gf.params = static_cast<void*>(const_cast<RefractionMsp*>(this));

  // Integration is divided up into 3 pieces
  double w1 = s * gsl_integration_glfixed(&gf, rp, r1, quad_table);
  double w2 = s * gsl_integration_glfixed(&gf, r1, r2, quad_table);
  double w3 = asin(s/r2) - asin(s/ro);
  double w = w1 + w2 + w3;
  // Geometric zenith of the primary mirror vertex at the ground point P
  double alpha_g = atan(ro * sin(w)/(ro*cos(w) - rp));

  // Geometric zenith of the ground point p at the primary mirror vertex
  double beta_g = alpha_g - w;

  // Angle of refraction
  double a_r = a_o - beta_g;

  // Switch direction if we are removing refraction. This isn't
  // exactly right, but the differences should be very small from
  // doing a full reversal.
  if(!Forward_calc)
    a_r *= -1;

  double c1 = sin(a_r)/tan(a_o)-cos(a_r);
  double c2 = sin(a_r) / sin(a_o);
  CartesianFixedLookVector
    lv2(-lv.direction()[0] * c1 - ro_v->position[0] / ro * c2,
	-lv.direction()[1] * c1 - ro_v->position[1] / ro * c2,
	-lv.direction()[2] * c1 - ro_v->position[2] / ro * c2);
  if(false) 
    std::cerr << std::setprecision(20) << "rp: " << rp << "\n"
	      << "r1: " << r1 << "\n"
	      << "r2: " << r2 << "\n"
	      << "ro: " << ro << "\n"
	      << "re: " << re << "\n"
	      << "alpha_g: " << alpha_g * Constant::rad_to_deg << "\n"
	      << "beta_g: " << beta_g * Constant::rad_to_deg << "\n"
	      << "a_o: " << a_o * Constant::rad_to_deg << "\n"
	      << "a_r: " << a_r * Constant::rad_to_deg << "\n"
	      << lv << "\n"
	      << lv2 << "\n";
  return lv2;
}

void RefractionMsp::print(std::ostream& Os) const
{
  Os << "RefractionMsp:\n"
     << "  Wavelength:  " << wavelength_ << " micrometer\n"
     << "  Temperature: " << temperature_ << " K\n"
     << "  Pressure:    " << pressure_ << "millibar\n";
}

//-----------------------------------------------------------------------
/// Distance from the earth center to the given point.
//-----------------------------------------------------------------------

double RefractionMsp::r_calc(double Lat, double H) const
{
  double clat = cos(Lat * Constant::deg_to_rad);
  double slat = sin(Lat * Constant::deg_to_rad);
  return sqrt((sqr(sqr(Constant::wgs84_a) * clat) +
	       sqr(sqr(Constant::wgs84_b) * slat)) /
	      (sqr(Constant::wgs84_a * clat) + sqr(Constant::wgs84_b * slat))) + H;
}

//-----------------------------------------------------------------------
/// Distance from center of earth to tropopause above the target point.
//-----------------------------------------------------------------------

double RefractionMsp::r_tropopause(const GroundCoordinate& Gc_target) const
{
  // We use these in a few places, so as an optimization calculate
  // once and save
  // double lat, lon, hrefsurf;
  // Gc_target.lat_lon_height(lat, lon ,hrefsurf);
  
  // Different height depending on if we are between -60 to 60 or not.
  // This height is in meters, and above WGS-84
  double h1 = (fabs(lat) < 60 ? 11019 : 8000);
  return r_calc(lat, h1);
}

//-----------------------------------------------------------------------
/// Distance from center of earth to stratopause above the target point.
//-----------------------------------------------------------------------

double RefractionMsp::r_stratopause(const GroundCoordinate& Gc_target) const
{
  // We use these in a few places, so as an optimization calculate
  // once and save
  // double lat, lon, hrefsurf;
  // Gc_target.lat_lon_height(lat, lon ,hrefsurf);
  
  double h2 = 50000;
  return r_calc(lat, h2);
}

//-----------------------------------------------------------------------
/// Distance from center of earth to ellipsoid.
//-----------------------------------------------------------------------

double RefractionMsp::r_ellipsoid(const GroundCoordinate& Gc_target) const
{
  // We use these in a few places, so as an optimization calculate
  // once and save
  // double lat, lon, hrefsurf;
  // Gc_target.lat_lon_height(lat, lon ,hrefsurf);
  
  return r_calc(lat, 0);
}

//-----------------------------------------------------------------------
/// Change in temperature (in K) when going from r_b to r_a. This is
/// up to the tropopause, past that we hold the temperature constant
/// in the simple model used for the refraction
//-----------------------------------------------------------------------

double RefractionMsp::delta_temperature(double r_a, double r_b) const
{
  return D * gm_prime * (1 / r_a - 1 / r_b);
}

//-----------------------------------------------------------------------
/// Scale factor for density function, taking us from the density at
/// r_b to r_a. Temperature at r_b should be supplied. This is valid
/// up to the tropopause.
//-----------------------------------------------------------------------

double RefractionMsp::d_scale_factor_tropopause
(double r_a, double r_b, double t_b) const
{
  return std::pow(1 + delta_temperature(r_a, r_b) / t_b, n);
}

//-----------------------------------------------------------------------
/// Scale factor for density function, taking us from the density at
/// r_b to r_a. Temperature at r_b should be supplied. This is valid
/// for above the tropopause to the stratopause.
//-----------------------------------------------------------------------

double RefractionMsp::d_scale_factor_stratopause
(double r_a, double r_b, double t_b) const
{
  return std::exp(gm_prime / (rho * t_b) * (1/r_a - 1/r_b));
}

//-----------------------------------------------------------------------
/// Temperature at radius r. We use delta_temperature up to the
/// tropopause, and then hold the temperature constant.
//-----------------------------------------------------------------------
  
double RefractionMsp::temperature_r(double r) const
{
  return t_rp + delta_temperature((r <= r1 ? r : r1), rp);
}

//-----------------------------------------------------------------------
/// Density at radius r.
//-----------------------------------------------------------------------

double RefractionMsp::d_r(double r) const
{
  if(r <= r1)
    return d_rp * d_scale_factor_tropopause(r, rp, t_rp);
  return d_r1 * d_scale_factor_stratopause(r, r1, t_r1);
}
      
//-----------------------------------------------------------------------
/// Gladstone-Dale equation.
//-----------------------------------------------------------------------

double RefractionMsp::k_r(double r) const
{
  const double a0 = 2.2125e-02;  // m^3/100kg
  const double a1 = 1.6764e-04;  // um^2 m^3/100kg
  return 1 + (a0 + a1 / sqr(wavelength_)) * d_r(r);
}

//-----------------------------------------------------------------------
/// Function we are integrating. Units 1/m^2
//-----------------------------------------------------------------------

double RefractionMsp::f_r(double r) const
{
  return 1 / (r * sqrt(sqr(r) * sqr(k_r(r)) - sqr(s)));
}

