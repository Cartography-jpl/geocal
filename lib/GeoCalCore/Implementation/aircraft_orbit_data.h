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
  /// I'm almost certain that this should always be
  /// GEODETIC_VERTICAL. In particular, look at
  /// http://en.wikipedia.org/wiki/Axes_conventions which seems to
  /// indicate this.
  enum VerticalDefinition { GEODETIC_VERTICAL, GEOCENTRIC_VERTICAL };

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  AircraftOrbitData(const Time& Tm,
		    const GroundCoordinate& Position, 
		    const boost::array<double, 3>& Vel_fixed,
		    double Roll, double Pitch,
		    double Heading,
		    VerticalDefinition V = GEODETIC_VERTICAL)
  {
    initialize(Tm, Position, Vel_fixed, Roll, Pitch, Heading, V);
  }
  AircraftOrbitData(const Time& Tm,
		    const GroundCoordinate& Position, 
		    const Time& Tm2,
		    const GroundCoordinate& Position2, 
		    double Roll, double Pitch,
		    double Heading,
		    VerticalDefinition V = GEODETIC_VERTICAL);

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

//-----------------------------------------------------------------------
/// Vertical direction yaw, pitch, and heading are defined relative
/// to.
/// I'm pretty sure this should be GEODETIC_VERTICAL in most cases,
/// but we allow this to be different.
//-----------------------------------------------------------------------

  VerticalDefinition vertical_definition() const {return vertical_definition_;}
protected:
  AircraftOrbitData() {}
  void initialize(const Time& Tm,
		  const Geodetic& Position, 
		  const boost::array<double, 3>& Vel_fixed,
		  double Roll, double Pitch,
		  double Heading,
		  VerticalDefinition V);
private:
  double roll_, pitch_, heading_;
  VerticalDefinition vertical_definition_;
  Geodetic position_geodetic_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(AircraftOrbitData);
#endif
