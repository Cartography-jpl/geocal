#include "camera_rational_polynomial.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include <cmath>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void CameraRationalPolyomial::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(QuaternionCamera);
  ar & GEOCAL_NVP_(kappa) & GEOCAL_NVP_(kappa_inverse);
}


GEOCAL_IMPLEMENT(CameraRationalPolyomial);
#endif

void CameraRationalPolyomial::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "CameraRationalPolyomial:\n"
     << "  Nu: " << number_sample(0) << "\n"
     << "  Nv: " << number_line(0) << "\n"
     << "  Xi: " << sample_pitch() << "\n"
     << "  U0: " << principal_point(0).sample << "\n"
     << "  V0: " << principal_point(0).line << "\n"
     << "  Pld_q_c: " << frame_to_sc() << "\n"
     << "  Kappa: " << "\n";
  opad << kappa() << "\n";
  opad.strict_sync();
  Os << "  Kappa_inverse: " << "\n";
  opad << kappa_inverse() << "\n";
  opad.strict_sync();
}

void CameraRationalPolyomial::dcs_to_focal_plane
(int Band, const boost::math::quaternion<double>& Dcs,
 double& Xfp, double& Yfp) const
{
  Array<double, 1> x(2);
  QuaternionCamera::dcs_to_focal_plane(Band, Dcs, x(0), x(1));
  Array<double, 1> x_tilde = apply_rational(x, kappa_);
  Xfp = x_tilde(0);
  Yfp = x_tilde(1);
}

void CameraRationalPolyomial::dcs_to_focal_plane
(int Band, const boost::math::quaternion<AutoDerivative<double> >& Dcs,
 AutoDerivative<double>& Xfp, AutoDerivative<double>& Yfp) const
{
  throw Exception("Don't support derivatives yet. We can add this if needed");
}

boost::math::quaternion<double> CameraRationalPolyomial::focal_plane_to_dcs
(int Band, double Xfp, double Yfp) const
{
  Array<double, 1> x(2);
  x = Xfp, Yfp;
  Array<double, 1> x_tilde = apply_rational(x, kappa_inverse_);
  return QuaternionCamera::focal_plane_to_dcs(Band, x_tilde(0), x_tilde(1));
}

boost::math::quaternion<AutoDerivative<double> >
CameraRationalPolyomial::focal_plane_to_dcs
(int Band, const AutoDerivative<double>& Xfp, 
 const AutoDerivative<double>& Yfp) const
{
  throw Exception("Don't support derivatives yet. We can add this if needed");
}

//-----------------------------------------------------------------------
/// This is the apply_rational function found in Mike Burl's matlab code
//-----------------------------------------------------------------------

blitz::Array<double, 1> CameraRationalPolyomial::apply_rational
(const blitz::Array<double, 1>& X,
 const blitz::Array<double, 2>& Coeff) const
{
  firstIndex i1; secondIndex i2;
  Range ra = Range::all();
  Array<double, 2> num(Coeff(Range(0,1), ra));
  Array<double, 2> den;
  if(Coeff.rows() == 4)
    den.reference(Array<double, 2>(Coeff(Range(2,3), ra)));
  else if(Coeff.rows() == 3) {
    den.resize(2, Coeff.cols());
    den(0,ra) = Coeff(2,ra);
    den(1,ra) = Coeff(2,ra);
  } else
    throw Exception("Coeff must have 3 or 4 rows");
  // Determine ord from number of rows in X.
  int m = -1;
  int ord;
  for(ord = 0; m != Coeff.cols() && ord <= 10; ) {
    m = 1;
    for(int k = 1; k <= X.rows(); ++k)
      m = (ord + k) * m / k;
    if(m != Coeff.cols())
      ++ord;
  }
  if(m != Coeff.cols())
    throw Exception("Could not determine order");
  Array<double, 1> chi = construct_chi_matrix(X, ord);
  Array<double, 1> top(num.rows());
  Array<double, 1> bot(den.rows());
  top = sum(chi(i2) * num, i2);
  bot = sum(chi(i2) * den, i2);
  Array<double, 1> res(top.rows());
  res = top / bot;
  return res;
}

//-----------------------------------------------------------------------
/// This is the construct_chi_matrix function found in Mike Burl's
/// matlab code
//-----------------------------------------------------------------------

blitz::Array<double, 1> CameraRationalPolyomial::construct_chi_matrix
(const blitz::Array<double, 1>& X,
 int ord) const
{
  if(X.rows() != 2)
    throw Exception("Not implemented");
  int m = 1;
  for(int k = 1; k <= X.rows(); ++k)
    m = (ord + k) * m / k;
  Array<double, 1> res(m);
  int i = 0;
  for(int tot = ord; tot >=0; --tot)
    for(int l = 0; l <= tot; ++l, ++i)
      res(i) = pow(X(0), tot-l) * pow(X(1), l);
  return res;
}

