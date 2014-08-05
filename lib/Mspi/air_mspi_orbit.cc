#include "air_mspi_orbit.h"
#include "ostream_pad.h"
#include "constant.h"
#include "aircraft_orbit_data.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor, that takes raw data and create AirMspiNavData from
/// this.
/// Note that I'm not sure about the datum here, the height might be
/// relative to WGS-84 already. I'll check with Mike Bull.
//-----------------------------------------------------------------------

AirMspiNavData::AirMspiNavData(const blitz::Array<double, 1>& d,
			       const Datum& datum,
			       bool Old_format)
{
  double u = datum.undulation(Geodetic(Constant::rad_to_deg * d(1),
				Constant::rad_to_deg * d(2)));
  position = Geodetic(Constant::rad_to_deg * d(1),
		      Constant::rad_to_deg * d(2),
		      d(0) + u);
  velocity[0] = d(5);
  velocity[1] = d(4);
  velocity[2] = d(3);
  ypr[0] = d(11);
  ypr[1] = d(6);
  ypr[2] = d(7);
  ypr_rate[0] = d(10);
  ypr_rate[1] = d(8);
  ypr_rate[2] = d(9);
  gimbal_pos = d(12);
  gimbal_vel = d(13);
  if(Old_format) {
    ypr_rate[0] = -d(10);
    double drift_angle = d(11);
    double track_made_good_angle = atan2(velocity[0], velocity[1]);
    double true_heading = track_made_good_angle - drift_angle;
    ypr[0] = true_heading;
  }
}


void AirMspiNavData::print(std::ostream& Os) const {
  Os << "AirMisrNavData:\n"
     << "  Position: " << position << "\n"
     << "  Velocity:  (" << velocity[0] << ", " << velocity[1] << ", " 
     << velocity[2] << ") m/s\n"
     << "  Ypr:       (" << ypr[0] << ", " << ypr[1] << ", "
     <<  ypr[2] << ") radians\n"
     << "  Ypr rate:  (" << ypr_rate[0] << ", " << ypr_rate[1] << ", "
     <<  ypr_rate[2] << ") radians/s\n"
     << "  Gimbal pos: " << gimbal_pos << " radians\n"
     << "  Gimbal vel: " << gimbal_vel << " radians/sec\n";
}

// This interpolation is used just for reporting data. We actually
// interpolate the final quaternions in the orbit file. But this is
// useful for some of the data dumping tools Mike Bull uses.

AirMspiNavData AirMspiNavData::interpolate(const AirMspiNavData& n1, 
					   const AirMspiNavData& n2,
					   double f)
{
  AirMspiNavData res;
  double lon_interp = 
    interpolate_angle(n1.position.longitude() * Constant::deg_to_rad,
		      n2.position.longitude() * Constant::deg_to_rad, f);
  res.position = Geodetic(n1.position.latitude() +
			  (n2.position.latitude() - n1.position.latitude()) * f,
			  lon_interp * Constant::rad_to_deg,
			  n1.position.height_reference_surface() +
			  (n2.position.height_reference_surface() - 
			   n1.position.height_reference_surface()) * f);
  for(int i = 0; i < 3; ++i) {
    res.velocity[i] = n1.velocity[i] + (n2.velocity[i] - n1.velocity[i]) * f;
    res.ypr[i] = interpolate_angle(n1.ypr[i], n2.ypr[i], f);
    res.ypr_rate[i] = n1.ypr_rate[i] + (n2.ypr_rate[i] - n1.ypr_rate[i]) * f;
  }
  res.gimbal_pos = n1.gimbal_pos + (n2.gimbal_pos - n1.gimbal_pos) * f;
  res.gimbal_vel = n1.gimbal_vel + (n2.gimbal_vel - n1.gimbal_vel) * f;
  return res;
}


double AirMspiNavData::interpolate_angle(double v1, double v2, double f)
{
  double v1x = cos(v1);
  double v1y = sin(v1);
  double v2x = cos(v2);
  double v2y = sin(v2);
  double newx = v1x + f * (v2x - v1x);
  double newy = v1y + f * (v2y - v1y);
  return atan2(newy, newx);
}

//-----------------------------------------------------------------------
/// Read the given file.
//-----------------------------------------------------------------------

AirMspiOrbit::AirMspiOrbit(const std::string& Fname, 
			   const blitz::Array<double, 1>& Gimbal_angle,
			   const blitz::Array<double, 1>& Ypr_corr,
			   const boost::shared_ptr<Datum>& D,
			   AircraftOrbitData::VerticalDefinition Def)
  : data(new GdalRasterImage(Fname)), 
    datum_(D),
    gimbal_angle_(Gimbal_angle.copy()),
    ypr_corr_(Ypr_corr.copy()),
    vdef_(Def)
{
  initialize();
}

AirMspiOrbit::AirMspiOrbit(const std::string& Fname, 
			   const blitz::Array<double, 1>& Gimbal_angle,
			   const boost::shared_ptr<Datum>& D,
			   AircraftOrbitData::VerticalDefinition Def)
  : data(new GdalRasterImage(Fname)), 
    datum_(D),
    gimbal_angle_(Gimbal_angle.copy()),
    ypr_corr_(3),
    vdef_(Def)
{
  ypr_corr_ = 0;
  initialize();
}

AirMspiOrbit::AirMspiOrbit(const std::string& Fname, 
			   const boost::shared_ptr<Datum>& D,
			   AircraftOrbitData::VerticalDefinition Def)
  : data(new GdalRasterImage(Fname)), 
    datum_(D),
    gimbal_angle_(3),
    ypr_corr_(3),
    vdef_(Def)
{
  gimbal_angle_ = 0;
  ypr_corr_ = 0;
  initialize();
}

void AirMspiOrbit::initialize()
{
  if(gimbal_angle_.rows() != 3)
    throw Exception("Gimbal_angle needs to be size 3");
  if(ypr_corr_.rows() != 3)
    throw Exception("Ypr_corr needs to be size 3");
  Time epoch = Time::parse_time(data->metadata<std::string>("epoch"));
  min_tm = epoch + data->metadata<double>("first_time");
  max_tm = epoch + data->metadata<double>("last_time");
  tspace_ = data->metadata<double>("interval");
  old_format = (data_version() == "v0");
  // Portion of quaternion that is constant.
  // Negative sign is because these are passive rotations 
  // (the quaternion definition is active)
  m = quat_rot("zyx", -gimbal_angle_(0) * Constant::deg_to_rad, 
	       -gimbal_angle_(1) * Constant::deg_to_rad, 
	       -gimbal_angle_(2) * Constant::deg_to_rad);
}

//-----------------------------------------------------------------------
/// Navigation data for the given index.
//-----------------------------------------------------------------------

AirMspiNavData AirMspiOrbit::nav_data(int Index) const
{
  range_check(Index, 0, data->number_line());
  blitz::Array<double, 2> raw_data = 
    data->read_double(Index, 0, 1, data->number_sample());
  return AirMspiNavData(raw_data(0, blitz::Range::all()), *datum_, old_format);
}

//-----------------------------------------------------------------------
/// Nav data at the given time. Note that we interpolate the nav data,
/// in a way *different* than we do for the actual orbit_data. This is
/// meant for use in various reporting tools Mike Bull uses, not for
/// actually calculating the orbit data with.
//-----------------------------------------------------------------------

AirMspiNavData AirMspiOrbit::nav_data(Time T) const
{
  range_check(T, min_time(), max_time());
  double x = (T - min_time()) / time_spacing();
  int i = (int) floor(x);
  double f = x - i;
  return AirMspiNavData::interpolate(nav_data(i), nav_data(i + 1), f);
}

//-----------------------------------------------------------------------
/// Orbit data for given index.
//-----------------------------------------------------------------------

boost::shared_ptr<QuaternionOrbitData> AirMspiOrbit::orbit_data_index(int Index) const
{
  range_check(Index, 0, data->number_line() - 1);
  AirMspiNavData n1 = nav_data(Index);
  AirMspiNavData n2 = nav_data(Index + 1);
  // This goes from station to spacecraft
  boost::math::quaternion<double> static_to_sc =
    m * quat_rot("y", n1.gimbal_pos);
  // This goes from spacecraft to cf
  AircraftOrbitData od(min_time() + Index * time_spacing(), n1.position,
		       min_time() + (Index + 1) * time_spacing(), n2.position,
		       n1.ypr[2] * Constant::rad_to_deg + ypr_corr_(2),
		       n1.ypr[1] * Constant::rad_to_deg + ypr_corr_(1),
		       n1.ypr[0] * Constant::rad_to_deg + ypr_corr_(0),
		       vdef_);
  // Tack these 2 transforms together
  return boost::shared_ptr<QuaternionOrbitData>
    (new QuaternionOrbitData(od.time(), od.position_cf(), od.velocity_cf(),
			     od.sc_to_cf() * static_to_sc));
}

boost::shared_ptr<OrbitData> AirMspiOrbit::orbit_data(Time T) const
{
  range_check(T, min_time(), max_time());
  int i = (int) floor((T - min_time()) / time_spacing());
  boost::shared_ptr<QuaternionOrbitData> q1 = orbit_data_index(i); 
  boost::shared_ptr<QuaternionOrbitData> q2 = orbit_data_index(i + 1); 
  return GeoCal::interpolate(*q1, *q2, T);
}

void AirMspiOrbit::print(std::ostream& Os) const {
  OstreamPad opad(Os, "    ");
  Os << "AirMspiOrbit:\n"
     << "  File name:    " << file_name() << "\n"
     << "  Data version: " << data_version() << "\n"
     << "  Min time:     " << min_time() << "\n"
     << "  Max time:     " << max_time() << "\n"
     << "  Time spacing: " << time_spacing() << "\n"
     << "  Flight description:\n";
  opad << flight_description() << "\n";
  opad.strict_sync();
}