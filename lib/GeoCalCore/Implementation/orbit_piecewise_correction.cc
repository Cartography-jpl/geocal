#include "orbit_piecewise_correction.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitPiecewiseCorrection::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(OrbitCorrection)
    & GEOCAL_NVP_(e_corr)
    & GEOCAL_NVP_(n_corr)
    & GEOCAL_NVP_(u_corr);
}

GEOCAL_IMPLEMENT(OrbitPiecewiseCorrection);
#endif

//-----------------------------------------------------------------------
/// Position correction with derivative.
//-----------------------------------------------------------------------

boost::array<AutoDerivative<double>, 3 > 
OrbitPiecewiseCorrection::pcorr_with_derivative(
const TimeWithDerivative& Tm,
const CartesianFixed& Pos_uncorr
) const
{
  boost::array<AutoDerivative<double>, 3 > pcorr;
  pcorr[0] = e_corr_.value_with_derivative(Tm);
  pcorr[1] = n_corr_.value_with_derivative(Tm);
  pcorr[2] = u_corr_.value_with_derivative(Tm);
  if(!pos_corr_is_cf())
    throw Exception("Can't use local north coordinates if underlying orbit uses CartesianInertial");
  LnLookVectorWithDerivative lv(pcorr);
  CartesianFixedLookVectorWithDerivative clv = lv.to_cf(Pos_uncorr);
  pcorr = clv.look_vector;
  return pcorr;
}

//-----------------------------------------------------------------------
/// Position correction.
//-----------------------------------------------------------------------

boost::array<double, 3 > 
OrbitPiecewiseCorrection::pcorr(
const Time& Tm,
const CartesianFixed& Pos_uncorr
) const
{
  boost::array<double, 3 > pcorr;
  pcorr[0] = e_corr_.value(Tm);
  pcorr[1] = n_corr_.value(Tm);
  pcorr[2] = u_corr_.value(Tm);
  if(!pos_corr_is_cf())
    throw Exception("Can't use local north coordinates if underlying orbit uses CartesianInertial");
  LnLookVector lv(pcorr);
  CartesianFixedLookVector clv = lv.to_cf(Pos_uncorr);
  pcorr = clv.look_vector;
  return pcorr;
}

// See base class for description
ArrayAd<double, 1> OrbitPiecewiseCorrection::parameter_with_derivative() const
{
  ArrayAd<double, 1> p1 = e_corr_.parameter_with_derivative();
  ArrayAd<double, 1> p2 = n_corr_.parameter_with_derivative();
  ArrayAd<double, 1> p3 = u_corr_.parameter_with_derivative();
  blitz::Array<AutoDerivative<double>, 1> res(p1.rows() + p2.rows() + p3.rows());
  int j = 0;
  for(int i = 0; i < p1.rows(); ++i, ++j)
    res(j) = p1(i);
  for(int i = 0; i < p2.rows(); ++i, ++j)
    res(j) = p2(i);
  for(int i = 0; i < p3.rows(); ++i, ++j)
    res(j) = p3(i);
  return res;
}

// See base class for description
void OrbitPiecewiseCorrection::parameter_with_derivative
(const ArrayAd<double, 1>& Parm)
{
  int s1 = e_corr_.parameter().rows();
  int s2 = n_corr_.parameter().rows();
  int s3 = u_corr_.parameter().rows();
  if(Parm.rows() !=  s1 + s2 + s3)
    throw Exception("Parm is not expected size");
  int i = 0;
  e_corr_.parameter_with_derivative(Parm(blitz::Range(i, i + s1-1)));
  i += s1;
  n_corr_.parameter_with_derivative(Parm(blitz::Range(i, i + s2-1)));
  i += s2;
  u_corr_.parameter_with_derivative(Parm(blitz::Range(i, i + s3-1)));
  notify_update();
}

// See base class for description
std::vector<std::string> OrbitPiecewiseCorrection::parameter_name() const
{
  int s1 = e_corr_.parameter().rows();
  int s2 = n_corr_.parameter().rows();
  int s3 = u_corr_.parameter().rows();
  std::vector<std::string> res;
  for(int i = 0; i < s1; ++i)
    res.push_back("E correction");
  for(int i = 0; i < s2; ++i)
    res.push_back("N correction");
  for(int i = 0; i < s3; ++i)
    res.push_back("U correction");
  return res;
}

// See base class for description
blitz::Array<bool, 1> OrbitPiecewiseCorrection::parameter_mask() const
{
  int s1 = e_corr_.parameter().rows();
  int s2 = n_corr_.parameter().rows();
  int s3 = u_corr_.parameter().rows();
  blitz::Array<bool, 1> res(s1 + s2 + s3);
  res = true;
  return res;
}

// See base class for description
void OrbitPiecewiseCorrection::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitPiecewiseCorrection\n"
     << "  Underlying orbit:\n";
  opad << *orbit_uncorrected() << "\n";
  opad.strict_sync();
  Os << "  E correction:\n";
  opad << e_corr() << "\n";
  opad.strict_sync();
  Os << "  N correction:\n";
  opad << n_corr() << "\n";
  opad.strict_sync();
  Os << "  U correction:\n";
  opad << u_corr() << "\n";
  opad.strict_sync();
}

