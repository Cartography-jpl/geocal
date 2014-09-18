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
};

/****************************************************************//**
  This is the coordinates of a frame camera, including derivatives.
*******************************************************************/

class FrameCoordinateWithDerivative : public Printable<FrameCoordinateWithDerivative> {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  FrameCoordinateWithDerivative() {}

//-----------------------------------------------------------------------
/// Create a FrameCoordinate with the given coordinates.
//-----------------------------------------------------------------------

  FrameCoordinateWithDerivative(const AutoDerivative<double>& L, 
				const AutoDerivative<double>& S) 
 :line(L), sample(S) {}
  AutoDerivative<double> line;			///< Line number
  AutoDerivative<double> sample;		///< Sample number
  void print(std::ostream& Os) const;
};
}

#endif
