#ifndef SPOT_ORBIT_H
#define SPOT_ORBIT_H
#include "orbit.h"
#include <boost/math/quaternion.hpp>
#include <map>
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This class models the SPOT orbit. The orbit parameters are given 
  by an DIMAP file. This file is an XML file format. You can find 
  documentation for this format at
  http://www.spot.com/web/SICORP/452-sicorp-the-dimap-format.php.

  We don't actually read this file format. XML is a bit of a pain to
  deal with in C++, and is much easier to process in Ruby. So we set
  up an interface that just takes the Ephemeris and Attitude
  measurements.  The Afids class Dimap processes the XML file and 
  can use that to create an instance of this class.

  The various angles and so forth are described in the "SPOT Geometry
  Handbook", available at
  http://www.spotimage.com/automne_modules_files/standard/public/p229_0b9c0d94a22e77aac09df2b360c73073SPOT_Geometry_Handbook.pdf

   A note for a developer working with this class. The definition of
   space craft and orbital coordinate used internally has a different 
   sign convention than we've used on other instruments such as
   MISR. 

   We use the conventions spelled out in the "SPOT Geometry Handbook"
   consistently in this class, even though this is different than what
   is used in the MISR ATB.  In particular, the Z axis points from
   earth center to spacecraft, not spacecraft to earth center as we've
   used on MISR. This is all accounted for by this class, for the
   outside user everything works as expected. But if you are working 
   internally on this class this might seem strange.

   Note also that according to the "SPOT Geometry Handbook" the reported
   attitudes are relative to -X, -Y, and Z axis. This means that
   pitch and roll (but not yaw) have the opposite sign of what you
   would expect. The Ypr given to this routine should match what is
   supplied by SPOT in its DIMAP file - we account for the strange
   sign in this class.
*******************************************************************/

class SpotOrbit : public Orbit {
public:
  SpotOrbit(const std::vector<Time>& Ephemeris_time, 
	    const blitz::Array<double, 2>& Ephemeris,
	    const std::vector<Time>& Attitude_time,
	    const blitz::Array<double, 2>& Ypr);
  virtual ~SpotOrbit() {}
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;
  virtual void print(std::ostream& Os) const;
private:
  std::vector<Time> teph;
  std::vector<blitz::Array<double, 1> > posvel;
  std::map<Time, boost::math::quaternion<double> > sc_to_orb;
  SpotOrbit() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(SpotOrbit);
#endif
