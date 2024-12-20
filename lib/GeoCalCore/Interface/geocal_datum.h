#ifndef DATUM_H
#define DATUM_H
#include "ground_coordinate.h"
#include "geodetic.h"
#include "printable.h"

namespace GeoCal {
/****************************************************************//**
  This is a Datum, which returns the distance from mean sea level to a
  reference ellipsoid.
*******************************************************************/

class Datum : public Printable<Datum> {
public:
//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

  virtual ~Datum() {}

//-----------------------------------------------------------------------
/// Undulation, which is the distance form mean sea level to the
/// reference ellipsoid, for the given ground location. This is in
/// meters. 
//-----------------------------------------------------------------------

  virtual double undulation(const GroundCoordinate& Gc) const = 0;

//-----------------------------------------------------------------------
/// Specialization for Gc being Geodetic. Since many of our Datums are
/// in geodetic coordinates, this is an important specialization for
/// performance.
//-----------------------------------------------------------------------

  virtual double undulation(const Geodetic& Gc) const = 0;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const = 0;

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a Datum which returns a constant undulation everywhere. This
  is useful for testing, and also in the case you are using a Dem with
  height already relative to the reference surface, so no Datum
  correction is needed.
*******************************************************************/

class SimpleDatum : public Datum {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  SimpleDatum(double U = 0.0) : u_(U) {}

//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

  virtual ~SimpleDatum() {}

//-----------------------------------------------------------------------
/// Fixed value that is returned by this class.
//-----------------------------------------------------------------------
  double u() const {return u_;}

//-----------------------------------------------------------------------
/// Undulation, which is the distance form mean sea level to the
/// reference ellipsoid, for the given ground location. This is in
/// meters. This is always 0 for this class.
//-----------------------------------------------------------------------

  virtual double undulation(const GroundCoordinate& UNUSED(Gc)) const { return u_; }
  virtual double undulation(const Geodetic& UNUSED(Gc)) const { return u_; }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "Simple Datum, undulation " << u_ << "m \n"; }
private:
  double u_;			///< Value to return everywhere.
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

typedef SimpleDatum NoDatum;	///< More descriptive name in some cases
}

GEOCAL_EXPORT_KEY(Datum);
GEOCAL_EXPORT_KEY(SimpleDatum);
#endif
