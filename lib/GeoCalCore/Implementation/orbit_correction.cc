#include "orbit_correction.h"
#include "geocal_serialize_support.h"
#include "ecr.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitCorrection::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP(orb_uncorr);
}

GEOCAL_IMPLEMENT(OrbitCorrection);
#endif

//-----------------------------------------------------------------------
/// Indicate if the position correction is relative to CartesianFixed 
/// coordinates.
//-----------------------------------------------------------------------

bool OrbitCorrection::pos_corr_is_cf() const
{
  if(!pos_corr_is_cf_cache_valid_) {
    boost::shared_ptr<QuaternionOrbitData> oc_uncorr = 
      boost::dynamic_pointer_cast<QuaternionOrbitData>(orb_uncorr->orbit_data(orb_uncorr->min_time()));
    if(!oc_uncorr)
      throw Exception("OrbitCorrection only works with orbits that return a QuaternionOrbitData");
    pos_corr_is_cf_cache_ = oc_uncorr->from_cf();
    pos_corr_is_cf_cache_valid_ = true;
  }
  return pos_corr_is_cf_cache_;
}

// See base class for description
boost::shared_ptr<OrbitData> OrbitCorrection::orbit_data(Time T) const
{
  boost::shared_ptr<QuaternionOrbitData> oc_uncorr = 
    boost::dynamic_pointer_cast<QuaternionOrbitData>(orb_uncorr->orbit_data(T));
  if(!oc_uncorr)
    throw Exception("OrbitCorrection only works with orbits that return a QuaternionOrbitData");

  return boost::shared_ptr<OrbitData>
    (new QuaternionOrbitData(*oc_uncorr, pcorr(T, *oc_uncorr->position_cf()),
			     acorr(T)));
}

// See base class for description
boost::shared_ptr<OrbitData> 
OrbitCorrection::orbit_data(const TimeWithDerivative& T) const
{
  boost::shared_ptr<QuaternionOrbitData> oc_uncorr = 
    boost::dynamic_pointer_cast<QuaternionOrbitData>(orb_uncorr->orbit_data(T));
  if(!oc_uncorr)
    throw Exception("OrbitCorrection only works with orbits that return a QuaternionOrbitData");
    
  return boost::shared_ptr<OrbitData>
    (new QuaternionOrbitData(*oc_uncorr, 
		     pcorr_with_derivative(T, *oc_uncorr->position_cf()), 
		     acorr_with_derivative(T)));
}

// See base class for description.
boost::shared_ptr<CartesianInertial> 
OrbitCorrection::position_ci(Time T) const
{
  if(pos_corr_is_cf())
    return orbit_data(T)->position_ci();
  boost::shared_ptr<CartesianInertial> ci = orb_uncorr->position_ci(T);
  boost::array<double, 3> pcr = pcorr(T, Ecr(0,0,0));
  for(int j = 0; j < 3; ++j)
    ci->position[j] += pcr[j];
  return ci;
}

// See base class for description.
boost::array<AutoDerivative<double>, 3> 
OrbitCorrection::position_ci_with_derivative(
const TimeWithDerivative& T
) const
{
  if(pos_corr_is_cf())
    return orbit_data(T)->position_ci_with_derivative();
  boost::array<AutoDerivative<double>, 3> ci = 
    orb_uncorr->position_ci_with_derivative(T);  
  boost::array<AutoDerivative<double>, 3> pcr =
    pcorr_with_derivative(T, Ecr(0,0,0));
  for(int j = 0; j < 3; ++j)
    ci[j] += pcr[j];
  return ci;
}

// See base class for description.
boost::shared_ptr<CartesianFixed> 
OrbitCorrection::position_cf(Time T) const
{
  if(!pos_corr_is_cf())
    return orbit_data(T)->position_cf();
  boost::shared_ptr<CartesianFixed> cf = orb_uncorr->position_cf(T);
  boost::array<double, 3> pcr = pcorr(T, *cf);
  for(int j = 0; j < 3; ++j)
    cf->position[j] += pcr[j];
  return cf;
}

// See base class for description.
boost::array<AutoDerivative<double>, 3> 
OrbitCorrection::position_cf_with_derivative(
const TimeWithDerivative& T
) const
{
  if(!pos_corr_is_cf())
    return orbit_data(T)->position_cf_with_derivative();
  boost::array<AutoDerivative<double>, 3> cf = 
    orb_uncorr->position_cf_with_derivative(T);  
  boost::array<AutoDerivative<double>, 3> pcr =
    pcorr_with_derivative(T, *orb_uncorr->position_cf(T.value()));
  for(int j = 0; j < 3; ++j)
    cf[j] += pcr[j];
  return cf;
}

