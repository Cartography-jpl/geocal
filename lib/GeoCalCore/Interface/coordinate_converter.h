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
  has been selected. The meaning of the X, Y, and Height value passed
  in and out of the functions depends on what concrete specialization
  of this class is used. For uses where the ambiguity isn't
  acceptable, it is not appropriate to use this class (i.e., the
  various Strategies of coordinate conversions aren't really
  exchangeable then).
*******************************************************************/

class CoordinateConverter : public Printable<CoordinateConverter> {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~CoordinateConverter() {}

//-----------------------------------------------------------------------
/// This converts from a particular coordinate system, returning a ground
/// coordinate for the given coordinates. The specific meaning and units
/// of X, Y, and height depend on which coordinates system is used by a
/// specialization of this class.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const = 0;

//-----------------------------------------------------------------------
/// This converts from a ground coordinate to a particular coordinate
/// system. The specific meaning and units of X, Y, and height depend on
/// which coordinates system is used by a specialization of this class.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Height) const = 0;

//-----------------------------------------------------------------------
/// Test if two CoordinateConverters are the same coordinate system.
//-----------------------------------------------------------------------

  virtual bool is_same(const CoordinateConverter& Conv) const = 0;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;
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
/// degrees, and height is in meters.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const
  {
    return boost::shared_ptr<GroundCoordinate>(new Geodetic(Y, X, Height));
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
/// degrees, and height is in meters.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Height) const
  {
    Geodetic gd(Gc);
    X = gd.longitude();
    Y = gd.latitude();
    Height = gd.height_reference_surface();
  }

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "Geodetic Coordinate Converter"; }
};
}
#endif
