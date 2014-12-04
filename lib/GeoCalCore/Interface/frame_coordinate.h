#ifndef FRAME_COORDINATE_H
#define FRAME_COORDINATE_H
#include "printable.h"
#include "auto_derivative.h"

namespace GeoCal {

/****************************************************************//**
  This is the coordinates of a frame camera.
*******************************************************************/

class FrameCoordinate : public Printable<FrameCoordinate> {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  FrameCoordinate() {}

//-----------------------------------------------------------------------
/// Create a FrameCoordinate with the given coordinates.
//-----------------------------------------------------------------------

  FrameCoordinate(double L, double S) :line(L), sample(S) {}
  double line;			///< Line number
  double sample;		///< Sample number
  void print(std::ostream& Os) const;
private:
#ifdef USE_BOOST_SERIALIZATON
  friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GenericObject);
    ar & GEOCAL_NVP(line) & GEOCAL_NVP(sample);
  }
#endif
};

/****************************************************************//**
  This is the coordinates of a frame camera, including derivatives.
*******************************************************************/

class FrameCoordinateWithDerivative : 
    public Printable<FrameCoordinateWithDerivative> {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  FrameCoordinateWithDerivative() {}

//-----------------------------------------------------------------------
/// Create FrameCoordinateWithDerivative from FrameCoordinate, so line
/// and sample are constants rather than having a gradient.
//-----------------------------------------------------------------------
  FrameCoordinateWithDerivative(const FrameCoordinate& F)
  {
    line = F.line;
    sample = F.sample;
  }

//-----------------------------------------------------------------------
/// Create a FrameCoordinateWithDerivative with the given coordinates.
//-----------------------------------------------------------------------

  FrameCoordinateWithDerivative(const AutoDerivative<double>& L, 
				const AutoDerivative<double>& S) 
 :line(L), sample(S) {}
  AutoDerivative<double> line;			///< Line number
  AutoDerivative<double> sample;		///< Sample number
  void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Strip off derivative information and return FrameCoordinate.
//-----------------------------------------------------------------------
  FrameCoordinate value() const
  { return FrameCoordinate(line.value(), sample.value()); }
#ifdef USE_BOOST_SERIALIZATON
  friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GenericObject);
    ar & GEOCAL_NVP(line) & GEOCAL_NVP(sample);
  }
#endif
};
}

#endif
