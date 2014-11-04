#ifndef GROUND_MSPI_ORBIT_H
#define GROUND_MSPI_ORBIT_H
#include "orbit.h"		// Definition of Orbit

namespace GeoCal {
/****************************************************************//**
  This models the MSPI ground orbit. We don't actually move, of
  course, but the zenith angle does change over time.
*******************************************************************/

class GroundMspiOrbit : public Orbit {
public:
//-----------------------------------------------------------------------
/// Constructor. The angles should be in degrees, and the rate is in
/// degrees per second.
//-----------------------------------------------------------------------

  GroundMspiOrbit(const Time& Start_time,
		  boost::shared_ptr<GroundCoordinate>& Pos,
		  double Azimuth,
		  double Start_elevation_angle,
		  double Rotation_rate)
    : tstart(Start_time),
      pos(Pos),
      azimuth_(Azimuth),
      start_elevation_angle_(Start_elevation_angle),
      rotation_rate_(Rotation_rate)
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GroundMspiOrbit() {}

//-----------------------------------------------------------------------
/// Starting time
//-----------------------------------------------------------------------

  Time start_time() const { return tstart; }

//-----------------------------------------------------------------------
/// Position
//-----------------------------------------------------------------------

  const boost::shared_ptr<GroundCoordinate>& position() const {return pos;}

//-----------------------------------------------------------------------
/// Azimuth angle, in degrees 
//-----------------------------------------------------------------------

  double azimuth() const {return azimuth_;}

//-----------------------------------------------------------------------
/// Starting elevation angle, in degrees.
//-----------------------------------------------------------------------

  double start_elevation_angle() const {return start_elevation_angle_;}

//-----------------------------------------------------------------------
/// Rotation rate of elevation angle, in deg/sec
//-----------------------------------------------------------------------

  double rotation_rate() const {return rotation_rate_; }

  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;
  virtual void print(std::ostream& Os) const;
private:
  Time tstart;
  boost::shared_ptr<GroundCoordinate> pos;
  double azimuth_, start_elevation_angle_, rotation_rate_;
};
}
#endif

