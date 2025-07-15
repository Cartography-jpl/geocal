#ifndef COORDINATE_CONVERTER_H
#define COORDINATE_CONVERTER_H
#include "ground_coordinate.h"
#include "dem.h"
#include "geodetic.h"
#include "printable.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This class is used to convert from and to a particular coordinate
  system. There are some functions that can work equal well with any
  map coordinate system, and where we want the decision of what
  coordinate system to be made outside of the function (e.g., the
  program map_project, where the user selects the coordinate system to
  map project to).

  The solution we use here is to make use of the Strategy pattern (see
  "Design Patterns" by Erich Gamma et. al.). This class is the base
  class defining the interface to a general coordinate convertor.

  This class allows conversion to and from whatever coordinate system
  has been selected. The meaning of the X, Y, and Z value passed
  in and out of the functions depends on what concrete specialization
  of this class is used. For uses where the ambiguity isn't
  acceptable, it is not appropriate to use this class (i.e., the
  various Strategies of coordinate conversions aren't really
  exchangeable then).
*******************************************************************/

class CoordinateConverter : public Printable<CoordinateConverter> {
public:
  enum {EARTH_NAIF_CODE = 399};

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~CoordinateConverter() {}

//-----------------------------------------------------------------------
/// This converts from a particular coordinate system, returning a ground
/// coordinate for the given coordinates. The specific meaning and units
/// of X, Y, and Z depend on which coordinates system is used by a
/// specialization of this class.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const = 0;

//-----------------------------------------------------------------------
/// This converts from a ground coordinate to a particular coordinate
/// system. The specific meaning and units of X, Y, and Z depend on
/// which coordinates system is used by a specialization of this class.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Z) const = 0;

//-----------------------------------------------------------------------
/// Convert and create a coordinate.
//-----------------------------------------------------------------------
  virtual boost::shared_ptr<GroundCoordinate> create
  (const GroundCoordinate& Gc) const
  {
    double x, y, z;
    convert_to_coordinate(Gc, x, y, z);
    return convert_from_coordinate(x, y, z);
  }
//-----------------------------------------------------------------------
/// Specialization that converts from Geodetic. Because much of our
/// data is in Geodetic coordinates this is an important performance
/// specialization.  
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const Geodetic& Gc, double& X, 
			       double& Y, double& Z) const = 0;

//-----------------------------------------------------------------------
/// Test if two CoordinateConverters are the same coordinate system.
//-----------------------------------------------------------------------

  virtual bool is_same(const CoordinateConverter& Conv) const = 0;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;

//-----------------------------------------------------------------------
/// Naif code for planet
//-----------------------------------------------------------------------

  virtual int naif_code() const { return EARTH_NAIF_CODE; }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  CoordinateConverter that goes to and from Geodetic coordinates.
*******************************************************************/

class GeodeticConverter : public CoordinateConverter {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GeodeticConverter() {}

//-----------------------------------------------------------------------
/// Convert to geodetic. X and Y are longitude and latitude in
/// degrees, and Z is height is in meters.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const
  {
    return boost::shared_ptr<GroundCoordinate>(new Geodetic(Y, X, Z));
  }

//-----------------------------------------------------------------------
/// Test if two CoordinateConverters are the same coordinate system.
//-----------------------------------------------------------------------

  virtual bool is_same(const CoordinateConverter& Conv) const
  {
    return dynamic_cast<const GeodeticConverter*>(&Conv);
  }


//-----------------------------------------------------------------------
/// Convert to geodetic. X and Y are longitude and latitude in
/// degrees, and Z is height is in meters.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Z) const
  {
    Geodetic gd(Gc);
    X = gd.longitude();
    Y = gd.latitude();
    Z = gd.height_reference_surface();
  }

  virtual void convert_to_coordinate(const Geodetic& Gc, double& X, 
			       double& Y, double& Z) const
  {
    X = Gc.longitude();
    Y = Gc.latitude();
    Z = Gc.height_reference_surface();
  }

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "Geodetic Coordinate Converter\n"; }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  CoordinateConverter that goes to and from Geodetic360 coordinates.
*******************************************************************/

class Geodetic360Converter : public CoordinateConverter {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Geodetic360Converter() {}

//-----------------------------------------------------------------------
/// Convert to geodetic360. X and Y are longitude and latitude in
/// degrees, and Z is height is in meters.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const
  {
    return boost::shared_ptr<GroundCoordinate>(new Geodetic360(Y, X, Z));
  }

//-----------------------------------------------------------------------
/// Test if two CoordinateConverters are the same coordinate system.
//-----------------------------------------------------------------------

  virtual bool is_same(const CoordinateConverter& Conv) const
  {
    return dynamic_cast<const Geodetic360Converter*>(&Conv);
  }


//-----------------------------------------------------------------------
/// Convert to geodetic. X and Y are longitude and latitude in
/// degrees, and Z is height is in meters.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Z) const
  {
    Geodetic360 gd(Gc);
    X = gd.longitude();
    Y = gd.latitude();
    Z = gd.height_reference_surface();
  }

  virtual void convert_to_coordinate(const Geodetic& Gc, double& X, 
			       double& Y, double& Z) const
  {
    X = Gc.longitude();
    if(X < 0)
      X += 360;
    Y = Gc.latitude();
    Z = Gc.height_reference_surface();
  }

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "Geodetic360 Coordinate Converter\n"; }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
  
/****************************************************************//**
  CoordinateConverter that goes to and from Geodetic coordinates. This
  variations uses radians instead
*******************************************************************/

class GeodeticRadianConverter : public CoordinateConverter {
public:
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GeodeticRadianConverter() {}

//-----------------------------------------------------------------------
/// Convert to geodetic. X and Y are longitude and latitude in
/// degrees, and Z is height is in meters.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const
  {
    return boost::shared_ptr<GroundCoordinate>(new Geodetic(Y * Constant::rad_to_deg, X * Constant::rad_to_deg, Z));
  }

//-----------------------------------------------------------------------
/// Test if two CoordinateConverters are the same coordinate system.
//-----------------------------------------------------------------------

  virtual bool is_same(const CoordinateConverter& Conv) const
  {
    return dynamic_cast<const GeodeticRadianConverter*>(&Conv);
  }


//-----------------------------------------------------------------------
/// Convert to geodetic. X and Y are longitude and latitude in
/// degrees, and Z is height is in meters.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Z) const
  {
    Geodetic gd(Gc);
    X = gd.longitude() * Constant::deg_to_rad;
    Y = gd.latitude() * Constant::deg_to_rad;
    Z = gd.height_reference_surface();
  }

  virtual void convert_to_coordinate(const Geodetic& Gc, double& X, 
			       double& Y, double& Z) const
  {
    X = Gc.longitude() * Constant::deg_to_rad;
    Y = Gc.latitude() * Constant::deg_to_rad;
    Z = Gc.height_reference_surface();
  }

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "Geodetic Coordinate Converter (Radians)\n"; }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  CoordinateConverter that goes to and from Geodetic coordinates. This
  variations uses radians instead. This version has longitude between
  0 and 360 degrees instead of -180 to 180
*******************************************************************/

class GeodeticRadian2piConverter : public CoordinateConverter {
public:
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GeodeticRadian2piConverter() {}

//-----------------------------------------------------------------------
/// Convert to geodetic. X and Y are longitude and latitude in
/// degrees, and Z is height is in meters.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Z = 0) const
  {
    double x = X * Constant::rad_to_deg;
    return boost::shared_ptr<GroundCoordinate>(new Geodetic(Y * Constant::rad_to_deg, (x > 180 ? x - 360 : x), Z));
  }

//-----------------------------------------------------------------------
/// Test if two CoordinateConverters are the same coordinate system.
//-----------------------------------------------------------------------

  virtual bool is_same(const CoordinateConverter& Conv) const
  {
    return dynamic_cast<const GeodeticRadian2piConverter*>(&Conv);
  }


//-----------------------------------------------------------------------
/// Convert to geodetic. X and Y are longitude and latitude in
/// degrees, and Z is height is in meters.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Z) const
  {
    Geodetic gd(Gc);
    X = gd.longitude();
    X = (X < 0 ? X + 360 : X);
    X *= Constant::deg_to_rad;
    Y = gd.latitude() * Constant::deg_to_rad;
    Z = gd.height_reference_surface();
  }

  virtual void convert_to_coordinate(const Geodetic& Gc, double& X, 
			       double& Y, double& Z) const
  {
    X = Gc.longitude();
    X = (X < 0 ? X + 360 : X);
    X *= Constant::deg_to_rad;
    Y = Gc.latitude() * Constant::deg_to_rad;
    Z = Gc.height_reference_surface();
  }

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "Geodetic Coordinate Converter (Radians, 0 to 2pi)\n"; }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(CoordinateConverter);
GEOCAL_EXPORT_KEY(GeodeticConverter);
GEOCAL_EXPORT_KEY(Geodetic360Converter);
GEOCAL_EXPORT_KEY(GeodeticRadianConverter);
GEOCAL_EXPORT_KEY(GeodeticRadian2piConverter);
#endif
