#ifndef REFRACTION_MSP_H
#define REFRACTION_MSP_H
#include "refraction.h"
#include "look_vector.h"
#include <gsl/gsl_integration.h>

namespace GeoCal {
/****************************************************************//**
  This calculates refraction. The algorithm used here is the one the
  MSP library uses.
*******************************************************************/
class RefractionMsp : public Refraction,
		      public boost::noncopyable{
public:
  RefractionMsp(double Wavelength_micrometer=0.6,
		double Temperature_kelvin = -1,
		double Pressure_millibar = -1);
  virtual ~RefractionMsp();

  CartesianFixedLookVector refraction_calc
  (const GroundCoordinate& Spacecraft_pos,
   const GroundCoordinate& Gc_no_refraction) const;
  // Do we want the DEM?
  virtual boost::shared_ptr<GroundCoordinate>
  refraction_apply(const GroundCoordinate& Spacecraft_pos,
  		   const GroundCoordinate& Gc_no_refraction) const
  { throw Exception("Not implemented yet"); }
  virtual boost::shared_ptr<GroundCoordinate>
  refraction_reverse(const GroundCoordinate& Spacecraft_pos,
  		     const GroundCoordinate& Gc_with_refraction) const
  { throw Exception("Not implemented yet"); }
  virtual void print(std::ostream& Os) const;
  
//-----------------------------------------------------------------------
/// Wavelength, in micrometers.
//-----------------------------------------------------------------------
  
  double wavelength() const { return wavelength_; }

//-----------------------------------------------------------------------
/// Temperature of ground point, in kelvin. If < 0, the algorithm
/// calculates a default value.
//-----------------------------------------------------------------------
  
  double temperature() const { return temperature_; }

//-----------------------------------------------------------------------
/// Pressure of ground point, in millibars. If < 0, the algorithm
/// calculates a default value.
//-----------------------------------------------------------------------

  double pressure() const { return pressure_;}
private:
  double wavelength_, temperature_, pressure_;
  gsl_integration_glfixed_table *quad_table;
  double r_calc(const GroundCoordinate& Gc) const;
  double r_tropopause(const GroundCoordinate& Gc_target) const;
  double r_stratopause(const GroundCoordinate& Gc_target) const;
  double r_ellipsoid(const GroundCoordinate& Gc_target) const;
  double delta_temperature(double r_a, double r_b) const;
  double d_scale_factor_tropopause(double r_a, double r_b, double t_b) const;
  double d_scale_factor_stratopause(double r_a, double r_b, double t_b) const;
  double temperature_r(double r) const;
  double d_r(double r) const;
  double k_r(double r) const;
  double f_r(double r) const;
  
  // We use these in a few places, so as an optimization calculate
  // once and save
  mutable double rp, r1, r2, ro, s;
  mutable double lat, lon, hrefsurf, t_rp, d_rp, t_r1, d_r1;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RefractionMsp);
#endif
