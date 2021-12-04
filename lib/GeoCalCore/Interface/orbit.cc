#include "orbit.h"
#include "eci.h"
#include "functor.h"
#include "geocal_gsl_root.h"
#include "constant.h"
#include "geocal_serialize_support.h"
#include <blitz/array.h>
#include <cmath>

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitData::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(OrbitData);
}

template<class Archive>
void Orbit::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(Orbit);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(Orbit, WithParameter);
  ar & GEOCAL_NVP(min_tm) & GEOCAL_NVP(max_tm);
}

template<class Archive>
void KeplerOrbit::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(epoch) & GEOCAL_NVP_(a) & GEOCAL_NVP_(ap)
    & GEOCAL_NVP_(e) & GEOCAL_NVP_(freq_rev) & GEOCAL_NVP_(ma)
    & GEOCAL_NVP_(inc) & GEOCAL_NVP_(ra) & GEOCAL_NVP_(r);
}

template<class Archive>
void QuaternionOrbitData::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(OrbitData);
  GEOCAL_BASE(QuaternionOrbitData, OrbitData);
  ar & GEOCAL_NVP(tm) & GEOCAL_NVP(pos) & GEOCAL_NVP(pos_with_der)
    & GEOCAL_NVP(vel_cf) & GEOCAL_NVP(vel_cf_with_der) 
    & GEOCAL_NVP_(sc_to_cf) & GEOCAL_NVP(sc_to_cf_with_der)
    & GEOCAL_NVP_(from_cf)
    & GEOCAL_NVP(have_ci_to_cf)
    & GEOCAL_NVP_(ci_to_cf)
    & GEOCAL_NVP_(ci_to_cf_der)
    & GEOCAL_NVP(pos_ci) & GEOCAL_NVP(pos_ci_with_der) & GEOCAL_NVP(vel_ci)
    & GEOCAL_NVP(vel_ci_with_der);
  // Older version didn't have aberration_correction, default wasn
  // FULL_CORRECTION
  if(version > 0)
    ar & GEOCAL_NVP_(aberration_correction);
}

GEOCAL_IMPLEMENT(OrbitData);
GEOCAL_IMPLEMENT(QuaternionOrbitData);
GEOCAL_IMPLEMENT(KeplerOrbit);
GEOCAL_IMPLEMENT(Orbit);
#endif

//-----------------------------------------------------------------------
/// Return the footprint on the ground for the given camera and dem. 
/// This just calls surface_intersect for the four corner points.
//-----------------------------------------------------------------------

std::vector<boost::shared_ptr<GroundCoordinate> >
OrbitData::footprint(const Camera& C, const Dem& D, 
	  double Resolution,
	  int Band, double Max_height) const
{
  int ns = C.number_sample(Band) - 1;
  int nl = C.number_line(Band) - 1;
  std::vector<boost::shared_ptr<GroundCoordinate> > res;
  res.push_back(surface_intersect(C, FrameCoordinate(0, 0),
				  D, Resolution, Band, Max_height));
  res.push_back(surface_intersect(C, FrameCoordinate(0, ns),
				  D, Resolution, Band, Max_height));
  res.push_back(surface_intersect(C, FrameCoordinate(nl, ns),
				  D, Resolution, Band, Max_height));
  res.push_back(surface_intersect(C, FrameCoordinate(nl, 0),
				  D, Resolution, Band, Max_height));
  return res;
}

//-----------------------------------------------------------------------
/// Calculate the intersection with the surface. Resolution is the
/// dem postings in meters, you usually don't want this much more
/// accurate than the intrinsic accuracy of the Dem (e.g., the Did is
/// 30 meter, so resolution shouldn't be better than 30).
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed>
OrbitData::surface_intersect(const Camera& C, 
		  const FrameCoordinate& Fc,
		  const Dem& D,
	          double Resolution,
		  int Band, double Max_height) const
{
  return D.intersect(*position_cf(), cf_look_vector(C.sc_look_vector(Fc, Band)),
		     Resolution, Max_height);
}


//-----------------------------------------------------------------------
/// Calculate the approximate resolution on the ground of a given
/// Camera for this OrbitData. This finds the intersection with the
/// reference surface for the center pixel of the camera, + 1 in the
/// line and sample direction. We find the difference in meters
/// between these points, and select the maximum value.
//-----------------------------------------------------------------------

double OrbitData::resolution_meter(const Camera& C, int Band) const
{
  return resolution_meter(C, FrameCoordinate(C.number_line(Band)/ 2,
					     C.number_sample(Band)/ 2), Band);
}

//-----------------------------------------------------------------------
/// Calculate the approximate resolution on the ground of a given
/// Camera for this OrbitData. This finds the intersection with the
/// reference surface for the given pixel of the camera, + 1 in the
/// line and sample direction. We find the difference in meters
/// between these points, and select the maximum value.
//-----------------------------------------------------------------------

double OrbitData::resolution_meter(const Camera& C, const FrameCoordinate& Fc,
				   int Band) const
{
  boost::shared_ptr<GroundCoordinate> gc1 = 
    reference_surface_intersect_approximate(C, Fc, Band);
  boost::shared_ptr<GroundCoordinate> gc2 = 
    reference_surface_intersect_approximate(C, 
		    FrameCoordinate(Fc.line + 1, Fc.sample), Band);
  boost::shared_ptr<GroundCoordinate> gc3 = 
    reference_surface_intersect_approximate(C, 
		    FrameCoordinate(Fc.line, Fc.sample + 1), Band);
  double d1 = distance(*gc1, *gc2);
  double d2 = distance(*gc1, *gc3);
  return std::max(d1, d2);
}

//-----------------------------------------------------------------------
/// Give the frame coordinates that a particular point on the ground
/// is seen.
//-----------------------------------------------------------------------

FrameCoordinate OrbitData::frame_coordinate(const GroundCoordinate& Gc, 
				 const Camera& C, int Band) const
{
  CartesianFixedLookVector lv(*position_cf(), Gc);
  ScLookVector sl = sc_look_vector(lv);
  return C.frame_coordinate(sl, Band);
}

//-----------------------------------------------------------------------
/// Give the frame coordinates that a particular point on the ground
/// is seen, including derivatives wrt. the parameters of the Camera
/// and this OrbitData.
//-----------------------------------------------------------------------

FrameCoordinateWithDerivative 
OrbitData::frame_coordinate_with_derivative
(const GroundCoordinate& Gc, const Camera& C, int Band) const
{
  boost::array<AutoDerivative<double>, 3> p1 = position_cf_with_derivative();
  boost::shared_ptr<CartesianFixed> p2 = Gc.convert_to_cf();
  CartesianFixedLookVectorWithDerivative lv;
  for(int i = 0; i < 3; ++i)
    lv.look_vector[i] = p2->position[i] - p1[i];
  ScLookVectorWithDerivative sl = sc_look_vector(lv);
  return C.frame_coordinate_with_derivative(sl, Band);
}

//-----------------------------------------------------------------------
/// Return location on the reference surface that a particular frame 
/// coordinate is seen. This is approximate, in the same way 
/// CartesianFixed::reference_intersect_approximate is approximate.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed> 
OrbitData::reference_surface_intersect_approximate(const Camera& C, 
   const FrameCoordinate& Fc, int Band, double Height_reference_surface) const
{
  ScLookVector sl = C.sc_look_vector(Fc, Band);
  CartesianFixedLookVector lv = cf_look_vector(sl);
  return position_cf()->reference_surface_intersect_approximate(lv, 
         Height_reference_surface);
}

//-----------------------------------------------------------------------
/// Create a Kepler orbit with the given elements, valid over the
/// given time range. Distances are in meters and angles are in
/// degrees. The Epoch gives the Time that the rest of the data is
/// valid for. The default values are a nominal orbit for MISR.
//-----------------------------------------------------------------------

KeplerOrbit::KeplerOrbit(Time Min_time, 
			 Time Max_time,
			 Time Epoch,
			 double Semimajor_axis,
			 double Eccentricity,
			 double Inclination, 
			 double Ra_ascending_node,
			 double Ap_at_epoch,
			 double Mean_anomaly_at_epoch)
: Orbit(Min_time, Max_time),
  epoch_(Epoch),
  a_(Semimajor_axis),
  ap_(Ap_at_epoch * Constant::deg_to_rad),
  e_(Eccentricity),
  ma_(Mean_anomaly_at_epoch * Constant::deg_to_rad),
  inc_(Inclination * Constant::deg_to_rad),
  ra_(Ra_ascending_node * Constant::deg_to_rad),
  r_(3, 3)
{
  calc_freq_rev();
  calc_r();
}

//-----------------------------------------------------------------------
/// Calcluate frequency of revolution, called omega in Goldstein.
//-----------------------------------------------------------------------

void KeplerOrbit::calc_freq_rev()
{
  const double GM = 3.986032e14; 
				// G * mass earth, in m^3 / s^2. This
				// is from A E Roy "Orbital Motion"
  freq_rev_ = sqrt(GM / (a_ * a_ * a_));
}

//-----------------------------------------------------------------------
/// Calculate r_ which takes the kepler orbit and applies the correct
/// inclination and right ascension of the ascending node.
//-----------------------------------------------------------------------

void KeplerOrbit::calc_r()
{  
//-----------------------------------------------------------------------
// Rotate about x axis by inclination.
//-----------------------------------------------------------------------

  blitz::Array<double, 2> r1(3, 3);
  r1 = 1,         0,          0,
       0, cos(inc_), -sin(inc_),
       0, sin(inc_),  cos(inc_);

//-----------------------------------------------------------------------
// Then, rotate about z axis by Ra_ascending_node
//-----------------------------------------------------------------------

  blitz::Array<double, 2> r2(3, 3);
  r2 = cos(ra_), -sin(ra_), 0,
       sin(ra_),  cos(ra_), 0,
	      0,         0, 1;

//-----------------------------------------------------------------------
// Combine rotations to give r_.
//-----------------------------------------------------------------------

  blitz::firstIndex i1;
  blitz::secondIndex i2;
  blitz::thirdIndex i3;
  r_ = blitz::sum(r2(i1, i3) * r1(i3, i2), i3);
}

//-----------------------------------------------------------------------
/// Kepler's equation, see Goldstein "Classical Mechanics 2nd Edition"
/// for details.
//-----------------------------------------------------------------------

class KeplerEquation: public DFunctorWithDerivative {
public:
  KeplerEquation(const AutoDerivative<double>& wt, 
		 const AutoDerivative<double>& e) 
    : e_(e) 
  {
    wt_ = AutoDerivative<double>(fmod(wt.value(), 2 * Constant::pi), 
				 wt.gradient());
    if(wt_ < 0)
      wt_ += 2 * Constant::pi;
  }
  virtual ~KeplerEquation() {}
  virtual double operator()(const double& psi) const
  { return wt_.value() - (psi - e_.value() * sin(psi));}
  virtual double df(double psi) const
  { return -(1.0 - e_.value() * cos(psi));}
  virtual AutoDerivative<double> f_with_derivative(double psi) const
  { return wt_ - (psi - e_ * sin(psi)); }
private:
  AutoDerivative<double> wt_;
  AutoDerivative<double> e_;
};

//-----------------------------------------------------------------------
/// Calculate OrbitData for KeplerOrbit.
//-----------------------------------------------------------------------

template<class T> inline T dotb(const blitz::Array<T, 1>& x, 
				const blitz::Array<T, 1>& y)
{
  return sum(x * y);
}

template<class T> inline T normb(const blitz::Array<T, 1>& x)
{
  return std::sqrt(dotb(x, x));
}

template<class T> inline blitz::Array<T, 1> cross2
(const blitz::Array<T, 1>& x,
 const blitz::Array<T, 1>& y)
{
  blitz::Array<T, 1> res(3);
  res(0) = x(1) * y(2) - x(2) * y(1);
  res(1) = x(2) * y(0) - x(0) * y(2);
  res(2) = x(0) * y(1) - x(1) * y(0);
  return res;
}

boost::shared_ptr<OrbitData> KeplerOrbit::orbit_data(Time T) const
{
  range_check_inclusive(T, min_time(), max_time());

//-----------------------------------------------------------------------
// Calculate r, theta, and their derivatives using Kepler's equations.
//-----------------------------------------------------------------------

  double eccentric_anomaly = 
    root(KeplerEquation(freq_rev_ * (T - epoch_) + ma_, e_), 0, 
	 2 * Constant::pi);

  double spsi = sin(eccentric_anomaly);
  double cpsi = cos(eccentric_anomaly);

// This is in the range -pi to pi
  double theta = 2 * atan(sqrt((1 + e_) / (1 - e_)) * 
			  tan(eccentric_anomaly / 2));
  double r = a_ * (1 - e_ * cpsi);
  double psidot = freq_rev_ / (1 - e_ * cpsi);
  double rdot = a_ * e_ * spsi * psidot;
  double thetadot = spsi * (1 - e_ * e_) / 
    (sin(theta) * (1 - e_ * cpsi) * (1 - e_ * cpsi)) * psidot;

//-----------------------------------------------------------------------
// Get Cartesian coordinates of point & velocity
//-----------------------------------------------------------------------

  blitz::Array<double, 1> pos(3), vel(3);
  double ctheta = cos(theta + ap_);
  double stheta = sin(theta + ap_);
  pos(0) = r * ctheta;
  pos(1) = r * stheta;
  pos(2) = 0;
  vel(0) = rdot * ctheta - r * stheta * thetadot;
  vel(1) = rdot * stheta + r * ctheta * thetadot;
  vel(2) = 0;

//-----------------------------------------------------------------------
// Rotate to proper inclination and longitude of ascending node.
//-----------------------------------------------------------------------
  
  blitz::firstIndex i1;
  blitz::secondIndex i2;
  Array<double, 1> p(3), v(3);
  p = sum(r_(i1, i2) * pos(i2), i2);
  v = sum(r_(i1, i2) * vel(i2), i2);

//-----------------------------------------------------------------------
// Create matrix.
//-----------------------------------------------------------------------

  Array<double, 1> x(3), y(3), z(3);
  z = -p / normb(p);
  x = v - z * dotb(v, z);
  x /= normb(x);
  y = cross2(z, x);
  double sc_to_ci[3][3] = {{x(0), y(0), z(0)},
			   {x(1), y(1), z(1)},
			   {x(2), y(2), z(2)}};
  boost::shared_ptr<CartesianInertial> pci(new Eci(p(0), p(1), p(2)));
  boost::array<double, 3> v2 = {{v(0), v(1), v(2)}};
  return boost::shared_ptr<OrbitData>(new QuaternionOrbitData(T, pci, v2, 
		      matrix_to_quaternion(sc_to_ci)));
}

boost::shared_ptr<OrbitData> KeplerOrbit::orbit_data
(const TimeWithDerivative& T) const
{
  range_check_inclusive(T.value(), min_time(), max_time());

//-----------------------------------------------------------------------
// Calculate r, theta, and their derivatives using Kepler's equations.
//-----------------------------------------------------------------------

  AutoDerivative<double> eccentric_anomaly = root_with_derivative
    (KeplerEquation(freq_rev_ * (T - epoch_) + ma_, e_), 0, 2 * Constant::pi);

  AutoDerivative<double> spsi = std::sin(eccentric_anomaly);
  AutoDerivative<double> cpsi = std::cos(eccentric_anomaly);

// This is in the range -pi to pi
  AutoDerivative<double> theta = 
    2 * std::atan(sqrt((1 + e_) / (1 - e_)) * std::tan(eccentric_anomaly / 2));
  AutoDerivative<double> r = a_ * (1 - e_ * cpsi);
  AutoDerivative<double> psidot = freq_rev_ / (1 - e_ * cpsi);
  AutoDerivative<double> rdot = a_ * e_ * spsi * psidot;
  AutoDerivative<double> thetadot = spsi * (1 - e_ * e_) / 
    (std::sin(theta) * (1 - e_ * cpsi) * (1 - e_ * cpsi)) * psidot;

//-----------------------------------------------------------------------
// Get Cartesian coordinates of point & velocity
//-----------------------------------------------------------------------

  blitz::Array<AutoDerivative<double>, 1> pos(3), vel(3);
  AutoDerivative<double> ctheta = std::cos(theta + ap_);
  AutoDerivative<double> stheta = std::sin(theta + ap_);
  pos(0) = r * ctheta;
  pos(1) = r * stheta;
  pos(2) = 0;
  vel(0) = rdot * ctheta - r * stheta * thetadot;
  vel(1) = rdot * stheta + r * ctheta * thetadot;
  vel(2) = 0;

//-----------------------------------------------------------------------
// Rotate to proper inclination and longitude of ascending node.
//-----------------------------------------------------------------------
  
  blitz::firstIndex i1;
  blitz::secondIndex i2;
  Array<AutoDerivative<double>, 1> p(3), v(3);
  p = sum(r_(i1, i2) * pos(i2), i2);
  v = sum(r_(i1, i2) * vel(i2), i2);

//-----------------------------------------------------------------------
// Create matrix.
//-----------------------------------------------------------------------

  Array<AutoDerivative<double>, 1> x(3), y(3), z(3);
  z = -p / normb(p);
  x = v - z * dotb(v, z);
  x /= normb(x);
  y = cross2(z, x);
  AutoDerivative<double> sc_to_ci[3][3] = {{x(0), y(0), z(0)},
					   {x(1), y(1), z(1)},
					   {x(2), y(2), z(2)}};
  boost::shared_ptr<CartesianInertial> 
    pci(new Eci(p(0).value(), p(1).value(), p(2).value())) ;
  boost::array<AutoDerivative<double>, 3> pci_der = {{p(0), p(1), p(2)}};
  boost::array<AutoDerivative<double>, 3> v2 = {{v(0), v(1), v(2)}};
  return boost::shared_ptr<OrbitData>(new QuaternionOrbitData(T, pci, pci_der,
		      v2, 
   		      matrix_to_quaternion(sc_to_ci)));
}

//-----------------------------------------------------------------------
/// Print out description of Orbit.
//-----------------------------------------------------------------------

void KeplerOrbit::print(std::ostream& Os) const
{
  Os << "Kepler orbit:\n"
     << "  Time Range:            " << min_time().to_string() << " to " 
     << max_time().to_string() << "\n"
     << "  Epoch of data:         " << epoch_.to_string() << "\n"
     << "  Semimajor axis:        " << a_  << " m\n"
     << "  Eccentricity:          " << e_ << "\n"
     << "  Inclination:           " << inc_ * Constant::rad_to_deg << " deg\n"
     << "  Right ascension of ascending node:\n" 
     << "                         " << ra_ * Constant::rad_to_deg << " deg\n"
     << "  Argument of perigee at epoch:\n"
     << "                         " << ap_ * Constant::rad_to_deg << " deg\n"
     << "  Mean anomoly at epoch: " << ma_  * Constant::rad_to_deg << " deg\n";
}

//-----------------------------------------------------------------------
/// Constructor.
/// This make an perturbation to an existing QuaternionOrbitData. We
/// take an offset to add to the position, and a rotation matrix that
/// takes us from the original sc coordinate system to the "corrected"
/// coordinate system, e.g., small correction to the yaw, roll and
/// pitch of the original quaternion.
///
/// The position correction is in CartesianFixed if from_cf() is true,
/// otherwise it is CartesianInertial.
//-----------------------------------------------------------------------

QuaternionOrbitData::QuaternionOrbitData
(const QuaternionOrbitData& Start,
 const boost::array<AutoDerivative<double>, 3>& Pos_off,
 const boost::math::quaternion<AutoDerivative<double> >& Sc_to_sc_corr)
{
  tm = Start.tm;
  pos = Start.pos->create(Start.pos->position);
  pos_with_der = Start.pos_with_der;
  vel_cf = Start.vel_cf;
  vel_cf_with_der = Start.vel_cf_with_der;
  sc_to_cf_with_der = Start.sc_to_cf_with_der * conj(Sc_to_sc_corr);
  normalize(sc_to_cf_with_der);
  sc_to_cf_ = value(sc_to_cf_with_der);
  from_cf_ = Start.from_cf_;
  aberration_correction_ = Start.aberration_correction_;
  have_ci_to_cf = Start.have_ci_to_cf;
  if(Start.have_ci_to_cf) {
    ci_to_cf_ = Start.ci_to_cf_;
    ci_to_cf_der_ = Start.ci_to_cf_der_;
    pos_ci = Start.pos_ci->create(Start.pos_ci->position);
    pos_ci_with_der = Start.pos_ci_with_der;
    vel_ci = Start.vel_ci;
    vel_ci_with_der = Start.vel_ci_with_der;
  }
  if(from_cf_) {
    pos->position[0] += Pos_off[0].value();
    pos->position[1] += Pos_off[1].value();
    pos->position[2] += Pos_off[2].value();
    pos_with_der = boost::math::quaternion<AutoDerivative<double> >
      (0,pos_with_der.R_component_2() + Pos_off[0],
       pos_with_der.R_component_3() + Pos_off[1],
       pos_with_der.R_component_4() + Pos_off[2]);
    if(have_ci_to_cf) {
      pos_ci_with_der = conj(ci_to_cf_with_derivative()) * pos_with_der * 
	ci_to_cf_with_derivative();
      boost::array<double, 3> p = {{pos_ci_with_der.R_component_2().value(),
				    pos_ci_with_der.R_component_3().value(),
				    pos_ci_with_der.R_component_4().value()}};
      pos_ci = pos_ci->create(p);
    }
  } else {
    pos_ci->position[0] += Pos_off[0].value();
    pos_ci->position[1] += Pos_off[1].value();
    pos_ci->position[2] += Pos_off[2].value();
    pos_ci_with_der = boost::math::quaternion<AutoDerivative<double> >
      (0,pos_ci_with_der.R_component_2() + Pos_off[0],
       pos_ci_with_der.R_component_3() + Pos_off[1],
       pos_ci_with_der.R_component_4() + Pos_off[2]);
    pos_with_der = ci_to_cf_with_derivative() * pos_ci_with_der * 
      conj(ci_to_cf_with_derivative());
    boost::array<double, 3> p = {{pos_with_der.R_component_2().value(),
				  pos_with_der.R_component_3().value(),
				  pos_with_der.R_component_4().value()}};
    pos = pos->create(p);
  }
}

//-----------------------------------------------------------------------
/// Copy constructor.
//-----------------------------------------------------------------------

QuaternionOrbitData::QuaternionOrbitData
(const QuaternionOrbitData& V)
{
  tm = V.tm;
  pos = V.pos->create(V.pos->position);
  pos_with_der = V.pos_with_der;
  vel_cf = V.vel_cf;
  vel_cf_with_der = V.vel_cf_with_der;
  sc_to_cf_with_der = V.sc_to_cf_with_der;
  sc_to_cf_ = value(sc_to_cf_with_der);
  from_cf_ = V.from_cf_;
  aberration_correction_ = V.aberration_correction_;
  have_ci_to_cf = V.have_ci_to_cf;
  if(V.have_ci_to_cf) {
    ci_to_cf_ = V.ci_to_cf_;
    ci_to_cf_der_ =V.ci_to_cf_der_;
    pos_ci = V.pos_ci->create(V.pos_ci->position);
    pos_ci_with_der = V.pos_ci_with_der;
    vel_ci = V.vel_ci;
    vel_ci_with_der = V.vel_ci_with_der;
  }
}

//-----------------------------------------------------------------------
/// Constructor.
/// This make an perturbation to an existing QuaternionOrbitData. We
/// take an offset to add to the position, and a rotation matrix that
/// takes us from the original sc coordinate system to the "corrected"
/// coordinate system, e.g., small correction to the yaw, roll and
/// pitch of the original quaternion.
///
/// The position correction is in CartesianFixed if from_cf() is true,
/// otherwise it is CartesianInertial.
///
/// This version does not include the derivative calculation.
//-----------------------------------------------------------------------

QuaternionOrbitData::QuaternionOrbitData
(const QuaternionOrbitData& Start,
 const boost::array<double, 3>& Pos_off,
 const boost::math::quaternion<double>& Sc_to_sc_corr)
{
  tm = Start.tm;
  pos = Start.pos->create(Start.pos->position);
  vel_cf = Start.vel_cf;
  vel_cf_with_der = vel_cf;
  sc_to_cf_ = Start.sc_to_cf_ * conj(Sc_to_sc_corr);
  normalize(sc_to_cf_);
  sc_to_cf_with_der = sc_to_cf_;
  from_cf_ = Start.from_cf_;
  aberration_correction_ = Start.aberration_correction_;
  have_ci_to_cf = Start.have_ci_to_cf;
  if(have_ci_to_cf) {
    ci_to_cf_ = Start.ci_to_cf_;
    ci_to_cf_der_ = ci_to_cf_;
    vel_ci = Start.vel_ci;
    vel_ci_with_der = vel_ci;
  }
  if(from_cf_) {
    pos->position[0] += Pos_off[0];
    pos->position[1] += Pos_off[1];
    pos->position[2] += Pos_off[2];
    pos_with_der = 
      boost::math::quaternion<double>(0, pos->position[0], 
				      pos->position[1], pos->position[2]);
    if(have_ci_to_cf) {
      pos_ci_with_der = conj(ci_to_cf()) * pos_with_der * 
	ci_to_cf();
      boost::array<double, 3> p = {{pos_ci_with_der.R_component_2().value(),
				    pos_ci_with_der.R_component_3().value(),
				    pos_ci_with_der.R_component_4().value()}};
      pos_ci = pos_ci->create(p);
    }
  } else {
    pos_ci = Start.pos_ci->create(Start.pos_ci->position);
    pos_ci->position[0] += Pos_off[0];
    pos_ci->position[1] += Pos_off[1];
    pos_ci->position[2] += Pos_off[2];
    pos_ci_with_der = boost::math::quaternion<AutoDerivative<double> >
      (0,pos_ci->position[0],pos_ci->position[1],pos_ci->position[2]);
    pos_with_der = ci_to_cf() * pos_ci_with_der * 
      conj(ci_to_cf());
    boost::array<double, 3> p = {{pos_with_der.R_component_2().value(),
				  pos_with_der.R_component_3().value(),
				  pos_with_der.R_component_4().value()}};
    pos = pos->create(p);
  }
}

//-----------------------------------------------------------------------
/// Construct QuaternionOrbitData. This takes data in a CartesianFixed
/// coordinate system (e.g., Ecr coordinates).
//-----------------------------------------------------------------------

QuaternionOrbitData::QuaternionOrbitData(Time Tm, 
   const boost::shared_ptr<CartesianFixed>& pos_cf,
   const boost::array<double, 3>& vel_fixed,
   const boost::math::quaternion<double>& sc_to_cf_q)
  : tm(Tm), pos(pos_cf), 
    pos_with_der(0, pos_cf->position[0], 
		 pos_cf->position[1], pos_cf->position[2]),
    vel_cf(0, vel_fixed[0], vel_fixed[1], vel_fixed[2]), 
    vel_cf_with_der(0, vel_fixed[0], vel_fixed[1], vel_fixed[2]), 
    sc_to_cf_(sc_to_cf_q), 
    sc_to_cf_with_der(sc_to_cf_q), 
    from_cf_(true), aberration_correction_(FULL_CORRECTION),
    have_ci_to_cf(false)
{ 
  normalize(sc_to_cf_);
  normalize(sc_to_cf_with_der);
}

//-----------------------------------------------------------------------
/// Construct QuaternionOrbitData. This takes data in a CartesianFixed
/// coordinate system (e.g., Ecr coordinates).
//-----------------------------------------------------------------------

QuaternionOrbitData::QuaternionOrbitData
(const TimeWithDerivative& Tm, 
 const boost::shared_ptr<CartesianFixed>& pos_cf,
 const boost::array<AutoDerivative<double>, 3>& pos_cf_with_der,
 const boost::array<AutoDerivative<double>, 3>& vel_fixed,
 const boost::math::quaternion<AutoDerivative<double> >& sc_to_cf_q
)
: tm(Tm), pos(pos_cf), 
  pos_with_der(0, pos_cf_with_der[0], pos_cf_with_der[1], pos_cf_with_der[2]),
  vel_cf(0, vel_fixed[0].value(), vel_fixed[1].value(), vel_fixed[2].value()), 
  vel_cf_with_der(0, vel_fixed[0], vel_fixed[1], vel_fixed[2]), 
  sc_to_cf_(value(sc_to_cf_q)), 
  sc_to_cf_with_der(sc_to_cf_q), 
  from_cf_(true), aberration_correction_(FULL_CORRECTION),
  have_ci_to_cf(false)
{ 
  normalize(sc_to_cf_);
  normalize(sc_to_cf_with_der);
}

//-----------------------------------------------------------------------
/// Initialize QuaternionOrbitData. This takes data in a CartesianFixed
/// coordinate system (e.g., Ecr coordinates).
//-----------------------------------------------------------------------

void QuaternionOrbitData::initialize(Time Tm, 
   const boost::shared_ptr<CartesianFixed>& pos_cf,
   const boost::array<double, 3>& vel_fixed,
   const boost::math::quaternion<double>& sc_to_cf_q)
{ 
  from_cf_ = true;
  tm = Tm;
  pos = pos_cf;
  pos_with_der = 
    boost::math::quaternion<double>(0, pos_cf->position[0], 
				    pos_cf->position[1], pos_cf->position[2]);
  vel_cf = boost::math::quaternion<double>(0, vel_fixed[0], vel_fixed[1], 
					   vel_fixed[2]);
  vel_cf_with_der = vel_cf;
  sc_to_cf_ = sc_to_cf_q;
  sc_to_cf_with_der = sc_to_cf_q;
  normalize(sc_to_cf_);
  normalize(sc_to_cf_with_der);
  have_ci_to_cf = false;
}

//-----------------------------------------------------------------------
/// Initialize QuaternionOrbitData. This takes data in a CartesianFixed
/// coordinate system (e.g., Ecr coordinates).
//-----------------------------------------------------------------------

void QuaternionOrbitData::initialize
(const TimeWithDerivative& Tm, 
 const boost::shared_ptr<CartesianFixed>& pos_cf,
 const boost::array<AutoDerivative<double>, 3>& pos_cf_with_der,
 const boost::array<AutoDerivative<double>, 3>& vel_fixed, const 
 boost::math::quaternion<AutoDerivative<double> >& sc_to_cf_q)
{ 
  from_cf_ = true;
  tm = Tm;
  pos = pos_cf;
  pos_with_der = boost::math::quaternion<AutoDerivative<double> >
    (0, pos_cf_with_der[0], pos_cf_with_der[1], pos_cf_with_der[2]);
  vel_cf_with_der = boost::math::quaternion<AutoDerivative<double> >
    (0, vel_fixed[0], vel_fixed[1], vel_fixed[2]);
  vel_cf = value(vel_cf_with_der);
  sc_to_cf_ = value(sc_to_cf_q);
  sc_to_cf_with_der = sc_to_cf_q;
  normalize(sc_to_cf_);
  normalize(sc_to_cf_with_der);
  have_ci_to_cf = false;
}

//-----------------------------------------------------------------------
/// Construct QuaternionOrbitData. This takes data in a CartesianInertial
/// coordinate system (e.g., Eci coordinates).
//-----------------------------------------------------------------------

QuaternionOrbitData::QuaternionOrbitData(Time Tm, 
   const boost::shared_ptr<CartesianInertial>& pos_ci,
   const boost::array<double, 3>& vel_inertial,
   const boost::math::quaternion<double>& sc_to_ci_q)
: aberration_correction_(FULL_CORRECTION),
  have_ci_to_cf(false)
{ 
  initialize(Tm, pos_ci, vel_inertial, sc_to_ci_q);
}

//-----------------------------------------------------------------------
/// Construct QuaternionOrbitData. This takes data in a CartesianInertial
/// coordinate system (e.g., Eci coordinates).
//-----------------------------------------------------------------------

QuaternionOrbitData::QuaternionOrbitData
(const TimeWithDerivative& Tm, 
 const boost::shared_ptr<CartesianInertial>& pos_ci,
 const boost::array<AutoDerivative<double>, 3>& pos_ci_with_der,
 const boost::array<AutoDerivative<double>, 3>& vel_inertial,
 const boost::math::quaternion<AutoDerivative<double> >& sc_to_ci_q
)
: aberration_correction_(FULL_CORRECTION),
  have_ci_to_cf(false)
{ 
  initialize(Tm, pos_ci, pos_ci_with_der, vel_inertial, sc_to_ci_q);
}

//-----------------------------------------------------------------------
/// Initialize QuaternionOrbitData. This takes data in a CartesianInertial
/// coordinate system (e.g., Eci coordinates).
//-----------------------------------------------------------------------

void QuaternionOrbitData::initialize(Time Tm, 
   const boost::shared_ptr<CartesianInertial>& Pos_ci,
   const boost::array<double, 3>& vel_inertial,
   const boost::math::quaternion<double>& sc_to_ci_q)
{ 
  from_cf_ = false;
  tm = Tm;
  pos_ci = Pos_ci;
  pos_ci_with_der = boost::math::quaternion<double>(0, pos_ci->position[0],
						    pos_ci->position[1],
						    pos_ci->position[2]);
  vel_ci = boost::math::quaternion<double>(0, vel_inertial[0], vel_inertial[1],
					   vel_inertial[2]);
  vel_ci_with_der = vel_ci;
  boost::array<double, 3> vcf;
  convert_position_and_velocity(Tm, *Pos_ci, vel_inertial, pos, vcf, ci_to_cf_);
  ci_to_cf_der_ = ci_to_cf_;
  have_ci_to_cf = true;
  vel_cf = boost::math::quaternion<double>(0, vcf[0], vcf[1], vcf[2]);
  vel_cf_with_der = vel_cf;
  pos_with_der = boost::math::quaternion<double>(0, pos->position[0], 
						 pos->position[1],
						 pos->position[2]);
  sc_to_cf_ = ci_to_cf() * sc_to_ci_q;
  sc_to_cf_with_der = sc_to_cf_;
  normalize(sc_to_cf_);
  normalize(sc_to_cf_with_der);
}

//-----------------------------------------------------------------------
/// Set sc_to_ci
//-----------------------------------------------------------------------

void QuaternionOrbitData::sc_to_ci(const boost::math::quaternion<double>& sc_to_ci_q)
{
  sc_to_cf_ = ci_to_cf() * sc_to_ci_q;
  sc_to_cf_with_der = sc_to_cf_;
  normalize(sc_to_cf_);
  normalize(sc_to_cf_with_der);
}

//-----------------------------------------------------------------------
/// Set sc_to_ci_with_derivative
//-----------------------------------------------------------------------

void QuaternionOrbitData::sc_to_ci_with_derivative
(const boost::math::quaternion<AutoDerivative<double> >& sc_to_ci_q)
{
  sc_to_cf_with_der = ci_to_cf_with_derivative() * sc_to_ci_q;
  sc_to_cf_ = value(sc_to_cf_with_der);
  normalize(sc_to_cf_);
  normalize(sc_to_cf_with_der);
}

//-----------------------------------------------------------------------
/// Initialize QuaternionOrbitData. This takes data in a CartesianInertial
/// coordinate system (e.g., Eci coordinates).
//-----------------------------------------------------------------------

void QuaternionOrbitData::initialize
(const TimeWithDerivative& Tm, 
 const boost::shared_ptr<CartesianInertial>& Pos_ci,
 const boost::array<AutoDerivative<double>, 3>& Pos_ci_with_der,
 const boost::array<AutoDerivative<double>, 3>& vel_inertial,
 const boost::math::quaternion<AutoDerivative<double> >& sc_to_ci_q)
{ 
  from_cf_ = false;
  tm = Tm;
  pos_ci = Pos_ci;
  pos_ci_with_der = boost::math::quaternion<AutoDerivative<double> >
    (0, Pos_ci_with_der[0], Pos_ci_with_der[1], Pos_ci_with_der[2]);
  vel_ci_with_der = boost::math::quaternion<AutoDerivative<double> >
    (0, vel_inertial[0], vel_inertial[1], vel_inertial[2]);
  vel_ci = value(vel_ci_with_der);

  boost::array<AutoDerivative<double>, 3> pder, vcf;
  convert_position_and_velocity(tm, *Pos_ci, Pos_ci_with_der, vel_inertial, 
				pos, pder, vcf, ci_to_cf_der_);
  ci_to_cf_ = value(ci_to_cf_der_);
  vel_cf_with_der = boost::math::quaternion<AutoDerivative<double> >
    (0, vcf[0], vcf[1], vcf[2]);
  vel_cf = value(vel_cf_with_der);
  pos_with_der = boost::math::quaternion<AutoDerivative<double> >
    (0, pder[0], pder[1], pder[2]);
  sc_to_cf_with_der = ci_to_cf_with_derivative() * sc_to_ci_q;
  sc_to_cf_ = value(sc_to_cf_with_der);
  have_ci_to_cf = true;
  normalize(sc_to_cf_);
  normalize(sc_to_cf_with_der);
}

//-----------------------------------------------------------------------
/// Convert to CartesianInertialLookVector.
//-----------------------------------------------------------------------

CartesianInertialLookVector 
QuaternionOrbitData::ci_look_vector(const ScLookVector& Sl) const
{
  CartesianInertialLookVector res;
  fill_in_ci_to_cf();
  boost::math::quaternion<double> ci = 
    conj(ci_to_cf()) * sc_to_cf_ * Sl.look_quaternion() * conj(sc_to_cf_) * ci_to_cf();
  if(aberration_correction_ != NO_CORRECTION) {
    // Do aberration of light correction.
    double k = Sl.length() / Constant::speed_of_light;
    ci -= k * vel_ci;
  }
  res.look_quaternion(ci);
  return res;
}

//-----------------------------------------------------------------------
/// Convert to CartesianInertialLookVector.
//-----------------------------------------------------------------------

CartesianInertialLookVectorWithDerivative 
QuaternionOrbitData::ci_look_vector(const ScLookVectorWithDerivative& Sl) const
{
  CartesianInertialLookVectorWithDerivative res;
  fill_in_ci_to_cf();
  boost::math::quaternion<AutoDerivative<double> > ci = 
    conj(ci_to_cf_with_derivative()) * 
    sc_to_cf_with_der * Sl.look_quaternion() * conj(sc_to_cf_with_der)
    * ci_to_cf_with_derivative();
  if(aberration_correction_ != NO_CORRECTION) {
    // Do aberration of light correction.
    AutoDerivative<double> k = Sl.length() / Constant::speed_of_light;
    ci -= k * vel_ci_with_der;
  }
  res.look_quaternion(ci);
  return res;
}

//-----------------------------------------------------------------------
/// Convert to CartesianFixedLookVector
//-----------------------------------------------------------------------

CartesianFixedLookVector
QuaternionOrbitData::cf_look_vector(const ScLookVector& Sl) const
{
  CartesianFixedLookVector res;
  boost::math::quaternion<double> cf = sc_to_cf_ * Sl.look_quaternion() * 
    conj(sc_to_cf_);
  if(aberration_correction_ == IGNORE_PLANET_ROTATION_FOR_CARTESIAN_FIXED) {
    // Do aberration of light correction, ignoring planet rotation
    double k = Sl.length() / Constant::speed_of_light;
    cf -= k * vel_cf;
  } else if(aberration_correction_ == FULL_CORRECTION) {
    // Do aberration of light correction
    fill_in_ci_to_cf();
    double k = Sl.length() / Constant::speed_of_light;
    cf -= k * ci_to_cf() * vel_ci * conj(ci_to_cf());
  } else if (aberration_correction_ != NO_CORRECTION) {
    throw Exception("Unknown aberration_correction_ type");
  }
  
  res.look_quaternion(cf);
  return res;
}

//-----------------------------------------------------------------------
/// Convert to CartesianFixedLookVector
//-----------------------------------------------------------------------

CartesianFixedLookVectorWithDerivative
QuaternionOrbitData::cf_look_vector(const ScLookVectorWithDerivative& Sl) const
{
  CartesianFixedLookVectorWithDerivative res;
  boost::math::quaternion<AutoDerivative<double> > cf = 
    sc_to_cf_with_der * Sl.look_quaternion() * 
    conj(sc_to_cf_with_der);
  if(aberration_correction_ != IGNORE_PLANET_ROTATION_FOR_CARTESIAN_FIXED) {
    // Do aberration of light correction, ignoring planet rotation
    AutoDerivative<double> k = Sl.length() / Constant::speed_of_light;
    cf -= k * vel_cf_with_der;
  } else if(aberration_correction_ != FULL_CORRECTION) {
    // Do aberration of light correction, ignoring planet rotation
    fill_in_ci_to_cf();
    AutoDerivative<double> k = Sl.length() / Constant::speed_of_light;
    cf -= k * ci_to_cf_with_derivative() * vel_ci_with_der *
      conj(ci_to_cf_with_derivative());
  } else if(aberration_correction_ != NO_CORRECTION) {
    throw Exception("Unknown aberration_correction_ type");
  }
  res.look_quaternion(cf);
  return res;
}

//-----------------------------------------------------------------------
/// Convert to ScLookVector.
//-----------------------------------------------------------------------

ScLookVector 
QuaternionOrbitData::sc_look_vector(const CartesianInertialLookVector& Ci) 
const
{
  ScLookVector res;
  boost::math::quaternion<double> sc;
  if(aberration_correction_ == NO_CORRECTION) {
    sc = conj(sc_to_cf_) *
      (ci_to_cf() * Ci.look_quaternion() * conj(ci_to_cf())) * sc_to_cf_;
  } else {
    // Do aberration of light correction.
    fill_in_ci_to_cf();
    double k = Ci.length() / Constant::speed_of_light;
    sc = conj(sc_to_cf_) *
      (ci_to_cf() * (Ci.look_quaternion() + k * vel_ci) * conj(ci_to_cf())) *
      sc_to_cf_;
  }
  res.look_quaternion(sc);
  return res;
}

//-----------------------------------------------------------------------
/// Convert to ScLookVector.
//-----------------------------------------------------------------------

ScLookVectorWithDerivative 
QuaternionOrbitData::sc_look_vector(const CartesianInertialLookVectorWithDerivative& Ci) 
const
{
  ScLookVectorWithDerivative res;
  boost::math::quaternion<AutoDerivative<double> > sc;
  if(aberration_correction_ == NO_CORRECTION) {
    sc = conj(sc_to_cf_with_der) * 
      (ci_to_cf_with_derivative() * Ci.look_quaternion() * conj(ci_to_cf_with_derivative())) * sc_to_cf_with_der;
  } else {
    // Do aberration of light correction.
    fill_in_ci_to_cf();
    AutoDerivative<double> k = Ci.length() / Constant::speed_of_light;
    sc = conj(sc_to_cf_with_der) * 
      (ci_to_cf_with_derivative() * (Ci.look_quaternion() + k * vel_ci_with_der) * conj(ci_to_cf_with_derivative())) * sc_to_cf_with_der;
  }
  
  res.look_quaternion(sc);
  return res;
}

//-----------------------------------------------------------------------
/// Convert to ScLookVector.
//-----------------------------------------------------------------------

ScLookVector 
QuaternionOrbitData::sc_look_vector(const CartesianFixedLookVector& Cf) 
const
{
  ScLookVector res;
  boost::math::quaternion<double> sc;
  if(aberration_correction_ == NO_CORRECTION) {
    sc = conj(sc_to_cf_) * (Cf.look_quaternion()) * sc_to_cf_;
  } else if(aberration_correction_ == IGNORE_PLANET_ROTATION_FOR_CARTESIAN_FIXED) {
    // Do aberration of light correction, ignoring planet rotation
    double k = Cf.length() / Constant::speed_of_light;
    sc = conj(sc_to_cf_) * (Cf.look_quaternion() + k * vel_cf) * sc_to_cf_;
  } else if(aberration_correction_ == FULL_CORRECTION) {
    // Do aberration of light correction
    fill_in_ci_to_cf();
    double k = Cf.length() / Constant::speed_of_light;
    sc = conj(sc_to_cf_) * (Cf.look_quaternion() +
			    k * ci_to_cf() * vel_ci * conj(ci_to_cf())) *
      sc_to_cf_;
  } else {
    throw Exception("Unknown aberration_correction_ type");
  }    
  res.look_quaternion(sc);
  return res;
}

//-----------------------------------------------------------------------
/// Convert to ScLookVector.
//-----------------------------------------------------------------------

ScLookVectorWithDerivative 
QuaternionOrbitData::sc_look_vector(const CartesianFixedLookVectorWithDerivative& Cf) 
const
{
  ScLookVectorWithDerivative res;
  boost::math::quaternion<AutoDerivative<double> > sc;
  if(aberration_correction_ == NO_CORRECTION) {
    sc = conj(sc_to_cf_with_der) * (Cf.look_quaternion()) * sc_to_cf_with_der;
  } else if(aberration_correction_ == IGNORE_PLANET_ROTATION_FOR_CARTESIAN_FIXED) {
    // Do aberration of light correction, ignoring planet rotation
    AutoDerivative<double> k = Cf.length() / Constant::speed_of_light;
    sc = conj(sc_to_cf_with_der) *
      (Cf.look_quaternion() + k * vel_cf_with_der) * sc_to_cf_with_der;
  } else if(aberration_correction_ == FULL_CORRECTION) {
    // Do aberration of light correction
    fill_in_ci_to_cf();
    AutoDerivative<double> k = Cf.length() / Constant::speed_of_light;
    sc = conj(sc_to_cf_with_der) *
      (Cf.look_quaternion() + k *
       ci_to_cf_with_derivative() * vel_ci_with_der *
       conj(ci_to_cf_with_derivative())) * sc_to_cf_with_der;
  } else {
    throw Exception("Unknown aberration_correction_ type");
  }    
  res.look_quaternion(sc);
  return res;
}

//-----------------------------------------------------------------------
/// Return velocity.
//-----------------------------------------------------------------------

boost::array<double, 3> QuaternionOrbitData::velocity_ci() const
{
  fill_in_ci_to_cf();
  boost::array<double, 3> res = {{vel_ci.R_component_2(), 
				  vel_ci.R_component_3(),
				  vel_ci.R_component_4()}};
  return res;
}

//-----------------------------------------------------------------------
/// Return velocity.
//-----------------------------------------------------------------------

boost::array<AutoDerivative<double>, 3> 
QuaternionOrbitData::velocity_ci_with_derivative() const
{
  fill_in_ci_to_cf();
  boost::array<AutoDerivative<double>, 3> res = 
    {{vel_ci_with_der.R_component_2(), 
      vel_ci_with_der.R_component_3(),
      vel_ci_with_der.R_component_4()}};
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void QuaternionOrbitData::print(std::ostream& Os) const
{
  Os << "Quaternion Orbit Data\n"
     << "Time:     " << time().to_string() << "\n"
     << "Position (CartesianFixed): " << *pos << "\n"
     << "Velocity(CartesianFixed): (" << vel_cf.R_component_2() << " m/s, " 
     << vel_cf.R_component_3() <<  "m/s, "
     << vel_cf.R_component_4() << " m/s)\n"
     << "Spacecraft cartesian fixed: " << sc_to_cf_ << "\n"
     << "Aberration correction: ";
  if(aberration_correction_ == NO_CORRECTION)
    Os << "No correction\n";
  else if(aberration_correction_ == IGNORE_PLANET_ROTATION_FOR_CARTESIAN_FIXED)
    Os << "Ignore planet rotation for Cartesian fixed\n";
  else if(aberration_correction_ == FULL_CORRECTION)
    Os << "Full correction\n";
  else
    Os << "Unknown\n";
}

//-----------------------------------------------------------------------
/// This is a utility function for use by derived classes. A common
/// way of getting orbit data is to have discrete measurements of the
/// position and velocity. This function does a third order polynomial
/// interpolation to find the position and velocity at a time in
/// between the discrete measurements. For a time t between t1 and t2,
/// we have P1 and V1 the position and velocity at time t1, P2 and V2
/// the position and velocity at t2, tspace = t2 -  t1, toffset = t -
/// t1. This function then returns Pres and Vres.
//-----------------------------------------------------------------------

template<class T> inline void interpolate(const boost::array<T, 3>& P1,
		   const boost::array<T, 3>& V1,
		   const boost::array<T, 3>& P2,
		   const boost::array<T, 3>& V2,
		   const T& toffset, double tspace,
		   boost::array<T, 3>& Pres,
		   boost::array<T, 3>& Vres)
{
  const boost::array<T, 3>& c0 = P1;
  boost::array<T, 3> c1, c2, c3;
  for(int i = 0; i < 3; ++i) {
    c1[i] = V1[i] * tspace;
    c2[i] = (P2[i] - P1[i]) * 3 - (V2[i] + V1[i] * 2) * tspace;
    c3[i] = (P2[i] - P1[i]) * (-2) + (V2[i] + V1[i]) * tspace;
  }
  T t = toffset / tspace;
  for(int i = 0; i < 3; ++i) {
    Pres[i] = c0[i] + (c1[i] + (c2[i] + c3[i] * t) * t) * t;
    Vres[i] = (c1[i] + (c2[i] * 2 + c3[i] * (3 * t)) * t) / tspace;
  }
}

//-----------------------------------------------------------------------
/// This is a utility function for use by derived classes. A common
/// way of getting orbit data is to have discrete measurements of the
/// quaternion describing the rotation of the spacecraft. For a time t
/// between t1 and t2, we have Q1 as the quaternion at time t1, Q2 the
/// quaternion at time t2, tspace = t2 - t1, toffset = t - t1. This
/// function then returns Qres. We calculate this by determining the
/// axis and angle rotation that takes use from Q1 to Q2, and then do
/// a linear interpolation of that angle for the given time.
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
/// This is a utility function for use by derived classes. A common
/// way of getting orbit data is to have discrete measurements of the
/// position and velocity. This function does a third order polynomial
/// interpolation to find the position and velocity at a time in
/// between the discrete measurements. For a time t between t1 and t2,
/// we have P1 and V1 the position and velocity at time t1, P2 and V2
/// the position and velocity at t2, tspace = t2 -  t1, toffset = t -
/// t1. This function then returns Pres and Vres.
//-----------------------------------------------------------------------

void Orbit::interpolate(const boost::array<double, 3>& P1,
		   const boost::array<double, 3>& V1,
		   const boost::array<double, 3>& P2,
		   const boost::array<double, 3>& V2,
		   double toffset, double tspace,
		   boost::array<double, 3>& Pres,
		   boost::array<double, 3>& Vres)
{
  ::interpolate(P1, V1, P2, V2, toffset, tspace, Pres, Vres);
}

void Orbit::interpolate(const boost::array<AutoDerivative<double>, 3>& P1,
		   const boost::array<AutoDerivative<double>, 3>& V1,
		   const boost::array<AutoDerivative<double>, 3>& P2,
		   const boost::array<AutoDerivative<double>, 3>& V2,
		   const AutoDerivative<double>& toffset, double tspace,
		   boost::array<AutoDerivative<double>, 3>& Pres,
		   boost::array<AutoDerivative<double>, 3>& Vres)
{
  ::interpolate(P1, V1, P2, V2, toffset, tspace, Pres, Vres);
}

//-----------------------------------------------------------------------
/// This is a utility function for use by derived classes. A common
/// way of getting orbit data is to have discrete measurements of the
/// quaternion describing the rotation of the spacecraft. For a time t
/// between t1 and t2, we have Q1 as the quaternion at time t1, Q2 the
/// quaternion at time t2, tspace = t2 - t1, toffset = t - t1. This
/// function then returns Qres. We calculate this by determining the
/// axis and angle rotation that takes use from Q1 to Q2, and then do
/// a linear interpolation of that angle for the given time.
//-----------------------------------------------------------------------

boost::math::quaternion<double> Orbit::interpolate(
              const boost::math::quaternion<double>& Q1, 
              const boost::math::quaternion<double>& Q2,
	      double toffset, double tspace)
{
  return interpolate_quaternion_rotation(Q1, Q2, toffset, tspace);
}

boost::math::quaternion<AutoDerivative<double> > Orbit::interpolate(
              const boost::math::quaternion<AutoDerivative<double> >& Q1, 
              const boost::math::quaternion<AutoDerivative<double> >& Q2,
	      const AutoDerivative<double>& toffset, double tspace)
{
  return interpolate_quaternion_rotation(Q1, Q2, toffset, tspace);
}

//-----------------------------------------------------------------------
/// Velocity in CartisianFixed coordinates.
//-----------------------------------------------------------------------

boost::array<double, 3> QuaternionOrbitData::velocity_cf() const
{
  boost::array<double, 3> res = {{vel_cf.R_component_2(),
				  vel_cf.R_component_3(),
				  vel_cf.R_component_4()}};
  return res;
}

//-----------------------------------------------------------------------
/// Velocity in CartisianFixed coordinates.
//-----------------------------------------------------------------------

boost::array<AutoDerivative<double>, 3> QuaternionOrbitData::velocity_cf_with_derivative() const
{
  boost::array<AutoDerivative<double>, 3> res = 
    {{vel_cf_with_der.R_component_2(),
      vel_cf_with_der.R_component_3(),
      vel_cf_with_der.R_component_4()}};
  return res;
}


//-----------------------------------------------------------------------
/// Calculate ci_to_cf() on demand.
//-----------------------------------------------------------------------

void QuaternionOrbitData::fill_in_ci_to_cf() const 
{
    if(!have_ci_to_cf) {
      boost::array<AutoDerivative<double>, 3> vci, cider;
      boost::math::quaternion<AutoDerivative<double> > cf_to_ci_der;
      boost::array<AutoDerivative<double>, 3> pder =
	{{pos_with_der.R_component_2(),
	  pos_with_der.R_component_3(),
	  pos_with_der.R_component_4() 
	  }};
      boost::array<AutoDerivative<double>, 3> vcf = 
	{{vel_cf_with_der.R_component_2(),
	  vel_cf_with_der.R_component_3(),
	  vel_cf_with_der.R_component_4()
	  }};
      convert_position_and_velocity(tm, *pos, pder, vcf, 
				    pos_ci, cider, vci, cf_to_ci_der);
      ci_to_cf_der_ = conj(cf_to_ci_der);
      ci_to_cf_ = value(ci_to_cf_der_);
      pos_ci_with_der = boost::math::quaternion<AutoDerivative<double> >
	(0, cider[0], cider[1], cider[2]);
      vel_ci_with_der = boost::math::quaternion<AutoDerivative<double> >
	(0, vci[0], vci[1], vci[2]);
      vel_ci = value(vel_ci_with_der);
      have_ci_to_cf = true;
    }
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Interpolate between two QuaternionOrbitData for the given time.
//-----------------------------------------------------------------------

boost::shared_ptr<QuaternionOrbitData>
 QuaternionOrbitData::interpolate
(const QuaternionOrbitData& t1, const QuaternionOrbitData& t2,
 const TimeWithDerivative& tm, bool Extrapolation_ok)
{
  if(!Extrapolation_ok && (tm.value() < t1.time() || tm.value() > t2.time()))
    throw Exception("tm needs to be between t1 and t2");
  double tspace = t2.time() - t1.time();
  AutoDerivative<double> toffset = tm - t1.time_with_derivative();
  boost::math::quaternion<AutoDerivative<double> > sc_to_cf_ = 
    interpolate_quaternion_rotation(t1.sc_to_cf_with_derivative(), 
			   t2.sc_to_cf_with_derivative(), toffset, tspace);
  boost::array<AutoDerivative<double>, 3> pos1, pos2, vel1, vel2, 
    vel_cf, pos_cf;
  pos1[0] = t1.pos_with_der.R_component_2();
  pos1[1] = t1.pos_with_der.R_component_3();
  pos1[2] = t1.pos_with_der.R_component_4();
  pos2[0] = t2.pos_with_der.R_component_2();
  pos2[1] = t2.pos_with_der.R_component_3();
  pos2[2] = t2.pos_with_der.R_component_4();
  vel1[0] = t1.vel_cf_with_der.R_component_2();
  vel1[1] = t1.vel_cf_with_der.R_component_3();
  vel1[2] = t1.vel_cf_with_der.R_component_4();
  vel2[0] = t2.vel_cf_with_der.R_component_2();
  vel2[1] = t2.vel_cf_with_der.R_component_3();
  vel2[2] = t2.vel_cf_with_der.R_component_4();
  ::interpolate(pos1, vel1, pos2, vel2, toffset, tspace,
		pos_cf, vel_cf);
  boost::array<double, 3> p;
  p[0] = pos_cf[0].value();
  p[1] = pos_cf[1].value();
  p[2] = pos_cf[2].value();
  boost::shared_ptr<QuaternionOrbitData> res
    (new QuaternionOrbitData(tm, t1.pos->create(p), pos_cf, 
			     vel_cf, sc_to_cf_));
  res->from_cf_ = t1.from_cf_;
  if(t1.have_ci_to_cf) {
    t2.fill_in_ci_to_cf();
    res->have_ci_to_cf = true;
    res->ci_to_cf_der_ = interpolate_quaternion_rotation(t1.ci_to_cf_der_, t2.ci_to_cf_der_, toffset, tspace);
    res->ci_to_cf_ = value(res->ci_to_cf_der_);
    
    boost::array<AutoDerivative<double>, 3> pos1, pos2, vel1, vel2, 
      vel_ci, pos_ci;
    pos1[0] = t1.pos_ci_with_der.R_component_2();
    pos1[1] = t1.pos_ci_with_der.R_component_3();
    pos1[2] = t1.pos_ci_with_der.R_component_4();
    pos2[0] = t2.pos_ci_with_der.R_component_2();
    pos2[1] = t2.pos_ci_with_der.R_component_3();
    pos2[2] = t2.pos_ci_with_der.R_component_4();
    vel1[0] = t1.vel_ci_with_der.R_component_2();
    vel1[1] = t1.vel_ci_with_der.R_component_3();
    vel1[2] = t1.vel_ci_with_der.R_component_4();
    vel2[0] = t2.vel_ci_with_der.R_component_2();
    vel2[1] = t2.vel_ci_with_der.R_component_3();
    vel2[2] = t2.vel_ci_with_der.R_component_4();
    ::interpolate(pos1, vel1, pos2, vel2, toffset, tspace,
		  pos_ci, vel_ci);
    boost::array<double, 3> p;
    p[0] = pos_ci[0].value();
    p[1] = pos_ci[1].value();
    p[2] = pos_cf[2].value();
    res->pos_ci = t1.pos_ci->create(p);
    res->pos_ci_with_der = boost::math::quaternion<AutoDerivative<double> >
      (0,pos_ci[0], pos_ci[1], pos_ci[2]);
    res->vel_ci_with_der = boost::math::quaternion<AutoDerivative<double> >
      (0,vel_ci[0], vel_ci[1], vel_ci[2]);
    res->vel_ci = value(res->vel_ci_with_der);
  }
  return res;
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Interpolate between two QuaternionOrbitData for the given time,
/// without interpolating the derivative stuff.
//-----------------------------------------------------------------------

boost::shared_ptr<QuaternionOrbitData>
QuaternionOrbitData::interpolate
(const QuaternionOrbitData& t1, 
 const QuaternionOrbitData& t2,
 const Time& tm,
 bool Extrapolation_ok)
{
  if(!Extrapolation_ok && (tm < t1.time() || tm > t2.time()))
    throw Exception("tm needs to be between t1 and t2");
  double tspace = t2.time() - t1.time();
  double toffset = tm - t1.time();
  boost::math::quaternion<double> sc_to_cf_ = 
    interpolate_quaternion_rotation(t1.sc_to_cf(), 
			   t2.sc_to_cf(), toffset, tspace);
  boost::array<double, 3> pos1, pos2, vel1, vel2, 
    vel_cf, pos_cf;
  pos1[0] = t1.pos->position[0];
  pos1[1] = t1.pos->position[1];
  pos1[2] = t1.pos->position[2];
  pos2[0] = t2.pos->position[0];
  pos2[1] = t2.pos->position[1];
  pos2[2] = t2.pos->position[2];
  vel1[0] = t1.vel_cf.R_component_2();
  vel1[1] = t1.vel_cf.R_component_3();
  vel1[2] = t1.vel_cf.R_component_4();
  vel2[0] = t2.vel_cf.R_component_2();
  vel2[1] = t2.vel_cf.R_component_3();
  vel2[2] = t2.vel_cf.R_component_4();
  ::interpolate(pos1, vel1, pos2, vel2, toffset, tspace,
		pos_cf, vel_cf);
  boost::shared_ptr<QuaternionOrbitData> res
    (new QuaternionOrbitData(tm, t1.pos->create(pos_cf),
			     vel_cf, sc_to_cf_));
  res->from_cf_ = t1.from_cf_;
  if(t1.have_ci_to_cf) {
    t2.fill_in_ci_to_cf();
    res->have_ci_to_cf = true;
    res->ci_to_cf_der_ = interpolate_quaternion_rotation(t1.ci_to_cf_der_, t2.ci_to_cf_der_, toffset, tspace);
    res->ci_to_cf_ = value(res->ci_to_cf_der_);
    
    boost::array<double, 3> pos1, pos2, vel1, vel2, 
      vel_ci, pos_ci;
    pos1[0] = t1.pos_ci->position[0];
    pos1[1] = t1.pos_ci->position[1];
    pos1[2] = t1.pos_ci->position[2];
    pos2[0] = t2.pos_ci->position[0];
    pos2[1] = t2.pos_ci->position[1];
    pos2[2] = t2.pos_ci->position[2];
    vel1[0] = t1.vel_ci.R_component_2();
    vel1[1] = t1.vel_ci.R_component_3();
    vel1[2] = t1.vel_ci.R_component_4();
    vel2[0] = t2.vel_ci.R_component_2();
    vel2[1] = t2.vel_ci.R_component_3();
    vel2[2] = t2.vel_ci.R_component_4();
    ::interpolate(pos1, vel1, pos2, vel2, toffset, tspace,
		  pos_ci, vel_ci);
    res->pos_ci = t1.pos_ci->create(pos_ci);
    res->pos_ci_with_der = 
      boost::math::quaternion<double>(0, 
				      res->pos_ci->position[0],
				      res->pos_ci->position[1],
				      res->pos_ci->position[2]);
    res->vel_ci = boost::math::quaternion<double>
      (0,vel_ci[0], vel_ci[1], vel_ci[2]);
    res->vel_ci_with_der = res->vel_ci;
  }
  return res;
}

