#ifndef AIRCRAFT_ORBIT_DATA_H
#define AIRCRAFT_ORBIT_DATA_H
#include "orbit.h"
#include "geodetic.h"

namespace GeoCal {
/****************************************************************//**
  This is a single navigation file record for aircraft data. This was
  originally written for data as reported by Applanix ephemeris and
  attitude, but it is likely to be useful for other INU untis. 

  Note that we frequently don't have access to a direct measurement of
  the velocity. Rather than trying to do some clever estimate of the
  velocity, we just set it to 0. The velocity is only used for the
  abberation of light calculation, which isn't important for aircraft
  speeds. We can revisit this choice if needed, it wouldn't be too
  hard to use a couple of position measurements at nearby times to
  estimate the velocity.
*******************************************************************/

class AircraftOrbitData : public QuaternionOrbitData {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  AircraftOrbitData(const Time& Tm,
		    const GroundCoordinate& Position, 
		    const boost::array<double, 3>& Vel_fixed,
		    double Roll, double Pitch,
		    double Heading)
  {
    initialize(Tm, Position, Vel_fixed, Roll, Pitch, Heading);
  }
  AircraftOrbitData(const Time& Tm,
		    const GroundCoordinate& Position, 
		    const Time& Tm2,
		    const GroundCoordinate& Position2, 
		    double Roll, double Pitch,
		    double Heading);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~AircraftOrbitData() {}
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Position of aircraft.
//-----------------------------------------------------------------------

  const Geodetic& position_geodetic() const { return position_geodetic_; }

//-----------------------------------------------------------------------
/// Roll, in degrees
//-----------------------------------------------------------------------

  double roll() const {return roll_;}

//-----------------------------------------------------------------------
/// Pitch, in degrees
//-----------------------------------------------------------------------

  double pitch() const {return pitch_;}

//-----------------------------------------------------------------------
/// Heading, in degrees
//-----------------------------------------------------------------------

  double heading() const {return heading_;}
private:
  void initialize(const Time& Tm,
		  const Geodetic& Position, 
		  const boost::array<double, 3>& Vel_fixed,
		  double Roll, double Pitch,
		  double Heading);
  double roll_, pitch_, heading_;
  Geodetic position_geodetic_;
};
}
#endif
