#ifndef MSPI_GIMBAL_H
#define MSPI_GIMBAL_H
#include "with_parameter.h"
#include "auto_derivative_quaternion.h"

namespace GeoCal {
/****************************************************************//**
  This handles the AirMSPI gimbal calculations.
*******************************************************************/

class MspiGimbal : public Printable<MspiGimbal>,
		   public WithParameter {
public:
//-----------------------------------------------------------------------
/// Constructor. The angles should be in degrees.
//-----------------------------------------------------------------------

  MspiGimbal(double Epsilon, double Psi, double Theta)
    : epsilon_(Epsilon), psi_(Psi), theta_(Theta), pm(3), cache_valid(false)
  { pm = true; }

//-----------------------------------------------------------------------
/// Constructor, which creates a MspiGimbal from the given
/// configuration file.
//-----------------------------------------------------------------------

  MspiGimbal(const std::string& File_name, 
	     const std::string& Extra_config_file = "")
    : pm(3), cache_valid(false)
  { pm = true; read_config_file(File_name, Extra_config_file); }

  void read_config_file(const std::string& File_name,
			const std::string& Extra_config_file = "");

//-----------------------------------------------------------------------
/// Gimbal epsilon in degrees.
//-----------------------------------------------------------------------

  double epsilon() const { return epsilon_.value(); }
  AutoDerivative<double> epsilon_with_derivative() const 
  { return epsilon_; }
  
//-----------------------------------------------------------------------
/// Gimbal psi in degrees. We don't actually include the gimbal
/// angles in the camera model, instead this is handled by
/// AirMspiOrbit. However the gimbal parameters are recorded in the
/// camera configuration file, so it make sense to read and report
/// them here.
//-----------------------------------------------------------------------

  double psi() const { return psi_.value(); }
  AutoDerivative<double> psi_with_derivative() const 
  { return psi_; }

//-----------------------------------------------------------------------
/// Gimbal theta in degrees. We don't actually include the gimbal
/// angles in the camera model, instead this is handled by
/// AirMspiOrbit. However the gimbal parameters are recorded in the
/// camera configuration file, so it make sense to read and report
/// them here.
//-----------------------------------------------------------------------

  double theta() const { return theta_.value(); }
  AutoDerivative<double> theta_with_derivative() const 
  { return theta_; }

  boost::math::quaternion<AutoDerivative<double> >  
  station_to_sc(const AutoDerivative<double>& Gimbal_pos) const;

  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual blitz::Array<bool, 1> parameter_mask() const
  { return pm; }
  virtual void parameter_mask(const blitz::Array<bool, 1>& Pm);
  virtual void print(std::ostream& Os) const;
private:
  AutoDerivative<double> epsilon_, psi_, theta_;
  blitz::Array<bool, 1> pm;
  mutable bool cache_valid;
  mutable boost::math::quaternion<AutoDerivative<double> > m;

  MspiGimbal() : cache_valid(false) {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(MspiGimbal);
#endif
