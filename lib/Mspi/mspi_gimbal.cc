#include "mspi_gimbal.h"
#include "mspi_config_file.h"
#include "constant.h"
#include "geocal_serialize_support.h"
#include "geocal_quaternion.h"
#include "geocal_autoderivative_quaternion.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void MspiGimbal::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(MspiGimbal, WithParameter);
  ar & GEOCAL_NVP_(epsilon) & GEOCAL_NVP_(psi) & GEOCAL_NVP_(theta)
    & GEOCAL_NVP_(parameter_mask);
}

GEOCAL_IMPLEMENT(MspiGimbal);
#endif

//-----------------------------------------------------------------------
/// Read in the given configuration file.
//-----------------------------------------------------------------------

void MspiGimbal::read_config_file(const std::string& File_name,
				  const std::string& Extra_config_file)
{
  MspiConfigFile c(File_name);
  if(Extra_config_file != "")
    c.add_file(Extra_config_file);
  if(c.have_key("gimbal_epsilon")) {
    epsilon_ = c.value<double>("gimbal_epsilon");
    psi_ = c.value<double>("gimbal_psi");
    theta_ = c.value<double>("gimbal_theta");
  } else {
    epsilon_ = 0;
    psi_ = 0;
    theta_ = 0;
  }
}

//-----------------------------------------------------------------------
/// Return the station to spacecraft transformation. The gimbal
/// position should be in *radians*. This is inconsistent with the
/// other angles used by this class that are in degrees, but this
/// matches what AirMSPI navigation data is stored as.
//-----------------------------------------------------------------------

boost::math::quaternion<AutoDerivative<double> >  
MspiGimbal::station_to_sc_with_derivative(const AutoDerivative<double>& Gimbal_pos) const
{
  fill_in_cache();
  return m_with_der * quat_rot("y", Gimbal_pos);
}

boost::math::quaternion<double>  
MspiGimbal::station_to_sc(double Gimbal_pos) const
{
  fill_in_cache();
  return m * quat_rot("y", Gimbal_pos);
}

void MspiGimbal::fill_in_cache() const
{
  if(!cache_valid) {
  // Portion of quaternion that is constant.
  // Negative sign is because these are passive rotations 
  // (the quaternion definition is active)
    m_with_der = quat_rot("zyx", 
			  -epsilon_ * Constant::deg_to_rad, 
			  -psi_ * Constant::deg_to_rad, 
			  -theta_ * Constant::deg_to_rad);
    normalize(m_with_der);
    m = value(m_with_der);
    normalize(m);
    cache_valid = true;
  }
}

// See base class for description
ArrayAd<double, 1> MspiGimbal::parameter_with_derivative() const
{
  Array<AutoDerivative<double>, 1> res(3);
  res = epsilon_, psi_, theta_;
  return ArrayAd<double, 1>(res);
}

// See base class for description
void MspiGimbal::parameter_with_derivative(const ArrayAd<double, 1>& Parm)
{
  if(Parm.rows() != 3)
    throw Exception("Parameter must have a size of exactly 3");
  epsilon_ = Parm(0);
  psi_ = Parm(1);
  theta_ = Parm(2);
  cache_valid = false;
}

// See base class for description
std::vector<std::string> MspiGimbal::parameter_name() const
{
  std::vector<std::string> res;
  res.push_back("Gimbal epsilon (degrees)");
  res.push_back("Gimbal psi (degrees)");
  res.push_back("Gimbal theta (degrees)");
  return res;
}

// See base class for description
void MspiGimbal::parameter_mask(const blitz::Array<bool, 1>& Pm) 
{
  if(Pm.rows() != parameter_mask_.rows())
    throw Exception("Parameter mask must have a size of exactly 3");
  parameter_mask_ = Pm;
}

// See base class for description
void MspiGimbal::print(std::ostream& Os) const
{
  Os << "MspiGimbal:\n"
     << "  Gimbal Epsilon: " << epsilon() << " degrees\n"
     << "  Gimbal Psi:     " << psi() << " degrees\n"
     << "  Gimbal Theta:   " << theta() << " degrees\n";
}
