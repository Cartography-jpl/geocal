#include "orbit.h"
#include "eci.h"
#include "geocal_matrix.h"
#include "functor.h"
#include "geocal_gsl_root.h"
#include "constant.h"
#include <blitz/array.h>
#include <cmath>

using namespace GeoCal;

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
  boost::shared_ptr<CartesianFixed> p1 = position_cf();
  boost::shared_ptr<CartesianFixed> p2 = Gc.convert_to_cf();
  CartesianFixedLookVector lv;
  for(int i = 0; i < 3; ++i)
    lv.look_vector[i] = p2->position[i] - p1->position[i];
  ScLookVector sl = sc_look_vector(lv);
  return C.frame_coordinate(sl, Band);
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

class KeplerEquation: public DFunctor {
public:
  KeplerEquation(double wt, double e) 
    : wt_(fmod(wt, 2 * Constant::pi)), e_(e) 
  {
    if(wt_ < 0)
      wt_ += 2 * Constant::pi;
  }
  virtual ~KeplerEquation() {}
  virtual double operator()(const double& psi) const
  { return wt_ - (psi - e_ * sin(psi));}
private:
  double wt_;
  double e_;
};

//-----------------------------------------------------------------------
/// Calculate OrbitData for KeplerOrbit.
//-----------------------------------------------------------------------

inline double dotb(const blitz::Array<double, 1>& x, 
		  const blitz::Array<double, 1>& y)
{
  return sum(x * y);
}

inline double normb(const blitz::Array<double, 1>& x)
{
  return sqrt(dotb(x, x));
}

inline blitz::Array<double, 1> cross2(const blitz::Array<double, 1>& x,
				     const blitz::Array<double, 1>& y)
{
  blitz::Array<double, 1> res(3);
  res(0) = x(1) * y(2) - x(2) * y(1);
  res(1) = x(2) * y(0) - x(0) * y(2);
  res(2) = x(0) * y(1) - x(1) * y(0);
  return res;
}

boost::shared_ptr<OrbitData> KeplerOrbit::orbit_data(Time T) const
{
  using blitz::Array;
  range_check(T, min_time(), max_time());

//-----------------------------------------------------------------------
// Calculate r, theta, and their derivatives using Kepler's equations.
//-----------------------------------------------------------------------

  double eccentric_anomaly = 
    gsl_root(KeplerEquation(freq_rev_ * (T - epoch_) + ma_, e_), 0, 
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
  vel(2) = rdot * stheta + r * ctheta * thetadot;

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
  boost::array<double, 3> v2 = {{vel(0), vel(1), vel(2)}};
  return boost::shared_ptr<OrbitData>(new QuaternionOrbitData(T, pci, v2, 
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
/// Construct QuaternionOrbitData. This takes data in a CartesianFixed
/// coordinate system (e.g., Ecr coordinates).
//-----------------------------------------------------------------------

QuaternionOrbitData::QuaternionOrbitData(Time Tm, 
   const boost::shared_ptr<CartesianFixed>& pos_cf,
   const boost::array<double, 3>& vel_fixed,
   const boost::math::quaternion<double>& sc_to_cf_q)
  : tm(Tm), pos(pos_cf), vel_cf(0, vel_fixed[0], vel_fixed[1], vel_fixed[2]), 
    sc_to_cf_(sc_to_cf_q), from_cf_(true), have_ci_to_cf(false)
{ 
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
  vel_cf = boost::math::quaternion<double>(0, vel_fixed[0], vel_fixed[1], 
					   vel_fixed[2]);
  sc_to_cf_ = sc_to_cf_q;
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
{ 
  initialize(Tm, pos_ci, vel_inertial, sc_to_ci_q);
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
  ci_to_cf_ = pos_ci->ci_to_cf_quat(Tm);
  have_ci_to_cf = true;
  boost::math::quaternion<double> vel_ci(0, vel_inertial[0], vel_inertial[1], 
				      vel_inertial[2]);

  vel_cf = ci_to_cf() * vel_ci * conj(ci_to_cf());
  pos = pos_ci->convert_to_cf(Tm);
  sc_to_cf_ = ci_to_cf() * sc_to_ci_q;
}

//-----------------------------------------------------------------------
/// Convert to CartesianInertialLookVector.
//-----------------------------------------------------------------------

CartesianInertialLookVector 
QuaternionOrbitData::ci_look_vector(const ScLookVector& Sl) const
{
  // Do abberation of light correction.
  CartesianInertialLookVector res;
  double k = Sl.length() / Constant::speed_of_light;
  boost::math::quaternion<double> ci = 
    conj(ci_to_cf()) * 
    (sc_to_cf_ * Sl.look_quaternion() * conj(sc_to_cf_) - k * vel_cf)
    * ci_to_cf();
  res.look_quaternion(ci);
  return res;
}

//-----------------------------------------------------------------------
/// Convert to CartesianFixedLookVector
//-----------------------------------------------------------------------

CartesianFixedLookVector
QuaternionOrbitData::cf_look_vector(const ScLookVector& Sl) const
{
  // Do abberation of light correction.
  CartesianFixedLookVector res;
  double k = Sl.length() / Constant::speed_of_light;
  boost::math::quaternion<double> cf = sc_to_cf_ * Sl.look_quaternion() * 
    conj(sc_to_cf_) - k * vel_cf;
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
  // Do abberation of light correction.
  ScLookVector res;
  double k = Ci.length() / Constant::speed_of_light;
  boost::math::quaternion<double> sc =
    conj(sc_to_cf_) * (ci_to_cf() * Ci.look_quaternion() * conj(ci_to_cf()) + 
		      k * vel_cf) * sc_to_cf_;
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
  // Do abberation of light correction.
  ScLookVector res;
  double k = Cf.length() / Constant::speed_of_light;
  boost::math::quaternion<double> sc =
    conj(sc_to_cf_) * (Cf.look_quaternion() + k * vel_cf) * sc_to_cf_;
  res.look_quaternion(sc);
  return res;
}

//-----------------------------------------------------------------------
/// Return velocity.
//-----------------------------------------------------------------------

boost::array<double, 3> QuaternionOrbitData::velocity_ci() const
{
  boost::math::quaternion<double> vel_ci = conj(ci_to_cf()) * vel_cf * 
    ci_to_cf();
  boost::array<double, 3> res = {{vel_ci.R_component_2(), 
				  vel_ci.R_component_3(),
				  vel_ci.R_component_4()}};
  return res;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void QuaternionOrbitData::print(std::ostream& Os) const
{
  Os << "Quaternion Orbit Data\n"
     << "Time:     " << time().to_string() << "\n"
     << "Position (ECR): " << *pos << "\n"
     << "Velocity(ECR): (" << vel_cf.R_component_2() << " m/s, " 
     << vel_cf.R_component_3() <<  "m/s, "
     << vel_cf.R_component_4() << " m/s)\n"
     << "Spacecraft cartesian fixed: " << sc_to_cf_ << "\n";
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

inline void interpolate(const boost::array<double, 3>& P1,
		   const boost::array<double, 3>& V1,
		   const boost::array<double, 3>& P2,
		   const boost::array<double, 3>& V2,
		   double toffset, double tspace,
		   boost::array<double, 3>& Pres,
		   boost::array<double, 3>& Vres)
{
  const boost::array<double, 3>& c0 = P1;
  boost::array<double, 3> c1, c2, c3;
  for(int i = 0; i < 3; ++i) {
    c1[i] = V1[i] * tspace;
    c2[i] = (P2[i] - P1[i]) * 3 - (V2[i] + V1[i] * 2) * tspace;
    c3[i] = (P2[i] - P1[i]) * (-2) + (V2[i] + V1[i]) * tspace;
  }
  double t = toffset / tspace;
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

inline boost::math::quaternion<double> interpolate(
              const boost::math::quaternion<double>& Q1, 
              const boost::math::quaternion<double>& Q2,
	      double toffset, double tspace)
{
  boost::math::quaternion<double> delta_quat = Q2 * conj(Q1);
  double t = delta_quat.R_component_1();
  t = (t > 1 ? 1 : (t < -1 ? -1 : t)); // Handle t being slightly
  // out of range due to round off.
  double delta_ang = 2.0 * acos(t);
  if(delta_ang < 1e-8)	// Handle degenerate case of Q1 and Q2
    // almost the same.
    return Q1;
  double d_ang = delta_ang * toffset / tspace;
  double sratio = sin(d_ang / 2.0) / sin(delta_ang / 2.0);
  boost::math::quaternion<double> 
    d_quat(cos(d_ang / 2.0),
	   delta_quat.R_component_2() * sratio,
	   delta_quat.R_component_3() * sratio,
	   delta_quat.R_component_4() * sratio);
  return d_quat * Q1;
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

void Orbit::interpolate(const boost::array<double, 3>& P1,
		   const boost::array<double, 3>& V1,
		   const boost::array<double, 3>& P2,
		   const boost::array<double, 3>& V2,
		   double toffset, double tspace,
		   boost::array<double, 3>& Pres,
		   boost::array<double, 3>& Vres) const
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
	      double toffset, double tspace) const
{
  return ::interpolate(Q1, Q2, toffset, tspace);
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
/// \ingroup Miscellaneous
/// Interpolate between two QuaternionOrbitData for the given time.
//-----------------------------------------------------------------------

boost::shared_ptr<QuaternionOrbitData>
 GeoCal::interpolate(const QuaternionOrbitData& t1, 
		     const QuaternionOrbitData& t2, Time tm)
{
  if(tm < t1.time() || tm > t2.time())
    throw Exception("tm needs to be between t1 and t2");
  double tspace = t2.time() - t1.time();
  double toffset = tm - t1.time();
  boost::math::quaternion<double> sc_to_cf_ = 
    ::interpolate(t1.sc_to_cf(), t2.sc_to_cf(), toffset, tspace);
  boost::array<double, 3> vel1, vel2, vel_cf, pos_cf;
  vel1[0] = t1.vel_cf.R_component_2();
  vel1[1] = t1.vel_cf.R_component_3();
  vel1[2] = t1.vel_cf.R_component_4();
  vel2[0] = t2.vel_cf.R_component_2();
  vel2[1] = t2.vel_cf.R_component_3();
  vel2[2] = t2.vel_cf.R_component_4();
  ::interpolate(t1.pos->position, vel1, t2.pos->position, vel2, toffset, tspace,
		pos_cf, vel_cf);
  return boost::shared_ptr<QuaternionOrbitData>(new 
     QuaternionOrbitData(tm, t1.pos->create(pos_cf), vel_cf, sc_to_cf_));
}