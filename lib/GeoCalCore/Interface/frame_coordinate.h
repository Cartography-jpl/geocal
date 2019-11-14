#ifndef FRAME_COORDINATE_H
#define FRAME_COORDINATE_H
#include "printable.h"
#include "auto_derivative.h"

namespace GeoCal {

/****************************************************************//**
  This is the coordinates of a frame camera.

  We use the same convention that we do for ImageCoordinate, the
  center of a pixel in the frame camera is at an integer value. So the 
  first pixel goes from frame coordinate (-0.5, -0.5) to (0.5, 0.5).
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
  friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version);
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
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(FrameCoordinate);
GEOCAL_EXPORT_KEY(FrameCoordinateWithDerivative);
#endif
